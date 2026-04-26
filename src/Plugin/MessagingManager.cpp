#include "Plugin/MessagingManager.hpp"

#include "Plugin/InternalManager.hpp"

namespace Plugin::MessagingManager
{
	void Setup(REX::NotNull<const F4SE::MessagingInterface*> a_messaging)
	{
		static constexpr auto FUNCTION_NAME = REX::GetCurrentFunctionName();

		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, [a_messaging]() {
			REX::LogDebug("{}"sv,
				FUNCTION_NAME);

			a_messaging->RegisterListener(Callback);
		});
	}

	void Callback(F4SE::MessagingInterface::Message* a_message)
	{
		if (!a_message) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		REX::LogDebug(R"(Received message of type "{}")"sv,
			a_message->GetType());

		using MessageType = F4SE::MessagingInterface::MessageType;

		switch (a_message->GetType()) {
			case MessageType::kPostLoad: {
				OnPostLoad();
				break;
			}
			case MessageType::kGameDataReady: {
				OnGameDataReady();
				break;
			}
			case MessageType::kNewGame: {
				const auto charGenQuest = a_message->GetAsPointer<RE::TESQuest>();
				if (!charGenQuest.has_value()) [[unlikely]] {
					REX::Assert(false);
					break;
				}

				OnNewGame(charGenQuest.value());
				OnNewOrPostLoadGame(charGenQuest.has_value());
				break;
			}
			case MessageType::kPostLoadGame: {
				const auto isSucceeded = a_message->GetAsRawValue<bool>();
				if (!isSucceeded.has_value()) [[unlikely]] {
					REX::Assert(false);
					break;
				}

				OnPostLoadGame(isSucceeded.value());
				OnNewOrPostLoadGame(isSucceeded.value());
				break;
			}
			case MessageType::kPostSaveGame: {
				const auto saveName = a_message->GetAsStringView<char>();
				if (!saveName.has_value()) [[unlikely]] {
					REX::Assert(false);
					break;
				}

				OnPostSaveGame(RE::BSFixedString(saveName.value()));
				break;
			}
			case MessageType::kDeleteGame: {
				const auto saveName = a_message->GetAsStringView<char>();
				if (!saveName.has_value()) [[unlikely]] {
					REX::Assert(false);
					break;
				}

				OnDeleteGame(RE::BSFixedString(saveName.value()));
				break;
			}
			default: {
				break;
			}
		}
	}

	void OnPostLoad()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		Plugin::GetInternalManager()->OnPostLoad();
	}

	void OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		Plugin::GetInternalManager()->OnGameDataReady();
	}

	void OnNewGame(REX::NotNull<RE::TESQuest*> a_charGenQuest)
	{
		REX::LogDebug("{} with char gen quest {:08X}"sv,
			REX::GetCurrentFunctionName(), a_charGenQuest->GetFormID());

		Plugin::GetInternalManager()->OnNewGame(a_charGenQuest);
	}

	void OnPostLoadGame(bool a_isSucceeded)
	{
		REX::LogDebug("{} with success {}"sv,
			REX::GetCurrentFunctionName(), a_isSucceeded);

		Plugin::GetInternalManager()->OnPostLoadGame(a_isSucceeded);
	}

	void OnNewOrPostLoadGame(bool a_isSucceeded)
	{
		REX::LogDebug("{} with success {}"sv,
			REX::GetCurrentFunctionName(), a_isSucceeded);

		Plugin::GetInternalManager()->OnNewOrPostLoadGame(a_isSucceeded);
	}

	void OnPostSaveGame(const RE::BSFixedString& a_saveName)
	{
		REX::LogDebug(R"({} with save name "{}")"sv,
			REX::GetCurrentFunctionName(), a_saveName);

		Plugin::GetInternalManager()->OnPostSaveGame(a_saveName);
	}

	void OnDeleteGame(const RE::BSFixedString& a_saveName)
	{
		REX::LogDebug(R"({} with save name "{}")"sv,
			REX::GetCurrentFunctionName(), a_saveName);

		Plugin::GetInternalManager()->OnDeleteGame(a_saveName);
	}
}
