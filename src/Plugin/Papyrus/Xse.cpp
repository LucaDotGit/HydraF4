#include "Plugin/Papyrus/Xse.hpp"

#include "Plugin/Internal/Structs/Versions.hpp"

namespace Plugin::Papyrus::Xse
{
	using VersionStruct = ::Plugin::Internal::Structs::Versions::VersionData;

	static VersionStruct GetVersion(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetF4SEVersion();
	}

	static std::uint32_t GetVersionPack(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetF4SEVersion().Pack<std::uint32_t>();
	}

	static RE::BSFixedString GetVersionString(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto VERSION_STRING = RE::BSFixedString(F4SE::GetF4SEVersion().ToString<char>());
		return VERSION_STRING;
	}

	static std::uint16_t GetVersionMajor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetF4SEVersion().GetMajor();
	}

	static std::uint16_t GetVersionMinor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetF4SEVersion().GetMinor();
	}

	static std::uint16_t GetVersionPatch(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetF4SEVersion().GetPatch();
	}

	static std::uint16_t GetVersionBuild(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetF4SEVersion().GetBuild();
	}

	static bool IsPluginLoaded(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::zstring_view a_pluginName)
	{
		return F4SE::GetPluginInfo(a_pluginName.data()) != nullptr;
	}

	static std::optional<VersionStruct> GetPluginVersion(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::zstring_view a_pluginName)
	{
		const auto* pluginInfo = F4SE::GetPluginInfo(a_pluginName.data());
		if (!pluginInfo) {
			return std::nullopt;
		}

		return pluginInfo->GetPluginVersion();
	}

	static std::uint32_t GetPluginVersionPack(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::zstring_view a_pluginName)
	{
		const auto* pluginInfo = F4SE::GetPluginInfo(a_pluginName.data());
		if (!pluginInfo) {
			return 0;
		}

		return pluginInfo->GetPluginVersion().Pack<std::uint32_t>();
	}

	static std::uint32_t GetPluginVersionNumber(RE::BSScript::StaticTag /*a_staticTag*/,
		REX::zstring_view a_pluginName)
	{
		const auto* pluginInfo = F4SE::GetPluginInfo(a_pluginName.data());
		if (!pluginInfo) {
			return 0;
		}

		return pluginInfo->GetPluginVersionNumber();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionPack);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionMajor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionMinor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionPatch);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionBuild);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsPluginLoaded);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPluginVersion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPluginVersionPack);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPluginVersionNumber);
	}
}
