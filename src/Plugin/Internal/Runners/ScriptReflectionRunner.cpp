#include "Plugin/Internal/Runners/ScriptReflectionRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep

namespace Plugin::Internal::Runners
{
	static constexpr auto REPLACED_FUNCTIONS_KEY = "replacedFunctions"sv;
	static constexpr auto REPLACED_GETTER_FUNCTIONS_KEY = "replacedGetterFunctions"sv;
	static constexpr auto REPLACED_SETTER_FUNCTIONS_KEY = "replacedSetterFunctions"sv;
	static constexpr auto OLD_SCRIPT_NAME_KEY = "oldScriptName"sv;
	static constexpr auto OLD_FUNCTION_NAME_KEY = "oldFunctionName"sv;
	static constexpr auto NEW_STATE_NAME_KEY = "newStateName"sv;
	static constexpr auto NEW_SCRIPT_NAME_KEY = "newScriptName"sv;
	static constexpr auto NEW_FUNCTION_NAME_KEY = "newFunctionName"sv;
	static constexpr auto OLD_STATE_NAME_KEY = "oldStateName"sv;
	static constexpr auto OLD_PROPERTY_NAME_KEY = "oldPropertyName"sv;
	static constexpr auto NEW_PROPERTY_NAME_KEY = "newPropertyName"sv;

	auto ScriptReflectionRunner::ReplacedFunctionInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<ReplacedFunctionInfo, std::string>
	{
		const auto oldScriptNameIt = a_json.find(OLD_SCRIPT_NAME_KEY);
		if (oldScriptNameIt == a_json.end() || !oldScriptNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				OLD_SCRIPT_NAME_KEY);
		}

		const auto oldFunctionNameIt = a_json.find(OLD_FUNCTION_NAME_KEY);
		if (oldFunctionNameIt == a_json.end() || !oldFunctionNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				OLD_FUNCTION_NAME_KEY);
		}

		const auto oldStateNameIt = a_json.find(OLD_STATE_NAME_KEY);
		if (oldStateNameIt != a_json.end() && !oldStateNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				OLD_STATE_NAME_KEY);
		}

		const auto newScriptNameIt = a_json.find(NEW_SCRIPT_NAME_KEY);
		if (newScriptNameIt == a_json.end() || !newScriptNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				NEW_SCRIPT_NAME_KEY);
		}

		const auto newFunctionNameIt = a_json.find(NEW_FUNCTION_NAME_KEY);
		if (newFunctionNameIt == a_json.end() || !newFunctionNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				NEW_FUNCTION_NAME_KEY);
		}

		const auto newStateNameIt = a_json.find(NEW_STATE_NAME_KEY);
		if (newStateNameIt != a_json.end() && !newStateNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				NEW_STATE_NAME_KEY);
		}

		auto replacedFunctionInfo = ReplacedFunctionInfo();
		replacedFunctionInfo.oldScriptName = oldScriptNameIt->get<std::string>();
		replacedFunctionInfo.oldFunctionName = oldFunctionNameIt->get<std::string>();
		replacedFunctionInfo.newScriptName = newScriptNameIt->get<std::string>();
		replacedFunctionInfo.newFunctionName = newFunctionNameIt->get<std::string>();

		if (oldStateNameIt != a_json.end()) {
			replacedFunctionInfo.oldStateName = oldStateNameIt->get<std::string>();
		}

		if (newStateNameIt != a_json.end()) {
			replacedFunctionInfo.newStateName = newStateNameIt->get<std::string>();
		}

		return replacedFunctionInfo;
	}

	auto ScriptReflectionRunner::IReplacedPropertyFunctionInfo::FromJsonImpl(const IFileRunner::json_t& a_json, IReplacedPropertyFunctionInfo& a_functionInfo)
		-> std::optional<std::string>
	{
		const auto oldScriptNameIt = a_json.find(OLD_SCRIPT_NAME_KEY);
		if (oldScriptNameIt == a_json.end() || !oldScriptNameIt->is_string()) [[unlikely]] {
			return REX::Format(R"(failed to find string value for key "{}")"sv,
				OLD_SCRIPT_NAME_KEY);
		}

		const auto oldPropertyNameIt = a_json.find(OLD_PROPERTY_NAME_KEY);
		if (oldPropertyNameIt == a_json.end() || !oldPropertyNameIt->is_string()) [[unlikely]] {
			return REX::Format(R"(failed to find string value for key "{}")"sv,
				OLD_PROPERTY_NAME_KEY);
		}

		const auto newScriptNameIt = a_json.find(NEW_SCRIPT_NAME_KEY);
		if (newScriptNameIt == a_json.end() || !newScriptNameIt->is_string()) [[unlikely]] {
			return REX::Format(R"(failed to find string value for key "{}")"sv,
				NEW_SCRIPT_NAME_KEY);
		}

		const auto newPropertyNameIt = a_json.find(NEW_PROPERTY_NAME_KEY);
		if (newPropertyNameIt == a_json.end() || !newPropertyNameIt->is_string()) [[unlikely]] {
			return REX::Format(R"(failed to find string value for key "{}")"sv,
				NEW_PROPERTY_NAME_KEY);
		}

		a_functionInfo.oldScriptName = oldScriptNameIt->get<std::string>();
		a_functionInfo.oldPropertyName = oldPropertyNameIt->get<std::string>();
		a_functionInfo.newScriptName = newScriptNameIt->get<std::string>();
		a_functionInfo.newPropertyName = newPropertyNameIt->get<std::string>();

		return std::nullopt;
	}

	auto ScriptReflectionRunner::ReplacedGetterFunctionInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<ReplacedGetterFunctionInfo, std::string>
	{
		auto replacedGetterFunctionInfo = ReplacedGetterFunctionInfo();

		auto errorMessage = IReplacedPropertyFunctionInfo::FromJsonImpl(a_json, replacedGetterFunctionInfo);
		if (errorMessage.has_value()) {
			return std::unexpected(std::move(errorMessage).value());
		}

		return replacedGetterFunctionInfo;
	}

	auto ScriptReflectionRunner::ReplacedSetterFunctionInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<ReplacedSetterFunctionInfo, std::string>
	{
		auto replacedSetterFunctionInfo = ReplacedSetterFunctionInfo();

		auto errorMessage = IReplacedPropertyFunctionInfo::FromJsonImpl(a_json, replacedSetterFunctionInfo);
		if (errorMessage.has_value()) {
			return std::unexpected(std::move(errorMessage).value());
		}

		return replacedSetterFunctionInfo;
	}

	auto ScriptReflectionRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		const auto replacedFunctionsIt = a_json.find(REPLACED_FUNCTIONS_KEY);
		if (replacedFunctionsIt != a_json.end() && !replacedFunctionsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				REPLACED_FUNCTIONS_KEY);
		}

		auto rootInfo = RootInfo();

		if (replacedFunctionsIt == a_json.end()) {
			auto replacedFunctions = std::vector<REX::NotNull<std::shared_ptr<ReplacedFunctionInfo>>>();
			replacedFunctions.reserve(replacedFunctionsIt->size());

			for (auto replacedFunctionIt = replacedFunctionsIt->begin(); replacedFunctionIt != replacedFunctionsIt->end(); replacedFunctionIt++) {
				if (!replacedFunctionIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(replacedFunctionsIt->begin(), replacedFunctionIt), REPLACED_FUNCTIONS_KEY);
				}

				auto scriptInfo = ReplacedFunctionInfo::FromJson(*replacedFunctionIt);
				if (!scriptInfo.has_value()) {
					return std::unexpected(std::move(scriptInfo).error());
				}

				replacedFunctions.emplace_back(std::make_shared<ReplacedFunctionInfo>(std::move(scriptInfo).value()));
			}

			rootInfo.replacedFunctions = std::move(replacedFunctions);
		}

		const auto replacedGetterFunctionsIt = a_json.find(REPLACED_GETTER_FUNCTIONS_KEY);
		if (replacedGetterFunctionsIt != a_json.end() && !replacedGetterFunctionsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				REPLACED_GETTER_FUNCTIONS_KEY);
		}

		if (replacedGetterFunctionsIt != a_json.end()) {
			auto replacedGetterFunctions = std::vector<REX::NotNull<std::shared_ptr<ReplacedGetterFunctionInfo>>>();
			replacedGetterFunctions.reserve(replacedGetterFunctionsIt->size());

			for (auto replacedFunctionIt = replacedGetterFunctionsIt->begin(); replacedFunctionIt != replacedGetterFunctionsIt->end(); replacedFunctionIt++) {
				if (!replacedFunctionIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(replacedGetterFunctionsIt->begin(), replacedFunctionIt), REPLACED_GETTER_FUNCTIONS_KEY);
				}

				auto scriptInfo = ReplacedGetterFunctionInfo::FromJson(*replacedFunctionIt);
				if (!scriptInfo.has_value()) {
					return std::unexpected(std::move(scriptInfo).error());
				}

				replacedGetterFunctions.emplace_back(std::make_shared<ReplacedGetterFunctionInfo>(std::move(scriptInfo).value()));
			}

			rootInfo.replacedGetterFunctions = std::move(replacedGetterFunctions);
		}

		const auto replacedSetterFunctionsIt = a_json.find(REPLACED_SETTER_FUNCTIONS_KEY);
		if (replacedSetterFunctionsIt != a_json.end() && !replacedSetterFunctionsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				REPLACED_SETTER_FUNCTIONS_KEY);
		}

		if (replacedSetterFunctionsIt != a_json.end()) {
			auto replacedSetterFunctions = std::vector<REX::NotNull<std::shared_ptr<ReplacedSetterFunctionInfo>>>();
			replacedSetterFunctions.reserve(replacedSetterFunctionsIt->size());

			for (auto replacedFunctionIt = replacedSetterFunctionsIt->begin(); replacedFunctionIt != replacedSetterFunctionsIt->end(); replacedFunctionIt++) {
				if (!replacedFunctionIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(replacedSetterFunctionsIt->begin(), replacedFunctionIt), REPLACED_SETTER_FUNCTIONS_KEY);
				}

				auto scriptInfo = ReplacedSetterFunctionInfo::FromJson(*replacedFunctionIt);
				if (!scriptInfo.has_value()) {
					return std::unexpected(std::move(scriptInfo).error());
				}

				replacedSetterFunctions.emplace_back(std::make_shared<ReplacedSetterFunctionInfo>(std::move(scriptInfo).value()));
			}

			rootInfo.replacedSetterFunctions = std::move(replacedSetterFunctions);
		}

		return rootInfo;
	}

	ScriptReflectionRunner::ScriptReflectionRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension))
	{
	}

	ScriptReflectionRunner::~ScriptReflectionRunner() noexcept
	{
		Dispose();
	}

	auto ScriptReflectionRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
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

			const auto& replacedFunctions = rootInfo->replacedFunctions;
			const auto& replacedGetterFunctions = rootInfo->replacedGetterFunctions;
			const auto& replacedSetterFunctions = rootInfo->replacedSetterFunctions;

			REX::LogDebug(R"(Loaded file "{}" with {} replaced function(s), {} replaced getter and {} replaced setter function(s))"sv,
				a_filePath.generic_string(),
				replacedFunctions.has_value() ? replacedFunctions->size() : 0,
				replacedGetterFunctions.has_value() ? replacedGetterFunctions->size() : 0,
				replacedSetterFunctions.has_value() ? replacedSetterFunctions->size() : 0);

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto ScriptReflectionRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		const auto runnerLock = std::scoped_lock(_mapMutex);

		for (const auto& [filePath, rootInfo] : _rootMap) {
			const auto& replacedFunctions = rootInfo->replacedFunctions;
			if (replacedFunctions.has_value()) {
				RunReplacedFunctions(*internalVM, filePath, replacedFunctions.value());
			}

			const auto& replacedGetterFunctions = rootInfo->replacedGetterFunctions;
			if (replacedGetterFunctions.has_value()) {
				RunReplacedGetterFunctions(*internalVM, filePath, replacedGetterFunctions.value());
			}

			const auto& replacedSetterFunctions = rootInfo->replacedSetterFunctions;
			if (replacedSetterFunctions.has_value()) {
				RunReplacedSetterFunctions(*internalVM, filePath, replacedSetterFunctions.value());
			}
		}

		return {};
	}

	void ScriptReflectionRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto ScriptReflectionRunner::ReplaceFunction(
		RE::BSTSmartPointer<RE::BSScript::IFunction>& a_oldFunction,
		const RE::BSTSmartPointer<RE::BSScript::IFunction>& a_newFunction)
		-> std::optional<std::string>
	{
		if (!a_oldFunction) [[unlikely]] {
			return REX::Format("old function is null"sv);
		}

		if (!a_newFunction) [[unlikely]] {
			return REX::Format("new function is null"sv);
		}

		if (a_oldFunction == a_newFunction) [[unlikely]] {
			return REX::Format("old and new functions are the same"sv);
		}

		if (a_oldFunction->GetReturnType() != a_newFunction->GetReturnType()) [[unlikely]] {
			return REX::Format("return types do not match"sv);
		}

		if (a_oldFunction->GetParamCount() != a_newFunction->GetParamCount()) [[unlikely]] {
			return REX::Format("parameter counts do not match"sv);
		}

		if (!std::ranges::equal(a_oldFunction->GetParamTypes(), a_newFunction->GetParamTypes())) [[unlikely]] {
			return REX::Format("parameter types do not match"sv);
		}

		if (a_oldFunction->GetIsNative()) [[unlikely]] {
			return REX::Format("old function is native"sv);
		}

		if (a_newFunction->GetIsNative()) [[unlikely]] {
			return REX::Format("new function is native"sv);
		}

		if (a_oldFunction->GetIsStatic() != a_newFunction->GetIsStatic()) [[unlikely]] {
			return REX::Format("global/static flags do not match"sv);
		}

		if (a_oldFunction->GetUserFlags() != a_newFunction->GetUserFlags()) [[unlikely]] {
			return REX::Format("user flags do not match"sv);
		}

		auto copyErrorMessage = CopyByteCodeFunctionTo(a_newFunction, a_oldFunction);
		if (copyErrorMessage.has_value()) [[unlikely]] {
			return copyErrorMessage;
		}

		return std::nullopt;
	}

	auto ScriptReflectionRunner::CopyByteCodeFunctionTo(
		const RE::BSTSmartPointer<RE::BSScript::IFunction>& a_sourceFunction,
		RE::BSTSmartPointer<RE::BSScript::IFunction>& a_targetFunction)
		-> std::optional<std::string>
	{
		if (!a_sourceFunction) [[unlikely]] {
			return "source function is null"s;
		}

		if (!a_targetFunction) [[unlikely]] {
			return "target function is null"s;
		}

		if (a_sourceFunction == a_targetFunction) [[unlikely]] {
			return "source and target functions are the same"s;
		}

		if (a_sourceFunction->GetIsNative()) [[unlikely]] {
			return "source function is native"s;
		}

		if (a_targetFunction->GetIsNative()) [[unlikely]] {
			return "target function is native"s;
		}

		const auto sourceByteCodeFunction = RE::dynamic_pointer_cast<RE::BSScript::Internal::ScriptFunction>(a_sourceFunction);
		if (!sourceByteCodeFunction) [[unlikely]] {
			return "source function is not a byte code function"s;
		}

		const auto targetByteCodeFunction = RE::dynamic_pointer_cast<RE::BSScript::Internal::ScriptFunction>(a_targetFunction);
		if (!targetByteCodeFunction) [[unlikely]] {
			return "target function is not a byte code function"s;
		}

		const auto copyLineNumbers = [](const RE::BSTSmartPointer<RE::BSScript::Internal::ScriptFunction>& a_function) -> RE::SimpleArray<std::uint16_t> {
			const auto oldLineNumberCount = a_function->lineNumberCount;
			if (oldLineNumberCount == 0) {
				return nullptr;
			}

			const auto& oldLineNumbers = a_function->lineNumbers;
			if (!oldLineNumbers) {
				return nullptr;
			}

			auto newLineNumbers = RE::SimpleArray<std::uint16_t>(oldLineNumberCount);
			REX::Assert(static_cast<bool>(newLineNumbers));

			std::copy_n(
				reinterpret_cast<const std::byte*>(oldLineNumbers.data()),
				oldLineNumberCount * sizeof(std::uint16_t),
				reinterpret_cast<std::byte*>(newLineNumbers.data()));

			return newLineNumbers;
		};

		targetByteCodeFunction->descTable = sourceByteCodeFunction->descTable;
		targetByteCodeFunction->instructions = sourceByteCodeFunction->instructions.CopyInstructionStream();
		targetByteCodeFunction->docString = sourceByteCodeFunction->docString;
		targetByteCodeFunction->sourceFileName = sourceByteCodeFunction->sourceFileName;
		targetByteCodeFunction->lineNumberCount = sourceByteCodeFunction->lineNumberCount;
		targetByteCodeFunction->lineNumbers = copyLineNumbers(sourceByteCodeFunction);

		return std::nullopt;
	}

	auto ScriptReflectionRunner::RunReplacedFunction(
		RE::BSScript::Internal::VirtualMachine& a_internalVM,
		const REX::NotNull<std::shared_ptr<ReplacedFunctionInfo>>& a_replacedFunctions)
		-> std::optional<std::string>
	{
		const auto oldScriptName = RE::BSFixedString(a_replacedFunctions->oldScriptName);
		const auto oldFunctionName = RE::BSFixedString(a_replacedFunctions->oldFunctionName);
		const auto oldStateName = a_replacedFunctions->oldStateName.has_value() ?
									  RE::BSFixedString(a_replacedFunctions->oldStateName.value()) :
									  RE::BSScript::ObjectTypeInfo::GetEmptyStateName();

		const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);

		auto oldTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_internalVM.GetScriptObjectType(oldScriptName, oldTypeInfo) || !oldTypeInfo || !oldTypeInfo->IsLinked()) [[unlikely]] {
			return REX::Format(R"(failed to find old script "{}")"sv,
				oldScriptName);
		}

		auto* oldFunctionInfo = oldTypeInfo->GetFunctionInfoByName(oldFunctionName, oldStateName);
		if (!oldFunctionInfo) [[unlikely]] {
			return REX::Format(R"(failed to find old function "{}.{}" in state "{}")"sv,
				oldScriptName, oldFunctionName, oldStateName);
		}

		const auto newScriptName = RE::BSFixedString(a_replacedFunctions->newScriptName);
		const auto newFunctionName = RE::BSFixedString(a_replacedFunctions->newFunctionName);
		const auto newStateName = a_replacedFunctions->newStateName.has_value() ?
									  RE::BSFixedString(a_replacedFunctions->newStateName.value()) :
									  RE::BSScript::ObjectTypeInfo::GetEmptyStateName();

		auto newTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_internalVM.GetScriptObjectType(newScriptName, newTypeInfo) || !newTypeInfo || !newTypeInfo->IsLinked()) [[unlikely]] {
			return REX::Format(R"(failed to find new script "{}")"sv,
				newScriptName);
		}

		const auto* newFunctionInfo = newTypeInfo->GetFunctionInfoByName(newFunctionName, newStateName);
		if (!newFunctionInfo) [[unlikely]] {
			return REX::Format(R"(failed to find new function "{}.{}" in state "{}")"sv,
				newScriptName, newFunctionName, newStateName);
		}

		auto& oldFunctionPtr = oldFunctionInfo->func;
		const auto& newFunctionPtr = newFunctionInfo->func;

		auto customFunction = ReplaceFunction(oldFunctionPtr, newFunctionPtr);
		if (customFunction.has_value()) [[unlikely]] {
			return REX::Format(R"(failed to replace function "{}.{}" with "{}.{}"; {})"sv,
				oldFunctionPtr->GetObjectTypeName(), oldFunctionPtr->GetName(),
				newFunctionPtr->GetObjectTypeName(), newFunctionPtr->GetName(),
				customFunction.value());
		}

		return std::nullopt;
	}

	void ScriptReflectionRunner::RunReplacedFunctions(
		RE::BSScript::Internal::VirtualMachine& a_internalVM,
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<ReplacedFunctionInfo>>> a_replacedFunctions)
	{
		for (const auto& replacedFunctionInfo : a_replacedFunctions) {
			auto errorMessage = RunReplacedFunction(a_internalVM, replacedFunctionInfo);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Replaced function "{}.{}" with "{}.{}" from file "{}")"sv,
				replacedFunctionInfo->oldScriptName, replacedFunctionInfo->oldFunctionName,
				replacedFunctionInfo->newScriptName, replacedFunctionInfo->newFunctionName,
				a_filePath.generic_string());
		}
	}

	auto ScriptReflectionRunner::RunReplacedGetterFunction(
		RE::BSScript::Internal::VirtualMachine& a_internalVM,
		const REX::NotNull<std::shared_ptr<ReplacedGetterFunctionInfo>>& a_replacedGetterFunction)
		-> std::optional<std::string>
	{
		const auto oldScriptName = RE::BSFixedString(a_replacedGetterFunction->oldScriptName);
		const auto oldPropertyName = RE::BSFixedString(a_replacedGetterFunction->oldPropertyName);

		const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);

		auto oldTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_internalVM.GetScriptObjectType(oldScriptName, oldTypeInfo) || !oldTypeInfo || !oldTypeInfo->IsLinked()) [[unlikely]] {
			return REX::Format(R"(failed to find old script "{}")"sv,
				oldScriptName);
		}

		auto* oldPropertyInfo = oldTypeInfo->GetPropertyInfoByName(oldPropertyName);
		if (!oldPropertyInfo) [[unlikely]] {
			return REX::Format(R"(failed to find old getter property "{}.{}")"sv,
				oldScriptName, oldPropertyName);
		}

		if (oldPropertyInfo->info.autoVarIndex != std::numeric_limits<std::uint32_t>::max()) [[unlikely]] {
			return REX::Format(R"(getter property "{}.{}" is an auto variable)"sv,
				oldScriptName, oldPropertyName);
		}

		const auto newScriptName = RE::BSFixedString(a_replacedGetterFunction->newScriptName);
		const auto newPropertyName = RE::BSFixedString(a_replacedGetterFunction->newPropertyName);

		auto newTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_internalVM.GetScriptObjectType(newScriptName, newTypeInfo) || !newTypeInfo || !newTypeInfo->IsLinked()) [[unlikely]] {
			return REX::Format(R"(failed to find new script "{}")"sv,
				newScriptName);
		}

		const auto* newPropertyInfo = newTypeInfo->GetPropertyInfoByName(newPropertyName);
		if (!newPropertyInfo) [[unlikely]] {
			return REX::Format(R"(failed to find new getter property "{}.{}")"sv,
				newScriptName, newPropertyName);
		}

		if (newPropertyInfo->info.autoVarIndex != std::numeric_limits<std::uint32_t>::max()) [[unlikely]] {
			return REX::Format(R"(getter property "{}.{}" is an auto variable)"sv,
				newScriptName, newPropertyName);
		}

		auto& oldFunctionPtr = oldPropertyInfo->info.getFunction;
		const auto& newFunctionPtr = newPropertyInfo->info.getFunction;

		auto errorMessage = ReplaceFunction(oldFunctionPtr, newFunctionPtr);
		if (errorMessage.has_value()) [[unlikely]] {
			return REX::Format(R"(failed to replace getter property "{}.{}" with "{}.{}"; {})"sv,
				oldFunctionPtr->GetObjectTypeName(), oldPropertyName,
				newFunctionPtr->GetObjectTypeName(), newPropertyName,
				errorMessage.value());
		}

		return std::nullopt;
	}

	void ScriptReflectionRunner::RunReplacedGetterFunctions(
		RE::BSScript::Internal::VirtualMachine& a_internalVM,
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<ReplacedGetterFunctionInfo>>> a_replacedGetterFunctions)
	{
		for (const auto& replacedFunctionInfo : a_replacedGetterFunctions) {
			auto errorMessage = RunReplacedGetterFunction(a_internalVM, replacedFunctionInfo);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Replaced getter function "{}.{}" with "{}.{}" from file "{}")"sv,
				replacedFunctionInfo->oldScriptName, replacedFunctionInfo->oldPropertyName,
				replacedFunctionInfo->newScriptName, replacedFunctionInfo->newPropertyName,
				a_filePath.generic_string());
		}
	}

	auto ScriptReflectionRunner::RunReplacedSetterFunction(
		RE::BSScript::Internal::VirtualMachine& a_internalVM,
		const REX::NotNull<std::shared_ptr<ReplacedSetterFunctionInfo>>& a_replacedSetterFunction)
		-> std::optional<std::string>
	{
		const auto oldScriptName = RE::BSFixedString(a_replacedSetterFunction->oldScriptName);
		const auto oldPropertyName = RE::BSFixedString(a_replacedSetterFunction->oldPropertyName);

		const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);

		auto oldTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_internalVM.GetScriptObjectType(oldScriptName, oldTypeInfo) || !oldTypeInfo || !oldTypeInfo->IsLinked()) [[unlikely]] {
			return REX::Format(R"(failed to find old script "{}")"sv,
				oldScriptName);
		}

		auto* oldPropertyInfo = oldTypeInfo->GetPropertyInfoByName(oldPropertyName);
		if (!oldPropertyInfo) [[unlikely]] {
			return REX::Format(R"(failed to find old setter property "{}.{}")"sv,
				oldScriptName, oldPropertyName);
		}

		if (oldPropertyInfo->info.autoVarIndex != std::numeric_limits<std::uint32_t>::max()) [[unlikely]] {
			return REX::Format(R"(setter property "{}.{}" is an auto variable)"sv,
				oldScriptName, oldPropertyName);
		}

		const auto newScriptName = RE::BSFixedString(a_replacedSetterFunction->newScriptName);
		const auto newPropertyName = RE::BSFixedString(a_replacedSetterFunction->newPropertyName);

		auto newTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_internalVM.GetScriptObjectType(newScriptName, newTypeInfo) || !newTypeInfo || !newTypeInfo->IsLinked()) [[unlikely]] {
			return REX::Format(R"(failed to find new script "{}")"sv,
				newScriptName);
		}

		const auto* newPropertyInfo = newTypeInfo->GetPropertyInfoByName(newPropertyName);
		if (!newPropertyInfo) [[unlikely]] {
			return REX::Format(R"(failed to find new setter property "{}.{}")"sv,
				newScriptName, newPropertyName);
		}

		if (newPropertyInfo->info.autoVarIndex != std::numeric_limits<std::uint32_t>::max()) [[unlikely]] {
			return REX::Format(R"(setter property "{}.{}" is an auto variable)"sv,
				newScriptName, newPropertyName);
		}

		auto& oldFunctionPtr = oldPropertyInfo->info.setFunction;
		const auto& newFunctionPtr = newPropertyInfo->info.setFunction;

		auto errorMessage = ReplaceFunction(oldFunctionPtr, newFunctionPtr);
		if (errorMessage.has_value()) [[unlikely]] {
			return REX::Format(R"(failed to replace setter function "{}.{}" with "{}.{}"; {})"sv,
				oldFunctionPtr->GetObjectTypeName(), oldPropertyName,
				newFunctionPtr->GetObjectTypeName(), newPropertyName,
				errorMessage.value());
		}

		return std::nullopt;
	}

	void ScriptReflectionRunner::RunReplacedSetterFunctions(
		RE::BSScript::Internal::VirtualMachine& a_internalVM,
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<ReplacedSetterFunctionInfo>>> a_replacedSetterFunctions)
	{
		for (const auto& replacedFunctionInfo : a_replacedSetterFunctions) {
			auto errorMessage = RunReplacedSetterFunction(a_internalVM, replacedFunctionInfo);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Replaced setter function "{}.{}" with "{}.{}" from file "{}")"sv,
				replacedFunctionInfo->oldScriptName, replacedFunctionInfo->oldPropertyName,
				replacedFunctionInfo->newScriptName, replacedFunctionInfo->newPropertyName,
				a_filePath.generic_string());
		}
	}
}
