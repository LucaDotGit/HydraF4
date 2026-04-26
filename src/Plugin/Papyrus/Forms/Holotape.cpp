#include "Plugin/Papyrus/Forms/Holotape.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Holotape
{
	using NoteType = RE::BGSNote::NOTE_TYPE;

	static bool GetHasBeenPlayed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return false;
		}

		return std::atomic_ref(a_holotape->hasBeenRead).load(std::memory_order_acquire);
	}

	static void SetHasBeenPlayed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape,
		bool a_value)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_holotape->hasBeenRead).store(a_value, std::memory_order_release);
		a_holotape->AddChange(RE::BGSNote::ChangeFlags::kRead);
	}

	static NoteType GetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return static_cast<NoteType>(-1);
		}

		return a_holotape->type.get_atomic();
	}

	static NoteType GetType_Voice(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return NoteType::kVoice; }
	static NoteType GetType_Scene(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return NoteType::kScene; }
	static NoteType GetType_Program(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return NoteType::kProgram; }
	static NoteType GetType_Terminal(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return NoteType::kTerminal; }

	static RE::BGSSoundDescriptorForm* GetVoice(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return nullptr;
		}

		return a_holotape->GetNoteVoice();
	}

	static void SetVoice(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return;
		}

		a_holotape->SetNoteVoice(a_value);
	}

	static RE::BGSScene* GetScene(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return nullptr;
		}

		return a_holotape->GetNoteScene();
	}

	static void SetScene(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape,
		RE::BGSScene* a_value)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return;
		}

		a_holotape->SetNoteScene(a_value);
	}

	static RE::BSFixedString GetProgram(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return {};
		}

		return a_holotape->programFile;
	}

	static void SetProgram(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape,
		RE::BSFixedString a_value)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return;
		}

		a_holotape->SetNoteProgram(a_value);
	}

	static RE::BGSTerminal* GetTerminal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return nullptr;
		}

		return a_holotape->GetNoteTerminal();
	}

	static void SetTerminal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSNote* a_holotape,
		RE::BGSTerminal* a_value)
	{
		if (!a_holotape) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::HOLOTAPE_NULL, a_stackId);
			return;
		}

		a_holotape->SetNoteTerminal(a_value);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHasBeenPlayed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHasBeenPlayed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Voice);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Scene);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Program);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Terminal);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetVoice);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetVoice);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetScene);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetScene);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetProgram);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetProgram);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetTerminal);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetTerminal);
	}
}