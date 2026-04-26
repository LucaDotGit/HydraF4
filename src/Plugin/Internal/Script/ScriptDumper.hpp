#pragma once

namespace Plugin::Internal
{
	class SettingsManager;
}

namespace Plugin::Internal::Script
{
	class ScriptDumper final
	{
	public:
		ScriptDumper(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);
		~ScriptDumper() noexcept;

		ScriptDumper(const ScriptDumper&) = delete;
		ScriptDumper(ScriptDumper&&) = delete;

		ScriptDumper& operator=(const ScriptDumper&) = delete;
		ScriptDumper& operator=(ScriptDumper&&) = delete;

		void OnGameDataReady();

	private:
		void DumpAllDebugScripts(RE::BSScript::Internal::VirtualMachine& a_internalVM);
		void DumpAllInvalidLinkedScripts(RE::BSScript::Internal::VirtualMachine& a_internalVM);
		void DumpAllInvalidLinkedStructs(RE::BSScript::Internal::VirtualMachine& a_internalVM);
		void DumpAllDelayedFunctions(RE::BSScript::Internal::VirtualMachine& a_internalVM);
		void DumpAllLatentFunctions(RE::BSScript::Internal::VirtualMachine& a_internalVM);

		REX::NotNull<std::shared_ptr<SettingsManager>> _settingsManager;
	};
}
