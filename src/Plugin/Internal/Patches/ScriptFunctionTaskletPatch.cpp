#include "Plugin/Internal/Patches/ScriptFunctionTaskletPatch.hpp"

#include "Plugin/Internal/SettingsManager.hpp"

namespace Plugin::Internal::Patches::Impl
{
	static auto SequentialFunctionMap = REX::NotAssignable<std::unordered_map<RE::BSFixedString, std::unordered_set<RE::BSFixedString>>>();

	[[nodiscard]] static bool IsSequentialFunction(const RE::BSFixedString& a_scriptName, const RE::BSFixedString& a_functionName) noexcept
	{
		const auto scriptIt = SequentialFunctionMap->find(a_scriptName);
		if (scriptIt == SequentialFunctionMap->end()) {
			return false;
		}

		const auto& functionSet = scriptIt->second;
		return functionSet.contains(a_functionName);
	}

	static bool AddSequentialFunction(const RE::BSFixedString& a_scriptName, const RE::BSFixedString& a_functionName)
	{
		const auto&& [scriptIt, hasScriptInserted] = SequentialFunctionMap->try_emplace(a_scriptName);
		const auto&& [functionIt, hasFunctionInserted] = scriptIt->second.emplace(a_functionName);
		return hasFunctionInserted;
	}

	[[nodiscard]] static bool SequentializeScriptFunction(
		const SettingsManager& a_settingsManager,
		RE::BSScript::Internal::VirtualMachine& a_scriptVM,
		const RE::BSScript::IFunction& a_function)
	{
		if (!a_function.GetIsNative() || a_function.CanBeCalledFromTasklets()) {
			return false;
		}

		const auto& scriptName = a_function.GetObjectTypeName();
		if (scriptName.empty()) {
			return false;
		}

		const auto& functionName = a_function.GetName();
		if (functionName.empty()) {
			return false;
		}

		const auto doMatchPattern = [](std::string_view a_text, std::span<const std::string> a_patterns) -> std::optional<std::string_view> {
			for (const auto& pattern : a_patterns) {
				if (REX::MatchWildcardsIgnoreCase(a_text, static_cast<std::string_view>(pattern))) {
					return static_cast<std::string_view>(pattern);
				}
			}

			return std::nullopt;
		};

		const auto matchScriptNamePattern = doMatchPattern(scriptName,
			a_settingsManager.GetScriptTaskletPatch_IncludedScriptNamePatternsSetting()->GetValue());
		const auto matchFunctionNamePattern = doMatchPattern(functionName,
			a_settingsManager.GetScriptTaskletPatch_IncludedFunctionNamePatternsSetting()->GetValue());

		if (!matchScriptNamePattern.has_value() && !matchFunctionNamePattern.has_value()) {
			REX::LogTrace(R"(Skipping function "{}.{}" as it does not match any of the included script or function name patterns)"sv,
				scriptName, functionName);
			return false;
		}

		const auto matchExcludedScriptNamePattern = doMatchPattern(scriptName,
			a_settingsManager.GetScriptTaskletPatch_ExcludedScriptNamePatternsSetting()->GetValue());
		if (matchExcludedScriptNamePattern.has_value()) {
			REX::LogTrace(R"(Skipping function "{}.{}" as it matches an excluded script name pattern "{}")"sv,
				scriptName, functionName, matchExcludedScriptNamePattern.value());
			return false;
		}

		const auto matchesExcludedFunctionNamePattern = doMatchPattern(functionName,
			a_settingsManager.GetScriptTaskletPatch_ExcludedFunctionNamePatternsSetting()->GetValue());
		if (matchesExcludedFunctionNamePattern.has_value()) {
			REX::LogTrace(R"(Skipping function "{}.{}" as it matches an excluded function name pattern "{}")"sv,
				scriptName, functionName, matchesExcludedFunctionNamePattern.value());
			return false;
		}

		a_scriptVM.SetCallableFromTasklets(scriptName.data(), functionName.data(), true);

		if (!a_function.CanBeCalledFromTasklets()) [[unlikely]] {
			REX::LogWarning(R"(Failed to sequentialize function "{}.{}")"sv,
				scriptName, functionName);
			return false;
		}

		AddSequentialFunction(scriptName, functionName);

		REX::LogTrace(R"(Sequentialized function "{}.{}")"sv,
			scriptName, functionName);
		return true;
	}

	[[nodiscard]] static std::uint32_t SequentializeScriptFunctions(
		const SettingsManager& a_settingsManager,
		RE::BSScript::Internal::VirtualMachine& a_scriptVM,
		std::span<const RE::BSScript::ObjectTypeInfo::FunctionInfo> a_functions)
	{
		auto totalCount = 0ui32;

		for (const auto& functionInfo : a_functions) {
			const auto& func = functionInfo.func;
			if (!func) {
				continue;
			}

			if (SequentializeScriptFunction(a_settingsManager, a_scriptVM, *func)) {
				totalCount++;
			}
		}

		return totalCount;
	}

	static void SequentializeAllScriptFunctions(const SettingsManager& a_settingsManager)
	{
		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to the game's internal script VM."sv);
		}

		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
			const auto typeInfoLock = RE::BSAutoLock(internalVM->typeInfoLock);
			const auto& typeInfoMap = internalVM->objectTypeMap;

			for (const auto& [_, typeInfo] : typeInfoMap) {
				if (!typeInfo) {
					continue;
				}

				if (!typeInfo->IsLinked()) {
					REX::LogDebug(R"(Skipping unlinked script type "{}")"sv,
						typeInfo->GetName());
					continue;
				}

				if (!typeInfo->IsLinkedValid()) [[unlikely]] {
					REX::LogWarning(R"(Skipping invalid linked script type "{}")"sv,
						typeInfo->GetName());
					continue;
				}

				totalCount += Impl::SequentializeScriptFunctions(a_settingsManager, *internalVM, typeInfo->GetThisMemberFunctions());
				totalCount += Impl::SequentializeScriptFunctions(a_settingsManager, *internalVM, typeInfo->GetThisStaticFunctions());
			}
		}

		stopwatch.Stop();

		REX::LogDebug(R"(Sequentialized {} script functions in {})"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}

	class Hook final
	{
	public:
		Hook() = delete;

		static void Setup(REL::HookStore& a_hookStore)
		{
			constexpr const auto& RTTI_ID = RE::BSScript::NF_util::NativeFunctionBase::RTTI;

			const auto typeDescriptor = REL::Relocation<const RE::RTTI::TypeDescriptor*>{ RTTI_ID };
			REX::Ensure(static_cast<bool>(typeDescriptor));

			const auto typeDescriptorName = std::string_view(typeDescriptor->name());
			const auto typeFullName = REX::Format("{}::Call->MarshallAndDispatch"sv, typeDescriptorName);

			auto hookName01 = REX::Format("{}01"sv, typeFullName);
			auto hookName02 = REX::Format("{}02"sv, typeFullName);

			auto marshallAndDispatchHook01 = std::make_shared<decltype(MarshallAndDispatchHook01)::element_type>(
				std::move(hookName01), NATIVE_FUNCTION_BASE_CALL_FUNC_ADDRESS, MARCHALL_AND_DISPATCH_FUNC_OFFSET_01, Hook::MarshallAndDispatch01);
			auto marshallAndDispatchHook02 = std::make_shared<decltype(MarshallAndDispatchHook02)::element_type>(
				std::move(hookName02), NATIVE_FUNCTION_BASE_CALL_FUNC_ADDRESS, MARCHALL_AND_DISPATCH_FUNC_OFFSET_02, Hook::MarshallAndDispatch02);

			a_hookStore.Add(marshallAndDispatchHook01);
			a_hookStore.Add(marshallAndDispatchHook02);

			MarshallAndDispatchHook01 = std::move(marshallAndDispatchHook01);
			MarshallAndDispatchHook02 = std::move(marshallAndDispatchHook02);
		}

	private:
		[[nodiscard]] __forceinline static bool MarshallAndDispatchImpl(
			const RE::BSScript::NF_util::NativeFunctionBase* a_selfPtr,
			RE::BSScript::Variable& a_self,
			RE::BSScript::Internal::VirtualMachine& a_scriptVM,
			RE::BSScript::StackID a_stackId,
			RE::BSScript::Variable& a_outResult,
			const RE::BSScript::StackFrame& a_stackFrame)
		{
			if (!a_selfPtr) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			if (!Impl::IsSequentialFunction(a_selfPtr->GetObjectTypeName(), a_selfPtr->GetName())) {
				return a_selfPtr->MarshallAndDispatch(
					a_self,
					a_scriptVM,
					a_stackId,
					a_outResult,
					a_stackFrame);
			}

			const auto functionLock = std::scoped_lock(SequentialFunctionMutex.get());

			return a_selfPtr->MarshallAndDispatch(
				a_self,
				a_scriptVM,
				a_stackId,
				a_outResult,
				a_stackFrame);
		}

		static bool MarshallAndDispatch01(
			const RE::BSScript::NF_util::NativeFunctionBase* a_selfPtr,
			RE::BSScript::Variable& a_self,
			RE::BSScript::Internal::VirtualMachine& a_scriptVM,
			RE::BSScript::StackID a_stackId,
			RE::BSScript::Variable& a_outResult,
			const RE::BSScript::StackFrame& a_stackFrame)
		{
			return MarshallAndDispatchImpl(
				a_selfPtr,
				a_self,
				a_scriptVM,
				a_stackId,
				a_outResult,
				a_stackFrame);
		}

		static bool MarshallAndDispatch02(
			const RE::BSScript::NF_util::NativeFunctionBase* a_selfPtr,
			RE::BSScript::Variable& a_self,
			RE::BSScript::Internal::VirtualMachine& a_scriptVM,
			RE::BSScript::StackID a_stackId,
			RE::BSScript::Variable& a_outResult,
			const RE::BSScript::StackFrame& a_stackFrame)
		{
			return MarshallAndDispatchImpl(
				a_selfPtr,
				a_self,
				a_scriptVM,
				a_stackId,
				a_outResult,
				a_stackFrame);
		}

		inline static constexpr const auto& NATIVE_FUNCTION_BASE_CALL_FUNC_ADDRESS = RE::ID::BSScript::NF_util::NativeFunctionBase::Call;

		inline static constexpr auto MARCHALL_AND_DISPATCH_FUNC_OFFSET_01 = F4SE::CreateVariantOffset(0x211, 0x254, 0x254);
		inline static constexpr auto MARCHALL_AND_DISPATCH_FUNC_OFFSET_02 = F4SE::CreateVariantOffset(0x100, 0x0ED, 0x0ED);

		inline static constinit auto MarshallAndDispatchHook01 = std::shared_ptr<REL::HookCall6<decltype(&Hook::MarshallAndDispatch01)>>();
		inline static constinit auto MarshallAndDispatchHook02 = std::shared_ptr<REL::HookCall6<decltype(&Hook::MarshallAndDispatch02)>>();

		inline static constinit auto SequentialFunctionMutex = REX::NotAssignable<std::mutex>();
	};
}

namespace Plugin::Internal::Patches
{
	ScriptFunctionTaskletPatch::ScriptFunctionTaskletPatch(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: _settingsManager(a_settingsManager)
	{
	}

	ScriptFunctionTaskletPatch::~ScriptFunctionTaskletPatch() noexcept = default;

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void ScriptFunctionTaskletPatch::OnXseLoad(REL::HookStore& a_hookStore)
	{
		Impl::Hook::Setup(a_hookStore);
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void ScriptFunctionTaskletPatch::OnGameDataReady()
	{
		Impl::SequentializeAllScriptFunctions(*(_settingsManager.get()));
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	bool ScriptFunctionTaskletPatch::IsDelayedFunction(const RE::BSFixedString& a_scriptName, const RE::BSFixedString& a_functionName) const noexcept
	{
		return Impl::IsSequentialFunction(a_scriptName, a_functionName);
	}
}
