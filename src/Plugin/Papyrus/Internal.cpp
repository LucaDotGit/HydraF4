#include "Plugin/Papyrus/Internal.hpp"

#include "Plugin/Internal/Structs/Versions.hpp"

namespace Plugin::Papyrus::Internal
{
	using VersionStruct = ::Plugin::Internal::Structs::Versions::VersionData;

	static bool IsLoaded(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return true;
	}

	static RE::BSFixedString GetName(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto NAME = RE::BSFixedString(F4SE::GetPluginName());
		return NAME;
	}

	static VersionStruct GetVersion(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetPluginVersion();
	}

	static std::uint32_t GetVersionPack(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetPluginVersion().Pack<std::uint32_t>();
	}

	static RE::BSFixedString GetVersionString(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto VERSION_STRING = RE::BSFixedString(F4SE::GetPluginVersion().ToString<char>());
		return VERSION_STRING;
	}

	static std::uint16_t GetVersionMajor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetPluginVersion().GetMajor();
	}

	static std::uint16_t GetVersionMinor(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetPluginVersion().GetMinor();
	}

	static std::uint16_t GetVersionPatch(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetPluginVersion().GetPatch();
	}

	static std::uint16_t GetVersionBuild(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::GetPluginVersion().GetBuild();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsLoaded);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionPack);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionMajor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionMinor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionPatch);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVersionBuild);
	}
}
