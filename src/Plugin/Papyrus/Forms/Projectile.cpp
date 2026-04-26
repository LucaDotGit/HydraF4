#include "Plugin/Papyrus/Forms/Projectile.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Projectile
{
	using ProjectileType = RE::BGSProjectileData::Type;
	using ProjectileFlags = RE::BGSProjectileData::Flags;

	static ProjectileType GetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return ProjectileType::kNone;
		}

		return a_projectile->data.type.get_atomic();
	}

	static void SetType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		ProjectileType a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		a_projectile->data.type.store_atomic(a_value);
	}

	static ProjectileType GetType_Missile(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileType::kMissile; }
	static ProjectileType GetType_Lobber(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileType::kLobber; }
	static ProjectileType GetType_Beam(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileType::kBeam; }
	static ProjectileType GetType_Flame(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileType::kFlame; }
	static ProjectileType GetType_Cone(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileType::kCone; }
	static ProjectileType GetType_Barrier(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileType::kBarrier; }
	static ProjectileType GetType_Arrow(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileType::kArrow; }

	static REX::Float32 GetSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.speed).load(std::memory_order_acquire);
	}

	static void SetSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.speed).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetGravity(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.gravity).load(std::memory_order_acquire);
	}

	static void SetGravity(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.gravity).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.range).load(std::memory_order_acquire);
	}

	static void SetRange(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.range).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetImpactForce(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.force).load(std::memory_order_acquire);
	}

	static void SetImpactForce(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.force).store(a_value, std::memory_order_release);
	}

	static std::int8_t GetTracerFrequency(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_projectile->data.tracerFrequency).load(std::memory_order_acquire);
	}

	static void SetTracerFrequency(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		std::int8_t a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.tracerFrequency).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetFadeDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.fadeOutTime).load(std::memory_order_acquire);
	}

	static void SetFadeDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.fadeOutTime).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetConeSpread(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.coneSpread).load(std::memory_order_acquire);
	}

	static void SetConeSpread(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.coneSpread).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetCollisionRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.collisionRadius).load(std::memory_order_acquire);
	}

	static void SetCollisionRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.collisionRadius).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetLifetime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.lifetime).load(std::memory_order_acquire);
	}

	static void SetLifetime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.lifetime).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetRelaunchInterval(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.relaunchInterval).load(std::memory_order_acquire);
	}

	static void SetRelaunchInterval(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.relaunchInterval).store(a_value, std::memory_order_release);
	}

	static RE::TESObjectLIGH* GetMuzzleFlashLight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.muzzleFlashLight).load(std::memory_order_acquire);
	}

	static void SetMuzzleFlashLight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::TESObjectLIGH* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.muzzleFlashLight).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetMuzzleFlashDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.muzzleFlashDuration).load(std::memory_order_acquire);
	}

	static void SetMuzzleFlashDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.muzzleFlashDuration).store(a_value, std::memory_order_release);
	}

	static RE::TESObjectLIGH* GetLight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.light).load(std::memory_order_acquire);
	}

	static void SetLight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::TESObjectLIGH* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.light).store(a_value, std::memory_order_release);
	}

	static RE::TESObjectWEAP* GetSourceWeapon(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.defaultWeaponSource).load(std::memory_order_acquire);
	}

	static void SetSourceWeapon(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::TESObjectWEAP* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.defaultWeaponSource).store(a_value, std::memory_order_release);
	}

	static RE::BGSTextureSet* GetDecal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.decalData).load(std::memory_order_acquire);
	}

	static void SetDecal(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::BGSTextureSet* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.decalData).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.activeSoundLoop).load(std::memory_order_acquire);
	}

	static void SetSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.activeSoundLoop).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetCountdownSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.countdownSound).load(std::memory_order_acquire);
	}

	static void SetCountdownSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.countdownSound).store(a_value, std::memory_order_release);
	}

	static RE::BGSSoundDescriptorForm* GetDisarmSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.deactivateSound).load(std::memory_order_acquire);
	}

	static void SetDisarmSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.deactivateSound).store(a_value, std::memory_order_release);
	}

	static RE::SOUND_LEVEL GetSoundLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return static_cast<RE::SOUND_LEVEL>(-1);
		}

		return a_projectile->soundLevel.get_atomic();
	}

	static void SetSoundLevel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::SOUND_LEVEL a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		a_projectile->soundLevel.store_atomic(a_value);
	}

	static RE::BGSExplosion* GetExplosion(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.explosionType).load(std::memory_order_acquire);
	}

	static void SetExplosion(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::BGSExplosion* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.explosionType).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetExplosionTimer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.explosionTimer).load(std::memory_order_acquire);
	}

	static void SetExplosionTimer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.explosionTimer).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetExplosionProximity(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_projectile->data.explosionProximity).load(std::memory_order_acquire);
	}

	static void SetExplosionProximity(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		REX::Float32 a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.explosionProximity).store(a_value, std::memory_order_release);
	}

	static RE::BGSCollisionLayer* GetCollisionLayer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.collisionLayer).load(std::memory_order_acquire);
	}

	static void SetCollisionLayer(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::BGSCollisionLayer* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.collisionLayer).store(a_value, std::memory_order_release);
	}

	static RE::BGSProjectile* GetVatsProjectile(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_projectile->data.vatsProjectile).load(std::memory_order_acquire);
	}

	static void SetVatsProjectile(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		RE::BGSProjectile* a_value)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}
		if (!a_value) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_projectile->data.vatsProjectile).store(a_value, std::memory_order_release);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		ProjectileFlags a_flag)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return false;
		}

		return a_projectile->data.flags.any_atomic(a_flag);
	}

	static ProjectileFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return ProjectileFlags::kNone;
		}

		return a_projectile->data.flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		ProjectileFlags a_flags)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		a_projectile->data.flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSProjectile* a_projectile,
		ProjectileFlags a_flag,
		bool a_set)
	{
		if (!a_projectile) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROJECTILE_NULL, a_stackId);
			return;
		}

		a_projectile->data.flags.set_atomic(a_set, a_flag);
	}

	static ProjectileFlags GetFlag_HasHitScan(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kHitScan; }
	static ProjectileFlags GetFlag_HasExplosion(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kExplosion; }
	static ProjectileFlags GetFlag_HasAltExplosionTrigger(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kExplosionAltTrigger; }
	static ProjectileFlags GetFlag_HasMuzzleFlash(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kMuzzleFlash; }
	static ProjectileFlags GetFlag_CanBeDisarmed(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kCanTurnOff; }
	static ProjectileFlags GetFlag_CanBePickedUp(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kCanPickUp; }
	static ProjectileFlags GetFlag_IsSupersonic(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kSupersonic; }
	static ProjectileFlags GetFlag_PinLimbs(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kPinsLimbs; }
	static ProjectileFlags GetFlag_PassSmallTransparent(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kPassSmallTransparent; }
	static ProjectileFlags GetFlag_DisableCombatAimCorrection(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kDisableCombatAimCorrection; }
	static ProjectileFlags GetFlag_PenetrateGeometry(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kPenetratesGeometry; }
	static ProjectileFlags GetFlag_UpdateContinuously(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kContinuousUpdate; }
	static ProjectileFlags GetFlag_SeekTarget(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return ProjectileFlags::kSeeksTarget; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Missile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Lobber);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Beam);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Flame);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Cone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Barrier);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Arrow);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetGravity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetGravity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRange);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetImpactForce);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetImpactForce);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTracerFrequency);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTracerFrequency);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFadeDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFadeDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetConeSpread);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetConeSpread);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCollisionRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCollisionRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLifetime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLifetime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRelaunchInterval);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRelaunchInterval);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMuzzleFlashLight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMuzzleFlashLight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMuzzleFlashDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetMuzzleFlashDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSourceWeapon);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSourceWeapon);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDecal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDecal);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCountdownSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCountdownSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDisarmSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDisarmSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSoundLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSoundLevel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetExplosion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetExplosion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetExplosionTimer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetExplosionTimer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetExplosionProximity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetExplosionProximity);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCollisionLayer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCollisionLayer);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVatsProjectile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetVatsProjectile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasHitScan);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasExplosion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasAltExplosionTrigger);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasMuzzleFlash);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CanBeDisarmed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CanBePickedUp);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsSupersonic);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_PinLimbs);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_PassSmallTransparent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DisableCombatAimCorrection);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_PenetrateGeometry);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_UpdateContinuously);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_SeekTarget);
	}
}