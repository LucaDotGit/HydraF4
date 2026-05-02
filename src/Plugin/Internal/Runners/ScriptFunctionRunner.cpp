#include "Plugin/Internal/Runners/ScriptFunctionRunner.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp" // IWYU pragma: keep
#include "Plugin/Internal/Core/TypeSerializer.hpp"
#include "Plugin/Internal/Events/EventFunctorMap.hpp"
#include "Plugin/Internal/Save/SaveScriptFunctionMap.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

namespace Plugin::Internal::Runners
{
	static constexpr auto LOCAL_FUNCTIONS_KEY = "localFunctions"sv;
	static constexpr auto GLOBAL_FUNCTIONS_KEY = "globalFunctions"sv;
	static constexpr auto EVENT_NAME_KEY = "eventName"sv;
	static constexpr auto BOUND_OBJECT_KEY = "boundObject"sv;
	static constexpr auto SCRIPT_NAME_KEY = "scriptName"sv;
	static constexpr auto FUNCTION_NAME_KEY = "functionName"sv;
	static constexpr auto EVENT_ARGS_KEY = "eventArgs"sv;

	auto ScriptFunctionRunner::LocalFunctionInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<LocalFunctionInfo, std::string>
	{
		const auto eventNameIt = a_json.find(EVENT_NAME_KEY);
		if (eventNameIt == a_json.end() || !eventNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				EVENT_NAME_KEY);
		}

		const auto boundObjectIt = a_json.find(BOUND_OBJECT_KEY);
		if (boundObjectIt == a_json.end() || !boundObjectIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				BOUND_OBJECT_KEY);
		}

		const auto functionNameIt = a_json.find(FUNCTION_NAME_KEY);
		if (functionNameIt == a_json.end() || !functionNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				FUNCTION_NAME_KEY);
		}

		const auto eventArgsIt = a_json.find(EVENT_ARGS_KEY);
		if (eventArgsIt != a_json.end() && !eventArgsIt->is_object()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find object for key "{}")"sv,
				EVENT_ARGS_KEY);
		}

		auto functionInfo = LocalFunctionInfo();
		functionInfo.eventName = eventNameIt->get<std::string>();
		functionInfo.boundObject = boundObjectIt->get<std::string>();
		functionInfo.functionName = functionNameIt->get<std::string>();

		if (eventArgsIt != a_json.end()) {
			auto eventArgs = std::unordered_map<std::string, IFileRunner::json_t>();
			eventArgs.reserve(eventArgsIt->size());

			for (const auto& [nodeKey, nodeValue] : eventArgsIt->items()) {
				eventArgs.emplace(nodeKey, nodeValue);
			}

			functionInfo.eventArgs = std::move(eventArgs);
		}

		return functionInfo;
	}

	auto ScriptFunctionRunner::GlobalFunctionInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<GlobalFunctionInfo, std::string>
	{
		const auto eventNameIt = a_json.find(EVENT_NAME_KEY);
		if (eventNameIt == a_json.end() || !eventNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				EVENT_NAME_KEY);
		}

		const auto scriptNameIt = a_json.find(SCRIPT_NAME_KEY);
		if (scriptNameIt == a_json.end() || !scriptNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				SCRIPT_NAME_KEY);
		}

		const auto functionNameIt = a_json.find(FUNCTION_NAME_KEY);
		if (functionNameIt == a_json.end() || !functionNameIt->is_string()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find string value for key "{}")"sv,
				FUNCTION_NAME_KEY);
		}

		const auto eventArgsIt = a_json.find(EVENT_ARGS_KEY);
		if (eventArgsIt != a_json.end() && !eventArgsIt->is_object()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find object for key "{}")"sv,
				EVENT_ARGS_KEY);
		}

		auto functionInfo = GlobalFunctionInfo();
		functionInfo.eventName = eventNameIt->get<std::string>();
		functionInfo.scriptName = scriptNameIt->get<std::string>();
		functionInfo.functionName = functionNameIt->get<std::string>();

		if (eventArgsIt != a_json.end()) {
			auto eventArgs = std::unordered_map<std::string, IFileRunner::json_t>();
			eventArgs.reserve(eventArgsIt->size());

			for (const auto& [nodeKey, nodeValue] : eventArgsIt->items()) {
				eventArgs.emplace(nodeKey, nodeValue);
			}

			functionInfo.eventArgs = std::move(eventArgs);
		}

		return functionInfo;
	}

	auto ScriptFunctionRunner::RootInfo::FromJson(const IFileRunner::json_t& a_json)
		-> std::expected<RootInfo, std::string>
	{
		const auto localFunctionsIt = a_json.find(LOCAL_FUNCTIONS_KEY);
		if (localFunctionsIt != a_json.end() && !localFunctionsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				LOCAL_FUNCTIONS_KEY);
		}

		const auto globalFunctionsIt = a_json.find(GLOBAL_FUNCTIONS_KEY);
		if (globalFunctionsIt != a_json.end() && !globalFunctionsIt->is_array()) [[unlikely]] {
			return Core::StringUtility::FormatUnexpected(
				R"(failed to find array for key "{}")"sv,
				GLOBAL_FUNCTIONS_KEY);
		}

		auto rootInfo = RootInfo();

		if (localFunctionsIt != a_json.end()) {
			auto localFunctions = std::vector<REX::NotNull<std::shared_ptr<LocalFunctionInfo>>>();
			localFunctions.reserve(localFunctionsIt->size());

			for (auto functionIt = localFunctionsIt->begin(); functionIt != localFunctionsIt->end(); functionIt++) {
				if (!functionIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(localFunctionsIt->begin(), functionIt), LOCAL_FUNCTIONS_KEY);
				}

				auto function = LocalFunctionInfo::FromJson(*functionIt);
				if (!function.has_value()) {
					return std::unexpected(std::move(function).error());
				}

				localFunctions.emplace_back(std::make_shared<LocalFunctionInfo>(std::move(function).value()));
			}

			rootInfo.localFunctions = std::move(localFunctions);
		}

		if (globalFunctionsIt != a_json.end()) {
			auto globalFunctions = std::vector<REX::NotNull<std::shared_ptr<GlobalFunctionInfo>>>();
			globalFunctions.reserve(globalFunctionsIt->size());

			for (auto functionIt = globalFunctionsIt->begin(); functionIt != globalFunctionsIt->end(); functionIt++) {
				if (!functionIt->is_object()) [[unlikely]] {
					return Core::StringUtility::FormatUnexpected(
						R"(failed to find object element at index {} in array for key "{}")"sv,
						std::distance(globalFunctionsIt->begin(), functionIt), GLOBAL_FUNCTIONS_KEY);
				}

				auto function = GlobalFunctionInfo::FromJson(*functionIt);
				if (!function.has_value()) {
					return std::unexpected(std::move(std::move(function).error()));
				}

				globalFunctions.emplace_back(std::make_shared<GlobalFunctionInfo>(std::move(function).value()));
			}

			rootInfo.globalFunctions = std::move(globalFunctions);
		}

		return rootInfo;
	}

	ScriptFunctionRunner::ScriptFunctionRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension,
		const REX::NotNull<std::shared_ptr<Save::SaveScriptFunctionMap>>& a_saveScriptFunctionMap)
		: IFileRunner(std::move(a_directoryPath), std::move(a_fileExtension)),
		  _saveScriptFunctionMap(a_saveScriptFunctionMap)
	{
	}

	ScriptFunctionRunner::~ScriptFunctionRunner() noexcept
	{
		Dispose();
	}

	auto ScriptFunctionRunner::Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
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

			const auto& localFunctions = rootInfo->localFunctions;
			const auto& globalFunctions = rootInfo->globalFunctions;

			REX::LogDebug(R"(Loaded file "{}" with {} local and {} global function(s))"sv,
				a_filePath.generic_string(),
				localFunctions.has_value() ? localFunctions->size() : 0,
				globalFunctions.has_value() ? globalFunctions->size() : 0);

			const auto runnerLock = std::scoped_lock(_mapMutex);

			_rootMap.emplace(a_filePath, std::make_shared<RootInfo>(std::move(rootInfo).value()));
		});
	}

	auto ScriptFunctionRunner::Run([[maybe_unused]] REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>>
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Fail("Failed to get the game's script VM."sv);
		}

		const auto runnerLock = std::scoped_lock(_mapMutex);

		for (const auto& [filePath, rootInfo] : _rootMap) {
			const auto& localFunctions = rootInfo->localFunctions;
			const auto& globalFunctions = rootInfo->globalFunctions;

			if (localFunctions.has_value()) {
				RunLocalFunctions(scriptVM, filePath, *localFunctions);
			}

			if (globalFunctions.has_value()) {
				RunGlobalFunctions(scriptVM, filePath, *globalFunctions);
			}
		}

		return {};
	}

	void ScriptFunctionRunner::Dispose() noexcept
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto runnerLock = std::scoped_lock(_mapMutex);

		_rootMap.clear();
	}

	auto ScriptFunctionRunner::RunLocalFunction(
		const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
		const REX::NotNull<std::shared_ptr<LocalFunctionInfo>>& a_function)
		-> std::optional<std::string>
	{
		const auto& functionEventName = a_function->eventName;

		const auto eventFunctorData = Events::GetEventFunctorMap()->GetValue(functionEventName);
		if (!eventFunctorData) [[unlikely]] {
			return REX::Format(R"(invalid event "{}")"sv,
				functionEventName);
		}

		const auto& functionBoundObject = a_function->boundObject;

		const auto functionObjectData = Core::TypeSerializer::DeserializeObjectData(functionBoundObject);
		if (!functionObjectData.has_value()) [[unlikely]] {
			return REX::Format(R"(failed to deserialize bound object "{}")"sv,
				functionBoundObject);
		}

		const auto functionObjectHandle = functionObjectData->objectHandle;
		if (functionObjectHandle == RE::GameScript::HandlePolicy::EMPTY_HANDLE) [[unlikely]] {
			return REX::Format(R"(bound object "{}" has empty handle)"sv,
				functionBoundObject);
		}

		const auto& handlePolicy = a_scriptVM->GetObjectHandlePolicy();
		if (!handlePolicy.IsHandleObjectAvailable(functionObjectHandle)) [[unlikely]] {
			return REX::Format(R"(bound object "{}" with handle "{:X}" is not available)"sv,
				functionBoundObject, functionObjectHandle);
		}

		const auto paramsStructName = std::invoke(*(eventFunctorData->GetEventParamsNameCallback()));
		const auto argsStructName = std::invoke(*(eventFunctorData->GetEventArgsNameCallback()));

		auto paramTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_scriptVM->GetScriptStructType(RE::BSFixedString(paramsStructName), paramTypeInfo) || !paramTypeInfo) [[unlikely]] {
			return REX::Format(R"(failed to get type info for event parameter struct "{}")"sv,
				paramsStructName);
		}

		auto argsTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_scriptVM->GetScriptStructType(RE::BSFixedString(argsStructName), argsTypeInfo) || !argsTypeInfo) [[unlikely]] {
			return REX::Format(R"(failed to get type info for event argument struct "{}")"sv,
				argsStructName);
		}

		const auto scriptName = RE::BSFixedString(std::string(functionObjectData->scriptName));
		const auto functionName = RE::BSFixedString(a_function->functionName);

		const auto paramTypeInfoArray = std::array{
			RE::BSScript::TypeInfo(paramTypeInfo.get())
		};

		auto functionRef = Script::ScriptFunctionRef(functionObjectHandle, scriptName, functionName);
		if (!functionRef.IsFunctionOfParamTypes(paramTypeInfoArray)) [[unlikely]] {
			return REX::Format(R"(script "{}" with function "{}" does not exist or has not the struct "{}" as its only parameter)"sv,
				scriptName, functionName, paramsStructName);
		}

		const auto& functionEventArgs = a_function->eventArgs;

		auto eventArgs = std::unordered_map<RE::BSFixedString, IFileRunner::json_t>();
		if (functionEventArgs.has_value()) {
			for (const auto& [argName, argValue] : functionEventArgs.value()) {
				const auto nativeArgName = RE::BSFixedString(argName);

				const auto argVarIndex = argsTypeInfo->GetVariableIndex(nativeArgName);
				if (!argVarIndex.has_value()) {
					continue;
				}

				eventArgs.emplace(nativeArgName, argValue);
			}
		}

		auto eventInfo = Save::SaveScriptFunctionMap::EventInfo{
			.eventName = RE::BSFixedString(functionEventName),
			.functionRef = std::move(functionRef),
			.eventArgs = std::move(eventArgs)
		};

		_saveScriptFunctionMap->AddValue(std::make_shared<Save::SaveScriptFunctionMap::EventInfo>(std::move(eventInfo)));
		return std::nullopt;
	}

	void ScriptFunctionRunner::RunLocalFunctions(
		const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<LocalFunctionInfo>>> a_functions)
	{
		for (const auto& function : a_functions) {
			const auto errorMessage = RunLocalFunction(a_scriptVM, function);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Registered local function "{}" on bound object "{}" with event "{}" from file "{}")"sv,
				function->functionName, function->boundObject, function->eventName, a_filePath.generic_string());
		}
	}

	auto ScriptFunctionRunner::RunGlobalFunction(
		const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
		const REX::NotNull<std::shared_ptr<GlobalFunctionInfo>>& a_function)
		-> std::optional<std::string>
	{
		const auto& functionEventName = a_function->eventName;

		const auto eventFunctorData = Events::GetEventFunctorMap()->GetValue(functionEventName);
		if (!eventFunctorData) [[unlikely]] {
			return REX::Format(R"(invalid event "{}")"sv,
				functionEventName);
		}

		const auto paramsStructName = std::invoke(*(eventFunctorData->GetEventParamsNameCallback()));
		const auto argsStructName = std::invoke(*(eventFunctorData->GetEventArgsNameCallback()));

		auto paramTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_scriptVM->GetScriptStructType(RE::BSFixedString(paramsStructName), paramTypeInfo) || !paramTypeInfo) [[unlikely]] {
			return REX::Format(R"(failed to get type info for event parameter struct "{}")"sv,
				paramsStructName);
		}

		auto argsTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_scriptVM->GetScriptStructType(RE::BSFixedString(argsStructName), argsTypeInfo) || !argsTypeInfo) [[unlikely]] {
			return REX::Format(R"(failed to get type info for event argument struct "{}")"sv,
				argsStructName);
		}

		const auto scriptName = RE::BSFixedString(a_function->scriptName);
		const auto functionName = RE::BSFixedString(a_function->functionName);

		const auto paramTypeInfoArray = std::array{
			RE::BSScript::TypeInfo(paramTypeInfo.get())
		};

		auto functionRef = Script::ScriptFunctionRef(RE::GameScript::HandlePolicy::EMPTY_HANDLE, scriptName, functionName);
		if (!functionRef.IsFunctionOfParamTypes(paramTypeInfoArray)) [[unlikely]] {
			return REX::Format(R"(script "{}" with function "{}" does not exist or has not the struct "{}" as its only parameter)"sv,
				scriptName, functionName, paramsStructName);
		}

		const auto& functionEventArgs = a_function->eventArgs;

		auto eventArgs = std::unordered_map<RE::BSFixedString, IFileRunner::json_t>();
		if (functionEventArgs.has_value()) {
			for (const auto& [argName, argValue] : functionEventArgs.value()) {
				const auto nativeArgName = RE::BSFixedString(argName);

				const auto argVarIndex = argsTypeInfo->GetVariableIndex(nativeArgName);
				if (!argVarIndex.has_value()) {
					continue;
				}

				eventArgs.emplace(nativeArgName, argValue);
			}
		}

		auto eventInfo = Save::SaveScriptFunctionMap::EventInfo{
			.eventName = RE::BSFixedString(functionEventName),
			.functionRef = std::move(functionRef),
			.eventArgs = std::move(eventArgs)
		};

		_saveScriptFunctionMap->AddValue(std::make_shared<Save::SaveScriptFunctionMap::EventInfo>(std::move(eventInfo)));
		return std::nullopt;
	}

	void ScriptFunctionRunner::RunGlobalFunctions(
		const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
		const std::filesystem::path& a_filePath,
		std::span<const REX::NotNull<std::shared_ptr<GlobalFunctionInfo>>> a_functions)
	{
		for (const auto& function : a_functions) {
			const auto errorMessage = RunGlobalFunction(a_scriptVM, function);
			if (errorMessage.has_value()) [[unlikely]] {
				REX::LogError(R"(Error in file "{}"; {})"sv,
					a_filePath.generic_string(), errorMessage.value());
				continue;
			}

			REX::LogTrace(R"(Registered global function "{}.{}" with event "{}" from file "{}")"sv,
				function->scriptName, function->functionName, function->eventName, a_filePath.generic_string());
		}
	}
}
