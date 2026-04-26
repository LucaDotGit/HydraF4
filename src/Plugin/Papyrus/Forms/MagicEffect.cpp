#include "Plugin/Papyrus/Forms/MagicEffect.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::MagicEffect
{
	using MagicEffectFlags = RE::EffectSetting::EffectSettingData::Flags;

	static RE::EffectArchetypes::ArchetypeID GetArchetype(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return RE::EffectArchetypes::ArchetypeID::kNone;
		}

		return a_magicEffect->data.archetype.get_atomic();
	}

	static void SetArchetype(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::EffectArchetypes::ArchetypeID a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		a_magicEffect->data.archetype.store_atomic(a_value);
	}

	static RE::MagicSystem::CastingType GetCastingType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return RE::MagicSystem::CastingType::kNone;
		}

		return a_magicEffect->data.castingType.get_atomic();
	}

	static void SetCastingType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::MagicSystem::CastingType a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		a_magicEffect->data.castingType.store_atomic(a_value);
	}

	static RE::MagicSystem::Delivery GetDeliveryType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return RE::MagicSystem::Delivery::kNone;
		}

		return a_magicEffect->data.delivery.get_atomic();
	}

	static void SetDeliveryType(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::MagicSystem::Delivery a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		a_magicEffect->data.delivery.store_atomic(a_value);
	}

	static RE::ActorValueInfo* GetPrimaryValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.primaryAV).load(std::memory_order_acquire);
	}

	static void SetPrimaryValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::ActorValueInfo* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.primaryAV).store(a_value, std::memory_order_release);
	}

	static RE::ActorValueInfo* GetSecondaryValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.secondaryAV).load(std::memory_order_acquire);
	}

	static void SetSecondaryValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::ActorValueInfo* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.secondaryAV).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetSecondaryValueWeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.secondaryAVWeight).load(std::memory_order_acquire);
	}

	static void SetSecondaryValueWeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.secondaryAVWeight).store(a_value, std::memory_order_release);
	}

	static RE::ActorValueInfo* GetResistValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.resistVariable).load(std::memory_order_acquire);
	}

	static void SetResistValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::ActorValueInfo* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.resistVariable).store(a_value, std::memory_order_release);
	}

	static RE::BGSPerk* GetPerk(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.perk).load(std::memory_order_acquire);
	}

	static void SetPerk(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSPerk* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.perk).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetTaperCurve(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.taperCurve).load(std::memory_order_acquire);
	}

	static void SetTaperCurve(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.taperCurve).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetTaperWeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.taperWeight).load(std::memory_order_acquire);
	}

	static void SetTaperWeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.taperWeight).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetTaperDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.taperDuration).load(std::memory_order_acquire);
	}

	static void SetTaperDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.taperDuration).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetBaseCost(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.baseCost).load(std::memory_order_acquire);
	}

	static void SetBaseCost(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.baseCost).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetSkillUsageMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.skillUsageMult).load(std::memory_order_acquire);
	}

	static void SetSkillUsageMult(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.skillUsageMult).store(a_value, std::memory_order_release);
	}

	static RE::BGSReferenceEffect* GetHitVisuals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.hitVisuals).load(std::memory_order_acquire);
	}

	static void SetHitVisuals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSReferenceEffect* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.hitVisuals).store(a_value, std::memory_order_release);
	}

	static RE::BGSReferenceEffect* GetEnchantVisuals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.enchantVisuals).load(std::memory_order_acquire);
	}

	static void SetEnchantVisuals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSReferenceEffect* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.enchantVisuals).store(a_value, std::memory_order_release);
	}

	static RE::BGSArtObject* GetCastingArt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.castingArt).load(std::memory_order_acquire);
	}

	static void SetCastingArt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSArtObject* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.castingArt).store(a_value, std::memory_order_release);
	}

	static RE::TESObjectLIGH* GetCastingLight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.light).load(std::memory_order_acquire);
	}

	static void SetCastingLight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::TESObjectLIGH* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.light).store(a_value, std::memory_order_release);
	}

	static RE::BGSArtObject* GetHitArt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.hitEffectArt).load(std::memory_order_acquire);
	}

	static void SetHitArt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSArtObject* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.hitEffectArt).store(a_value, std::memory_order_release);
	}

	static RE::TESEffectShader* GetHitShader(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.effectShader).load(std::memory_order_acquire);
	}

	static void SetHitShader(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::TESEffectShader* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.effectShader).store(a_value, std::memory_order_release);
	}

	static RE::BGSArtObject* GetEnchantArt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.enchantEffectArt).load(std::memory_order_acquire);
	}

	static void SetEnchantArt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSArtObject* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.enchantEffectArt).store(a_value, std::memory_order_release);
	}

	static RE::TESEffectShader* GetEnchantShader(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.enchantEffect).load(std::memory_order_acquire);
	}

	static void SetEnchantShader(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::TESEffectShader* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.enchantEffect).store(a_value, std::memory_order_release);
	}

	static RE::BGSProjectile* GetProjectile(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.projectileBase).load(std::memory_order_acquire);
	}

	static void SetProjectile(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSProjectile* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.projectileBase).store(a_value, std::memory_order_release);
	}

	static RE::BGSImpactDataSet* GetImpactDataSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.impactDataSet).load(std::memory_order_acquire);
	}

	static void SetImpactDataSet(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSImpactDataSet* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.impactDataSet).store(a_value, std::memory_order_release);
	}

	static RE::BGSExplosion* GetExplosion(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.explosion).load(std::memory_order_acquire);
	}

	static void SetExplosion(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSExplosion* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.explosion).store(a_value, std::memory_order_release);
	}

	static RE::TESImageSpaceModifier* GetImageSpaceMod(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.imageSpaceMod).load(std::memory_order_acquire);
	}

	static void SetImageSpaceMod(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::TESImageSpaceModifier* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.imageSpaceMod).store(a_value, std::memory_order_release);
	}

	static std::int32_t GetSpellArea(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_magicEffect->data.spellmakingArea).load(std::memory_order_acquire);
	}

	static void SetSpellArea(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		std::int32_t a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.spellmakingArea).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetSpellChargeTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.spellmakingChargeTime).load(std::memory_order_acquire);
	}

	static void SetSpellChargeTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.spellmakingChargeTime).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetAIScore(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.aiScore).load(std::memory_order_acquire);
	}

	static void SetAIScore(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.aiScore).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetAIDelayTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_magicEffect->data.aiDelayTime).load(std::memory_order_acquire);
	}

	static void SetAIDelayTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		REX::Float32 a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.aiDelayTime).store(a_value, std::memory_order_release);
	}

	static RE::SpellItem* GetEquipAbility(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_magicEffect->data.equipAbility).load(std::memory_order_acquire);
	}

	static void SetEquipAbility(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::SpellItem* a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_magicEffect->data.equipAbility).store(a_value, std::memory_order_release);
	}

	static RE::BGSLocalizedString GetDescription(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return {};
		}

		return a_magicEffect->magicItemDescription;
	}

	static void SetDescription(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		RE::BGSLocalizedString a_value)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		a_magicEffect->magicItemDescription = std::move(a_value);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		MagicEffectFlags a_flag)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return false;
		}

		return a_magicEffect->data.flags.any_atomic(a_flag);
	}

	static MagicEffectFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return MagicEffectFlags::kNone;
		}

		return a_magicEffect->data.flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		MagicEffectFlags a_flags)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		a_magicEffect->data.flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::EffectSetting* a_magicEffect,
		MagicEffectFlags a_flag,
		bool a_set)
	{
		if (!a_magicEffect) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_EFFECT_NULL, a_stackId);
			return;
		}

		a_magicEffect->data.flags.set(a_set, a_flag);
	}

	static MagicEffectFlags GetFlag_IsHostile(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kHostile; }
	static MagicEffectFlags GetFlag_Recover(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kRecover; }
	static MagicEffectFlags GetFlag_IsDetrimental(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kDetrimental; }
	static MagicEffectFlags GetFlag_SnapToNavMesh(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kSnapToNavMesh; }
	static MagicEffectFlags GetFlag_HasNoHitEvent(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kNoHitEvent; }
	static MagicEffectFlags GetFlag_DispelWithKeywords(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kDispelWithKeywords; }
	static MagicEffectFlags GetFlag_HasNoDuration(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kNoDuration; }
	static MagicEffectFlags GetFlag_HasNoMagnitude(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kNoMagnitude; }
	static MagicEffectFlags GetFlag_HasNoArea(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kNoArea; }
	static MagicEffectFlags GetFlag_FXPersist(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kFXPersist; }
	static MagicEffectFlags GetFlag_HasGoryVisuals(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kGoryVisuals; }
	static MagicEffectFlags GetFlag_IsHiddenInUI(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kHideInUI; }
	static MagicEffectFlags GetFlag_HasNoRecast(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kNoRecast; }
	static MagicEffectFlags GetFlag_PowerAffectMagnitude(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kPowerAffectsMagnitude; }
	static MagicEffectFlags GetFlag_PowerAffectDuration(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kPowerAffectsDuration; }
	static MagicEffectFlags GetFlag_IsPainless(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kPainless; }
	static MagicEffectFlags GetFlag_HasNoHitEffect(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kNoHitEffect; }
	static MagicEffectFlags GetFlag_HasNoDeathDispel(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return MagicEffectFlags::kNoDeathDispel; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetArchetype);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetArchetype);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCastingType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeliveryType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDeliveryType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPrimaryValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPrimaryValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSecondaryValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSecondaryValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSecondaryValueWeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSecondaryValueWeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetResistValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetResistValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPerk);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPerk);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTaperCurve);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTaperCurve);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTaperWeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTaperWeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTaperDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTaperDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBaseCost);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBaseCost);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSkillUsageMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSkillUsageMult);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHitVisuals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHitVisuals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEnchantVisuals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEnchantVisuals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingArt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCastingArt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingLight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCastingLight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHitArt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHitArt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHitShader);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHitShader);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEnchantArt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEnchantArt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEnchantShader);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEnchantShader);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetProjectile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetProjectile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetImpactDataSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetImpactDataSet);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetExplosion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetExplosion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetImageSpaceMod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetImageSpaceMod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSpellArea);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSpellArea);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSpellChargeTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSpellChargeTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAIScore);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAIScore);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAIDelayTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAIDelayTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetEquipAbility);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetEquipAbility);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDescription);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetDescription);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsHostile);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_Recover);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsDetrimental);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_SnapToNavMesh);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoHitEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DispelWithKeywords);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoMagnitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoArea);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_FXPersist);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasGoryVisuals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsHiddenInUI);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoRecast);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_PowerAffectMagnitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_PowerAffectDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPainless);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoHitEffect);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoDeathDispel);
	}
}
