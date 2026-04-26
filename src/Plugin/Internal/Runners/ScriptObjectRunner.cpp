#include "Plugin/Internal/Runners/ScriptObjectRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep
#include "Plugin/Internal/Core/TypeSerializer.hpp"
#include "Plugin/Internal/Save/SaveScriptObjectMap.hpp"

namespace Plugin::Internal::Runners
{
	static constexpr auto OBJECTS_KEY = "objects"sv;
	static constexpr auto BOUND_OBJECT_KEY = "boundObject"sv;
	static constexpr auto PROPERTIES_KEY = "properties"sv;

	auto ScriptObjectRunner::ObjectInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<ObjectInfo, std::string>
	{
		const auto boundObjectIt = a_json.find(BOUND_OBJECT_KEY);
		if (boundObjectIt == a_json.end() || !boundObjectIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				BOUND_OBJECT_KEY);
		}

		const auto propertiesIt = a_json.find(PROPERTIES_KEY);
		if (propertiesIt != a_json.end() && !propertiesIt->is_object()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find object for key "{}")"sv,
				PROPERTIES_KEY);
		}

		auto objectInfo = ObjectInfo();
		objectInfo.boundObject = boundObjectIt->get<std::string>();

		if (propertiesIt != a_json.end()) {
			auto properties = std::unordered_map<std::string, IFileRunner::json_t>();
			properties.reserve(propertiesIt->size());

			for (const auto& [nodeKey, nodeValue] : propertiesIt->items()) {
				properties.emplace(nodeKey, nodeValue);
			}

			objectInfo.properties = std::move(properties);
		}

		return objectInfo;
	}

	auto ScriptObjectRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		const auto objectsIt = a_json.find(OBJECTS_KEY);
		if (objectsIt == a_json.end() || !objectsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				OBJECTS_KEY);
		}

		auto rootInfo = RootInfo();

		auto& objects = rootInfo.objects;
		objects.reserve(objectsIt->size());

		for (auto objectIt = objectsIt->begin(); objectIt != objectsIt->end(); objectIt++) {
			if (!objectIt->is_object()) [[unlikely]] {
				return Core::StringUtility::FormatUnexpected(
					R"(failed to find object element at index {} in array for key "{}")"sv,
					std::distance(objectsIt->begin(), objectIt), OBJECTS_KEY);
			}

			auto objectInfo = ObjectInfo::FromJson(*objectIt);
			if (!objectInfo.has_value()) {
				return std::unexpected(std::move(objectInfo).error());
			}

			objects.emplace_back(std::make_shared<ObjectInfo>(std::move(objectInfo).value()));
		}

		return rootInfo;
	}

	ScriptObjectRunner::ScriptObjectRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<Save::SaveScriptObjectMap>>& a_saveScriptObjectMap)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension)),
		  _saveScriptObjectMap(a_saveScriptObjectMap)
	{
	}

	ScriptObjectRunner::~ScriptObjectRunner() noexcept
	{
		Dispose();
	}

	auto ScriptObjectRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		auto filePaths = IFileRunner::LoadDirectoryFiles();
		if (!filePaths.has_value() && filePaths.error() != REX::PosixErrorCode::no_such_file_or_directory) [[unlikely]] {
			REX::Fail(R"(Failed to iterate directory "{}": {})"sv,
				_directoryPath.generic_string(), filePaths.error());
		}

		return a_threadPool.SubmitTaskRange(std::move(filePaths).value(), [this](const std::filesystem::path& a_filePath) {
			auto jsonRoot = IFileRunner::json_t();

			try {
				auto fileStream = std::fstream(a_filePath, std::ios::in);
				jsonRoot = IFileRunner::json_t::parse(fileStream,
					REX::Json::GetParserCallback<IFileRunner::json_t>(),
					REX::Json::ALLOW_EXCEPTIONS,
					REX::Json::IGNORE_COMMENTS);
			}
			catch (const IFileRunner::json_t::exception& error) {
				REX::LogError(R"(Failed to parse file "{}": {})"sv,
					a_filePath.generic_string(), error.what());
				return;
			}

			if (jsonRoot.empty()) {
				return;
			}

			auto rootInfo = RootInfo::FromJson(jsonRoot);
			if (!rootInfo.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}": {})"sv,
					a_filePath.generic_string(), rootInfo.error());
				return;
			}

			REX::LogDebug(R"(Loaded file "{}" with {} script(s)))"sv,
				a_filePath.generic_string(), rootInfo->objects.size());

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto ScriptObjectRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Fail("Failed to get the game's script VM."sv);
		}

		const auto runnerLock = std::scoped_lock(_mapMutex);

		for (const auto& [filePath, rootInfo] : _rootMap) {
			RunScripts(scriptVM, filePath, rootInfo->objects);
		}

		return {};
	}

	void ScriptObjectRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto ScriptObjectRunner::RunScript(
		const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
		const REX::NotNull<std::shared_ptr<ObjectInfo>>& a_script)
		-> std::optional<std::string>
	{
		const auto& objectFormat = a_script->boundObject;

		const auto objectData = Core::TypeSerializer::DeserializeObjectData(objectFormat);
		if (!objectData.has_value()) [[unlikely]] {
			return REX::Format(R"(failed to deserialize bound object "{}")"sv,
				objectFormat);
		}

		auto scriptName = RE::BSFixedString(std::string(objectData->scriptName));

		auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_scriptVM->GetScriptObjectType(scriptName, objectTypeInfo) || !objectTypeInfo || !objectTypeInfo->IsLinked()) [[unlikely]] {
			return REX::Format(R"(failed to get type info for script "{}")"sv,
				scriptName);
		}

		const auto objectHandle = objectData->objectHandle;
		if (objectHandle == RE::GameScript::HandlePolicy::EMPTY_HANDLE) [[unlikely]] {
			return REX::Format(R"(bound object "{}" has empty handle)"sv,
				objectFormat);
		}

		const auto& handlePolicy = a_scriptVM->GetObjectHandlePolicy();
		if (!handlePolicy.IsHandleObjectAvailable(objectHandle)) [[unlikely]] {
			return REX::Format(R"(bound object "{}" with handle "{:X}" is not available)"sv,
				objectFormat, objectHandle);
		}

		const auto& jsonProperties = a_script->properties;

		auto templateProperties = std::unordered_map<RE::BSFixedString, IFileRunner::json_t>();
		if (jsonProperties.has_value()) {
			templateProperties.reserve(jsonProperties->size());

			for (const auto& [propName, propValue] : jsonProperties.value()) {
				const auto nativePropName = RE::BSFixedString(propName);

				const auto* propInfo = objectTypeInfo->GetPropertyInfoByName(nativePropName);
				if (!propInfo) [[unlikely]] {
					continue;
				}

				templateProperties.emplace(nativePropName, propValue);
			}
		}

		auto saveObjectInfo = Save::SaveScriptObjectMap::ObjectInfo{
			.scriptName = scriptName,
			.objectHandle = objectHandle,
			.properties = std::move(templateProperties),
		};

		_saveScriptObjectMap->AddValue(std::make_shared<Save::SaveScriptObjectMap::ObjectInfo>(std::move(saveObjectInfo)));
		return std::nullopt;
	}

	void ScriptObjectRunner::RunScripts(
		const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<ObjectInfo>>> a_scripts)
	{
		for (const auto& script : a_scripts) {
			auto runScriptResult = RunScript(a_scriptVM, script);
			if (runScriptResult.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}": {})"sv,
					a_filePath.generic_string(), runScriptResult.value());
				continue;
			}

			REX::LogTrace(R"(Registered script object "{}" from file "{}")"sv,
				script->boundObject, a_filePath.generic_string());
		}
	}
}
