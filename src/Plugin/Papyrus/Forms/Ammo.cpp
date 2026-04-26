#include "Plugin/Papyrus/Forms/Ammo.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptStrictString.hpp"

namespace Plugin::Papyrus::Forms::Ammo
{
	using AmmoFlags = RE::AMMO_DATA::Flags;

	static RE::BGSProjectile* GetProjectile(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_ammo->data.projectile).load(std::memory_order_acquire);
	}

	static void SetProjectile(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo,
		RE::BGSProjectile* a_value)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_ammo->data.projectile).store(a_value, std::memory_order_release);
	}

	static std::uint32_t GetHealth(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_ammo->data.health).load(std::memory_order_acquire);
	}

	static void SetHealth(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo,
		std::uint32_t a_value)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_ammo->data.health).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetDamage(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_ammo->data.damage).load(std::memory_order_acquire);
	}

	static void SetDamage(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo,
		REX::Float32 a_value)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_ammo->data.damage).store(a_value, std::memory_order_release);
	}

	static RE::BGSLocalizedString GetShortName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return {};
		}

		return a_ammo->shortDesc;
	}

	static void SetShortName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo,
		::Plugin::Internal::Script::ScriptStrictString a_value)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return;
		}

		a_ammo->shortDesc = RE::BGSLocalizedString(a_value.GetValue());
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo,
		AmmoFlags a_flag)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return false;
		}

		return a_ammo->data.flags.any_atomic(a_flag);
	}

	static AmmoFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return AmmoFlags::kNone;
		}

		return a_ammo->data.flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo,
		AmmoFlags a_flags)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return;
		}

		a_ammo->data.flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESAmmo* a_ammo,
		AmmoFlags a_flag,
		bool a_set)
	{
		if (!a_ammo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::AMMO_NULL, a_stackId);
			return;
		}

		a_ammo->data.flags.set_atomic(a_set, a_flag);
	}

	static AmmoFlags GetFlag_IgnoreNormalWeaponResist(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AmmoFlags::kIgnoresNormalWeaponResist; }
	static AmmoFlags GetFlag_IsNonPlayable(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AmmoFlags::kNonPlayable; }
	static AmmoFlags GetFlag_HasCountBased3D(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return AmmoFlags::kHasCountBased3D; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetProjectile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetProjectile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHealth);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHealth);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDamage);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDamage);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetShortName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetShortName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreNormalWeaponResist);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsNonPlayable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasCountBased3D);
	}
}