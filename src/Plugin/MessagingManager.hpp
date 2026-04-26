#pragma once

namespace Plugin::MessagingManager
{
	void Setup(REX::NotNull<const F4SE::MessagingInterface*> a_messaging);
	void Callback(F4SE::MessagingInterface::Message* a_message);
	void OnPostLoad();
	void OnGameDataReady();
	void OnNewGame(REX::NotNull<RE::TESQuest*> a_charGenQuest);
	void OnPostLoadGame(bool a_isSucceeded);
	void OnNewOrPostLoadGame(bool a_isSucceeded);
	void OnPostSaveGame(const RE::BSFixedString& a_saveName);
	void OnDeleteGame(const RE::BSFixedString& a_saveName);
}
