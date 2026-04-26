#pragma once

namespace Plugin::Internal
{
	class SettingsManager;
}

namespace Plugin::Internal::Game
{
	class GameEditorIds final
		: public RE::BSTEventSink<RE::TESFormIDRemapEvent>,
		  public RE::BSTEventSink<RE::TESFormDeleteEvent>
	{
	public:
		GameEditorIds(const REX::NotNull<std::shared_ptr<SettingsManager>>& a_settingsManager);
		~GameEditorIds() noexcept override;

		GameEditorIds(const GameEditorIds&) = delete;
		GameEditorIds(GameEditorIds&&) = delete;

		GameEditorIds& operator=(const GameEditorIds&) = delete;
		GameEditorIds& operator=(GameEditorIds&&) = delete;

		RE::BSEventNotifyControl ProcessEvent(const RE::TESFormIDRemapEvent& a_event,
			RE::BSTEventSource<RE::TESFormIDRemapEvent>* a_eventSource) override;
		RE::BSEventNotifyControl ProcessEvent(const RE::TESFormDeleteEvent& a_event,
			RE::BSTEventSource<RE::TESFormDeleteEvent>* a_eventSource) override;

		void OnGameDataReady();

		void RegisterEvents();
		void UnregisterEvents();

		[[nodiscard]] auto GetFormByEditorId(const RE::BSFixedString& a_editorId) const -> std::optional<std::variant<REX::Observer<RE::TESForm*>, RE::NiPointer<RE::TESObjectREFR>>>;
		[[nodiscard]] auto GetFormEditorId(REX::Observer<const RE::TESForm*> a_form) const -> std::optional<RE::BSFixedString>;
		bool SetFormEditorId(REX::Observer<RE::TESForm*> a_form, const RE::BSFixedString& a_editorId);
		bool MoveFormEditorId(RE::TESFormID a_oldFormId, REX::Observer<RE::TESForm*> a_newForm);
		bool RemoveFormEditorId(RE::TESFormID a_formId);

	private:
		void LogDuplicateEditorId(RE::TESFormID a_ownerFormId, RE::TESFormID a_senderFormId, const RE::BSFixedString& a_editorId) const;

		REX::NotNull<std::shared_ptr<SettingsManager>> _settingsManager;
		std::unordered_map<RE::TESFormID, RE::BSFixedString> _formIdToEditorIdMap;
		std::once_flag _reserveOnceFlag;
	};
}
