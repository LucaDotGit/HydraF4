#include "Plugin/Internal/Script/ScriptDumper.hpp"

#include "Plugin/Internal/Patches/ScriptFunctionTaskletPatch.hpp"
#include "Plugin/Internal/PatchesManager.hpp"
#include "Plugin/Internal/SettingsManager.hpp"

namespace Plugin::Internal::Script::Impl
{
	[[nodiscard]] static bool ContainsDebugFunction(std::span<const RE::BSScript::ObjectTypeInfo::FunctionInfo> a_functions)
	{
		return std::ranges::any_of(a_functions, [](const RE::BSScript::ObjectTypeInfo::FunctionInfo& functionInfo) {
			const auto& functionPtr = functionInfo.func;
			if (!functionPtr) {
				return false;
			}

			const auto byteCodeFunction = RE::dynamic_pointer_cast<RE::BSScript::Internal::ScriptFunction>(functionPtr);
			if (!byteCodeFunction) {
				return false;
			}

			return byteCodeFunction->lineNumberCount > 0;
		});
	}

	[[nodiscard]] static bool DumpDebugScript(const RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>& a_typeInfo)
	{
		if (!a_typeInfo || !a_typeInfo->IsLinked()) {
			return false;
		}

		if (!ContainsDebugFunction(a_typeInfo->GetThisMemberFunctions()) &&
			!ContainsDebugFunction(a_typeInfo->GetThisStaticFunctions())) {
			return false;
		}

		REX::LogWarning(R"(Found debug script "{}")"sv,
			a_typeInfo->GetName());
		return true;
	}

	[[nodiscard]] static bool DumpInvalidLinkedScript(const RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>& a_typeInfo)
	{
		if (!a_typeInfo || !a_typeInfo->IsLinked() || a_typeInfo->IsLinkedValid()) {
			return false;
		}

		REX::LogWarning(R"(Found invalid linked script "{}")"sv,
			a_typeInfo->GetName());
		return true;
	}

	[[nodiscard]] static bool DumpInvalidLinkedStruct(const RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>& a_typeInfo)
	{
		if (!a_typeInfo || !a_typeInfo->IsLinked() || a_typeInfo->IsLinkedValid()) {
			return false;
		}

		REX::LogWarning(R"(Found invalid linked struct "{}")"sv,
			a_typeInfo->GetName());
		return true;
	}

	[[nodiscard]] static std::uint32_t DumpDelayedFunctions(std::span<const RE::BSScript::ObjectTypeInfo::FunctionInfo> a_functions)
	{
		auto totalCount = 0ui32;

		for (const auto& functionInfo : a_functions) {
			const auto functionPtr = functionInfo.func;
			if (!functionPtr || !functionPtr->GetIsNative()) {
				continue;
			}

			const auto nativeFunctionPtr = RE::reinterpret_pointer_cast<RE::BSScript::NF_util::NativeFunctionBase>(functionPtr);
			if (!nativeFunctionPtr) {
				continue;
			}

			const auto& scriptName = functionPtr->GetObjectTypeName();
			if (scriptName.empty()) {
				continue;
			}

			const auto& functionName = functionPtr->GetName();
			if (functionName.empty()) {
				continue;
			}

			if (nativeFunctionPtr->CanBeCalledFromTasklets()) {
				continue;
			}

			REX::LogTrace(R"(Found delayed function "{}.{}")"sv,
				scriptName, functionName);

			totalCount++;
		}

		return totalCount;
	}

	[[nodiscard]] static std::uint32_t DumpDelayedScriptFunctions(const RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>& a_typeInfo)
	{
		if (!a_typeInfo || !a_typeInfo->IsLinked()) {
			return 0;
		}

		auto totalCount = 0ui32;

		totalCount += DumpDelayedFunctions(a_typeInfo->GetThisMemberFunctions());
		totalCount += DumpDelayedFunctions(a_typeInfo->GetThisStaticFunctions());

		return totalCount;
	}

	[[nodiscard]] static std::uint32_t DumpLatentFunctions(std::span<const RE::BSScript::ObjectTypeInfo::FunctionInfo> a_functions)
	{
		auto totalCount = 0ui32;

		for (const auto& functionInfo : a_functions) {
			const auto functionPtr = functionInfo.func;
			if (!functionPtr || !functionPtr->GetIsNative()) {
				continue;
			}

			const auto nativeFunctionPtr = RE::reinterpret_pointer_cast<RE::BSScript::NF_util::NativeFunctionBase>(functionPtr);
			if (!nativeFunctionPtr || !nativeFunctionPtr->GetIsLatent()) {
				continue;
			}

			const auto& scriptName = functionPtr->GetObjectTypeName();
			if (scriptName.empty()) {
				continue;
			}

			const auto& functionName = functionPtr->GetName();
			if (functionName.empty()) {
				continue;
			}

			REX::LogTrace(R"(Found latent function "{}.{}")"sv,
				scriptName, functionName);

			totalCount++;
		}

		return totalCount;
	}

	[[nodiscard]] static std::uint32_t DumpLatentScriptFunctions(const RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>& a_typeInfo)
	{
		if (!a_typeInfo || !a_typeInfo->IsLinked()) {
			return 0;
		}

		auto totalCount = 0ui32;

		totalCount += DumpLatentFunctions(a_typeInfo->GetThisMemberFunctions());
		totalCount += DumpLatentFunctions(a_typeInfo->GetThisStaticFunctions());

		return totalCount;
	}
}

namespace Plugin::Internal::Script
{
	ScriptDumper::ScriptDumper(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager)
		: _settingsManager(a_settingsManager)
	{
	}

	ScriptDumper::~ScriptDumper() noexcept = default;

	void ScriptDumper::OnGameDataReady()
	{
		auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		DumpAllDebugScripts(*internalVM);
		DumpAllInvalidLinkedScripts(*internalVM);
		DumpAllInvalidLinkedStructs(*internalVM);
		DumpAllDelayedFunctions(*internalVM);
		DumpAllLatentFunctions(*internalVM);
	}

	void ScriptDumper::DumpAllDebugScripts(RE::BSScript::Internal::VirtualMachine& a_internalVM)
	{
		if (!_settingsManager->GetLogDebugPapyrusScriptsSetting()->GetValue()) {
			return;
		}

		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
			const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);
			const auto& typeInfoMap = a_internalVM.objectTypeMap;

			for (const auto& [typeName, typeInfo] : typeInfoMap) {
				if (Impl::DumpDebugScript(typeInfo)) {
					totalCount++;
				}
			}
		}

		stopwatch.Stop();

		REX::LogDebug("Found {} debug scripts in {}"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}

	void ScriptDumper::DumpAllInvalidLinkedScripts(RE::BSScript::Internal::VirtualMachine& a_internalVM)
	{
		if (!_settingsManager->GetLogInvalidLinkedPapyrusScriptsSetting()->GetValue()) {
			return;
		}

		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
			const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);
			const auto& typeInfoMap = a_internalVM.objectTypeMap;

			for (const auto& [typeName, typeInfo] : typeInfoMap) {
				if (Impl::DumpInvalidLinkedScript(typeInfo)) {
					totalCount++;
				}
			}
		}

		stopwatch.Stop();

		REX::LogDebug("Found {} invalid linked scripts in {}"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}

	void ScriptDumper::DumpAllInvalidLinkedStructs(RE::BSScript::Internal::VirtualMachine& a_internalVM)
	{
		if (!_settingsManager->GetLogInvalidLinkedPapyrusStructsSetting()->GetValue()) {
			return;
		}

		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
			const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);
			const auto& structTypeInfoMap = a_internalVM.structTypeMap;

			for (const auto& [typeName, typeInfo] : structTypeInfoMap) {
				if (Impl::DumpInvalidLinkedStruct(typeInfo)) {
					totalCount++;
				}
			}
		}

		stopwatch.Stop();

		REX::LogDebug("Found {} invalid linked structs in {}"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}

	void ScriptDumper::DumpAllDelayedFunctions(RE::BSScript::Internal::VirtualMachine& a_internalVM)
	{
		if (!_settingsManager->GetLogDelayedPapyrusFunctionsSetting()->GetValue()) {
			return;
		}

		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
			const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);
			const auto& typeInfoMap = a_internalVM.objectTypeMap;

			for (const auto& [typeName, typeInfo] : typeInfoMap) {
				totalCount += Impl::DumpDelayedScriptFunctions(typeInfo);
			}
		}

		stopwatch.Stop();

		REX::LogDebug("Found {} delayed functions in {}"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}

	void ScriptDumper::DumpAllLatentFunctions(RE::BSScript::Internal::VirtualMachine& a_internalVM)
	{
		if (!_settingsManager->GetLogLatentPapyrusFunctionsSetting()->GetValue()) {
			return;
		}

		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
			const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);
			const auto& typeInfoMap = a_internalVM.objectTypeMap;

			for (const auto& [typeName, typeInfo] : typeInfoMap) {
				totalCount += Impl::DumpLatentScriptFunctions(typeInfo);
			}
		}

		stopwatch.Stop();

		REX::LogDebug("Found {} latent functions in {}"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}
}
