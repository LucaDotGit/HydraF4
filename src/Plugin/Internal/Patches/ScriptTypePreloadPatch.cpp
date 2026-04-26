#include "Plugin/Internal/Patches/ScriptTypePreloadPatch.hpp"

namespace Plugin::Internal::Patches::ScriptTypePreloadPatch::Impl
{
	[[nodiscard]] static bool LoadAndLinkScriptType(RE::BSScript::IVirtualMachine& a_scriptVM, const RE::BSFixedString& a_typeName)
	{
		auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (a_scriptVM.GetScriptObjectTypeNoLoad(a_typeName, objectTypeInfo) && objectTypeInfo && objectTypeInfo->IsLinked()) {
			return true;
		}

		objectTypeInfo.reset();
		return a_scriptVM.GetScriptObjectType(a_typeName, objectTypeInfo) && objectTypeInfo && objectTypeInfo->IsLinkedValid();
	}

	[[nodiscard]] static bool LoadAndLinkStructType(RE::BSScript::IVirtualMachine& a_scriptVM, const RE::BSFixedString& a_typeName)
	{
		auto structTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (a_scriptVM.GetScriptStructTypeNoLoad(a_typeName, structTypeInfo) && structTypeInfo && structTypeInfo->IsLinked()) {
			return true;
		}

		structTypeInfo.reset();
		return a_scriptVM.GetScriptStructType(a_typeName, structTypeInfo) && structTypeInfo && structTypeInfo->IsLinkedValid();
	}

	static void LoadAllScriptTypes(RE::BSScript::Internal::VirtualMachine& a_internalVM)
	{
		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
			const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);
			const auto& typeInfoMap = a_internalVM.objectTypeMap;

			for (const auto& [_, typeInfo] : typeInfoMap) {
				if (!typeInfo || typeInfo->IsLinked()) {
					continue;
				}

				const auto& typeName = typeInfo->GetName();
				if (!Impl::LoadAndLinkScriptType(a_internalVM, typeName)) [[unlikely]] {
					REX::LogWarning(R"(Failed to link script type "{}")"sv,
						typeName);
					continue;
				}

				totalCount++;

				REX::LogTrace(R"(Linked script type "{}")"sv,
					typeName);
			}
		}

		REX::LogDebug(R"(Loaded {} script types in {})"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}

	static void LoadAllStructTypes(RE::BSScript::Internal::VirtualMachine& a_internalVM)
	{
		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
			const auto typeInfoLock = RE::BSAutoLock(a_internalVM.typeInfoLock);
			const auto& typeInfoMap = a_internalVM.objectTypeMap;

			for (const auto& [_, scriptTypeInfo] : typeInfoMap) {
				if (!scriptTypeInfo || !scriptTypeInfo->IsLinked()) {
					continue;
				}

				auto structTypeInfoNames = RE::BSTScrapObjectArena<RE::BSFixedString, 32>();
				if (!a_internalVM.GetChildStructTypes(scriptTypeInfo->GetName(), structTypeInfoNames)) {
					continue;
				}

				for (const auto& structTypeInfoName : structTypeInfoNames) {
					if (!Impl::LoadAndLinkStructType(a_internalVM, structTypeInfoName)) [[unlikely]] {
						REX::LogWarning(R"(Failed to link struct type "{}")"sv,
							structTypeInfoName);
						continue;
					}

					totalCount++;

					REX::LogTrace(R"(Linked struct type "{}")"sv,
						structTypeInfoName);
				}
			}
		}

		stopwatch.Stop();

		REX::LogDebug(R"(Loaded {} struct types in {})"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}
}

namespace Plugin::Internal::Patches::ScriptTypePreloadPatch
{
	void OnGameDataReady()
	{
		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		Impl::LoadAllScriptTypes(*internalVM);
		Impl::LoadAllStructTypes(*internalVM);
	}
}
