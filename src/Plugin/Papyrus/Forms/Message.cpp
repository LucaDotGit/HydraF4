#include "Plugin/Papyrus/Forms/Message.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptStrictString.hpp"

namespace Plugin::Papyrus::Forms::Message
{
	using MessageFlags = RE::BGSMessage::Flags;

	static RE::TESQuest* GetOwnerQuest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_message->ownerQuest).load(std::memory_order_acquire);
	}

	static void SetOwnerQuest(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		RE::TESQuest* a_value)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_message->ownerQuest).store(a_value, std::memory_order_release);
	}

	static std::uint32_t GetDisplayTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_message->displayTime).load(std::memory_order_acquire);
	}

	static void SetDisplayTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		std::uint32_t a_value)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_message->displayTime).store(a_value, std::memory_order_release);
	}

	static RE::BSFixedStringCS GetUIFilePath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return {};
		}

		return a_message->swfFile;
	}

	static void SetUIFilePath(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		RE::BSFixedStringCS a_value)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return;
		}

		a_message->swfFile = a_value;
	}

	static RE::BGSLocalizedString GetShortName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return {};
		}

		return a_message->shortName;
	}

	static void SetShortName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		::Plugin::Internal::Script::ScriptStrictString a_value)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return;
		}

		a_message->shortName = RE::BGSLocalizedString(a_value.GetValue());
	}

	static std::uint32_t GetButtonCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return 0;
		}

		const auto& buttonList = a_message->buttonList;
		return static_cast<std::uint32_t>(std::distance(buttonList.begin(), buttonList.end()));
	}

	static RE::BGSLocalizedString GetNthButtonText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		std::int32_t a_index)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return {};
		}

		if (a_index < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId,
				a_index, 0, std::numeric_limits<std::int32_t>::max());
			return {};
		}

		const auto& buttonList = a_message->buttonList;

		auto buttonIndex = 0i32;
		for (auto buttonIt = buttonList.begin(); buttonIt != buttonList.end(); buttonIndex++, buttonIt++) {
			if (buttonIndex == a_index) {
				return (*buttonIt)->text;
			}
		}

		a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId,
			a_index, 0, buttonIndex);
		return {};
	}

	static bool SetNthButtonText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		std::int32_t a_index,
		::Plugin::Internal::Script::ScriptStrictString a_text)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return false;
		}

		if (a_index < 0) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId,
				a_index, 0, std::numeric_limits<std::int32_t>::max());
			return false;
		}

		auto& buttonList = a_message->buttonList;

		auto buttonIndex = 0i32;
		for (auto it = buttonList.begin(); it != buttonList.end(); buttonIndex++, it++) {
			if (buttonIndex == a_index) {
				(*it)->text = RE::BGSLocalizedString(a_text.GetValue());
				return true;
			}
		}

		a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId,
			a_index, 0, buttonIndex);
		return false;
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		MessageFlags a_flag)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return false;
		}

		return a_message->flags.any_atomic(a_flag);
	}

	static MessageFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return MessageFlags::kNone;
		}

		return a_message->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		MessageFlags a_flags)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return;
		}

		a_message->flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMessage* a_message,
		MessageFlags a_flag,
		bool a_set)
	{
		if (!a_message) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MESSAGE_NULL, a_stackId);
			return;
		}

		a_message->flags.set_atomic(a_set, a_flag);
	}

	static MessageFlags GetFlag_IsMessageBox(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MessageFlags::kMessageBox; }
	static MessageFlags GetFlag_HasInitialDelay(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MessageFlags::kInitialDelay; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOwnerQuest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetOwnerQuest);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDisplayTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDisplayTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetUIFilePath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetUIFilePath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetShortName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetShortName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetButtonCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthButtonText);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthButtonText);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsMessageBox);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasInitialDelay);
	}
}
