#include "Plugin/Papyrus/Plugins.hpp"

namespace Plugin::Papyrus::Plugins
{
	using PluginFlags = RE::TESFile::Flags;

	static bool IsPluginLoaded(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_pluginName)
	{
		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto pluginIndex = dataHandler->FindLoadedFileIndex(a_pluginName);
		return pluginIndex.has_value();
	}

	static bool IsNthPluginLoaded(RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_pluginIndex)
	{
		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto* plugin = dataHandler->FindLoadedFileByIndex(static_cast<std::uint16_t>(a_pluginIndex));
		return plugin != nullptr;
	}

	static std::uint32_t GetPluginCount(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return 0;
		}

		return dataHandler->GetFileCount();
	}

	static std::int32_t GetPluginIndex(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_pluginName)
	{
		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return -1;
		}

		const auto pluginIndex = dataHandler->FindLoadedFileIndex(a_pluginName);
		if (!pluginIndex.has_value()) {
			return -1;
		}

		return pluginIndex.value();
	}

	static RE::BSFixedString GetNthPluginName(RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_pluginIndex)
	{
		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		const auto* plugin = dataHandler->FindLoadedFileByIndex(static_cast<std::uint16_t>(a_pluginIndex));
		if (!plugin) {
			return {};
		}

		return plugin->filename.data();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsPluginLoaded);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsNthPluginLoaded);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPluginCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPluginIndex);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthPluginName);
	}
}
