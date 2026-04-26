#include "Plugin/Papyrus/Forms/ColorForm.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Colors.hpp"

namespace Plugin::Papyrus::Forms::ColorForm
{
	using ColorStruct = ::Plugin::Internal::Structs::Colors::ColorData;

	using ColorFlags = RE::BGSColorForm::Flags;
	using ColorRecordFlags = RE::BGSColorForm::RecordFlags;

	static std::optional<ColorStruct> GetColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return std::nullopt;
		}

		if (a_colorForm->flags.any_atomic(ColorFlags::kRemappingIndex)) {
			return std::nullopt;
		}

		return ColorStruct::FromHex(std::atomic_ref(a_colorForm->hexColor).load(std::memory_order_acquire));
	}

	static void SetColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm,
		std::optional<ColorStruct> a_value)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return;
		}

		if (!a_value) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_colorForm->hexColor).store(a_value->ToHex(), std::memory_order_release);
		a_colorForm->flags.set_atomic(true, ColorFlags::kRemappingIndex);
	}

	static std::uint32_t GetHexColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return 0;
		}

		if (a_colorForm->flags.any_atomic(ColorFlags::kRemappingIndex)) {
			return 0;
		}

		return std::atomic_ref(a_colorForm->hexColor).load(std::memory_order_acquire);
	}

	static void SetHexColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm,
		std::uint32_t a_value)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_colorForm->hexColor).store(a_value, std::memory_order_release);
		a_colorForm->flags.set_atomic(true, ColorFlags::kRemappingIndex);
	}

	static REX::Float32 GetRemappingIndex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return 0.0_f32;
		}

		if (!a_colorForm->flags.any_atomic(ColorFlags::kRemappingIndex)) {
			return 0.0_f32;
		}

		return std::atomic_ref(a_colorForm->remappingIndex).load(std::memory_order_acquire);
	}

	static void SetRemappingIndex(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm,
		REX::Float32 a_value)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return;
		}

		a_colorForm->flags.set_atomic(true, ColorFlags::kRemappingIndex);
		std::atomic_ref(a_colorForm->remappingIndex).store(a_value, std::memory_order_release);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm,
		ColorFlags a_flag)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return false;
		}

		return a_colorForm->flags.any_atomic(a_flag);
	}

	static ColorFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return ColorFlags::kNone;
		}

		return a_colorForm->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm,
		ColorFlags a_flags)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return;
		}

		a_colorForm->flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSColorForm* a_colorForm,
		ColorFlags a_flag,
		bool a_set)
	{
		if (!a_colorForm) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::COLOR_FORM_NULL, a_stackId);
			return;
		}

		a_colorForm->flags.set_atomic(a_set, a_flag);
	}

	static ColorFlags GetFlag_IsPlayable(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ColorFlags::kPlayable; }
	static ColorFlags GetFlag_HasRemappingIndex(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ColorFlags::kRemappingIndex; }
	static ColorFlags GetFlag_HasExtendedLut(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ColorFlags::kExtendedLUT; }

	static ColorRecordFlags GetRecordFlag_IsNonPlayable(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ColorRecordFlags::kNonPlayable; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHexColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHexColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRemappingIndex);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRemappingIndex);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPlayable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasRemappingIndex);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasExtendedLut);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsNonPlayable);
	}
}
