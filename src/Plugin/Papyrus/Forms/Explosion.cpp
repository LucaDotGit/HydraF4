#include "Plugin/Papyrus/Forms/Explosion.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Vectors3.hpp"

namespace Plugin::Papyrus::Forms::Explosion
{
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;

	using ExplosionFlags = RE::BGSExplosionData::Flags;

	static RE::TESObjectLIGH* GetLight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_explosion->data.light).load(std::memory_order_acquire);
	}

	static void SetLight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.light).store(a_light, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetSound01(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_explosion->data.sound01).load(std::memory_order_acquire);
	}

	static void SetSound01(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		RE::BGSSoundDescriptorForm* a_sound)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.sound01).store(a_sound, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetSound02(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_explosion->data.sound02).load(std::memory_order_acquire);
	}

	static void SetSound02(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		RE::BGSSoundDescriptorForm* a_sound)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.sound02).store(a_sound, std::memory_order_release);
	}

	static RE::BGSImpactDataSet* GetImpactDataSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_explosion->data.impactDataSet).load(std::memory_order_acquire);
	}

	static void SetImpactDataSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		RE::BGSImpactDataSet* a_dataSet)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.impactDataSet).store(a_dataSet, std::memory_order_release);
	}

	static RE::TESBoundObject* GetPlacedObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_explosion->data.impactPlacedObject).load(std::memory_order_acquire);
	}

	static void SetPlacedObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		RE::TESBoundObject* a_object)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.impactPlacedObject).store(a_object, std::memory_order_release);
	}

	static REX::Float32 GetPlacedObjectAutoFadeDelay(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_explosion->data.placedObjectFadeDelay).load(std::memory_order_acquire);
	}

	static void SetPlacedObjectAutoFadeDelay(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		REX::Float32 a_delay)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.placedObjectFadeDelay).store(a_delay, std::memory_order_release);
	}

	static RE::BGSProjectile* GetSpawnedProjectile(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_explosion->data.spawnProjectile).load(std::memory_order_acquire);
	}

	static void SetSpawnedProjectile(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.spawnProjectile).store(a_projectile, std::memory_order_release);
	}

	static std::optional<Vector3Struct> GetProjectileSpawnPosition(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return std::nullopt;
		}

		return Vector3Struct::FromNiPoint3(std::atomic_ref(a_explosion->data.projectileVector).load(std::memory_order_acquire));
	}

	static void SetProjectileSpawnPosition(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		std::optional<Vector3Struct> a_position)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		if (!a_position.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.projectileVector).store(Vector3Struct::ToNiPoint3(a_position.value()), std::memory_order_release);
	}

	static REX::Float32 GetProjectileSpreadAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_explosion->data.projectileSpread).load(std::memory_order_acquire);
	}

	static void SetProjectileSpreadAngle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		REX::Float32 a_angle)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.projectileSpread).store(a_angle, std::memory_order_release);
	}

	static std::uint32_t GetProjectileCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_explosion->data.projectileCount).load(std::memory_order_acquire);
	}

	static void SetProjectileCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		std::uint32_t a_count)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.projectileCount).store(a_count, std::memory_order_release);
	}

	static REX::Float32 GetForce(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_explosion->data.force).load(std::memory_order_acquire);
	}

	static void SetForce(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		REX::Float32 a_force)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.force).store(a_force, std::memory_order_release);
	}

	static REX::Float32 GetDamage(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_explosion->data.damage).load(std::memory_order_acquire);
	}

	static void SetDamage(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		REX::Float32 a_damage)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.damage).store(a_damage, std::memory_order_release);
	}

	static REX::Float32 GetInnerRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_explosion->data.innerRadius).load(std::memory_order_acquire);
	}

	static void SetInnerRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		REX::Float32 a_radius)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.innerRadius).store(a_radius, std::memory_order_release);
	}

	static REX::Float32 GetOuterRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_explosion->data.outerRadius).load(std::memory_order_acquire);
	}

	static void SetOuterRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		REX::Float32 a_radius)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.outerRadius).store(a_radius, std::memory_order_release);
	}

	static REX::Float32 GetImageSpaceRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_explosion->data.imageSpaceRadius).load(std::memory_order_acquire);
	}

	static void SetImageSpaceRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		REX::Float32 a_radius)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.imageSpaceRadius).store(a_radius, std::memory_order_release);
	}

	static REX::Float32 GetVerticalOffsetMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_explosion->data.verticalOffsetMult).load(std::memory_order_acquire);
	}

	static void SetVerticalOffsetMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		REX::Float32 a_mult)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_explosion->data.verticalOffsetMult).store(a_mult, std::memory_order_release);
	}

	static RE::SOUND_LEVEL GetSoundLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return static_cast<RE::SOUND_LEVEL>(-1);
		}

		return a_explosion->data.soundLevel.get_atomic();
	}

	static void SetSoundLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		RE::SOUND_LEVEL a_level)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		a_explosion->data.soundLevel.store_atomic(a_level);
	}

	static RE::STAGGER_MAGNITUDE GetStaggerMagnitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return RE::STAGGER_MAGNITUDE::kNone;
		}

		return a_explosion->data.staggerMagnitude.get_atomic();
	}

	static void SetStaggerMagnitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		RE::STAGGER_MAGNITUDE a_magnitude)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		a_explosion->data.staggerMagnitude.store_atomic(a_magnitude);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		ExplosionFlags a_flag)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return false;
		}

		return a_explosion->data.flags.any_atomic(a_flag);
	}

	static ExplosionFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return ExplosionFlags::kNone;
		}

		return a_explosion->data.flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		ExplosionFlags a_flags)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		a_explosion->data.flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSExplosion* a_explosion,
		ExplosionFlags a_flag,
		bool a_set)
	{
		if (!a_explosion) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::EXPLOSION_NULL, a_stackId);
			return;
		}

		a_explosion->data.flags.set_atomic(a_set, a_flag);
	}

	static ExplosionFlags GetFlag_AlwaysUseWorldOrientation(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kAlwaysUsesWorldOrientation; }
	static ExplosionFlags GetFlag_AlwaysKnockDown(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kKnockDown_Always; }
	static ExplosionFlags GetFlag_KnockDownByFormula(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kKnockDown_ByFormula; }
	static ExplosionFlags GetFlag_IgnoreLosCheck(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kIgnoreLOSCheck; }
	static ExplosionFlags GetFlag_PushExplosionSourceRefOnly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kPushExplosionSourceRefOnly; }
	static ExplosionFlags GetFlag_IgnoreImageSpaceSwap(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kIgnoreImageSpaceSwap; }
	static ExplosionFlags GetFlag_HasChain(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kChain; }
	static ExplosionFlags GetFlag_HasNoControllerVibration(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kNoControllerVibration; }
	static ExplosionFlags GetFlag_PlacedObjectPersist(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kPlacedObjectPersists; }
	static ExplosionFlags GetFlag_SkipUnderwaterTest(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ExplosionFlags::kSkipUnderwaterTest; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSound01);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSound01);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSound02);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSound02);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetImpactDataSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetImpactDataSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPlacedObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPlacedObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPlacedObjectAutoFadeDelay);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPlacedObjectAutoFadeDelay);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSpawnedProjectile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSpawnedProjectile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetProjectileSpawnPosition);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetProjectileSpawnPosition);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetProjectileSpreadAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetProjectileSpreadAngle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetProjectileCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetProjectileCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetForce);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetForce);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDamage);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDamage);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInnerRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetInnerRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetOuterRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetOuterRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetImageSpaceRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetImageSpaceRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVerticalOffsetMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetVerticalOffsetMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSoundLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSoundLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetStaggerMagnitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetStaggerMagnitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AlwaysUseWorldOrientation);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AlwaysKnockDown);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_KnockDownByFormula);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreLosCheck);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_PushExplosionSourceRefOnly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreImageSpaceSwap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasChain);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoControllerVibration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_PlacedObjectPersist);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_SkipUnderwaterTest);
	}
}