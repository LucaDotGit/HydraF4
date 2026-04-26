#include "Plugin/Papyrus/Forms/MagicItem.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::MagicItem
{
	using MagicItemFlags = RE::MagicItem::Data::Flags;

	using SpellType = RE::MagicSystem::SpellType;
	using CastingType = RE::MagicSystem::CastingType;
	using DeliveryType = RE::MagicSystem::Delivery;

	static std::uint32_t GetEffectCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return 0;
		}

		return a_magicItem->listOfEffects.size();
	}

	static RE::EffectSetting* GetNthEffect(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		std::uint32_t a_index)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return nullptr;
		}

		const auto& effects = a_magicItem->listOfEffects;
		if (a_index >= effects.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, effects.size());
			return nullptr;
		}

		const auto* effect = effects[a_index];
		if (!effect) {
			return nullptr;
		}

		return effect->effectSetting;
	}

	static bool SetNthEffect(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		std::uint32_t a_index,
		RE::EffectSetting* a_effect)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return false;
		}

		auto& effects = a_magicItem->listOfEffects;
		if (a_index >= effects.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, effects.size());
			return false;
		}

		auto* effect = effects[a_index];
		if (!effect) {
			return false;
		}

		effect->effectSetting = a_effect;
		return true;
	}

	static REX::Float32 GetNthEffectMagnitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		std::uint32_t a_index)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return 0.0_f32;
		}

		const auto& effects = a_magicItem->listOfEffects;
		if (a_index >= effects.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, effects.size());
			return 0.0_f32;
		}

		const auto* effect = effects[a_index];
		if (!effect) {
			return 0.0_f32;
		}

		return effect->data.magnitude;
	}

	static bool SetNthEffectMagnitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		std::uint32_t a_index,
		REX::Float32 a_magnitude)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return false;
		}

		auto& effects = a_magicItem->listOfEffects;
		if (a_index >= effects.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, effects.size());
			return false;
		}

		auto* effect = effects[a_index];
		if (!effect) {
			return false;
		}

		effect->data.magnitude = a_magnitude;
		return true;
	}

	static std::int32_t GetNthEffectArea(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		std::uint32_t a_index)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return 0;
		}

		const auto& effects = a_magicItem->listOfEffects;
		if (a_index >= effects.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, effects.size());
			return 0;
		}

		const auto* effect = effects[a_index];
		if (!effect) {
			return 0;
		}

		return effect->data.area;
	}

	static bool SetNthEffectArea(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		std::uint32_t a_index,
		std::int32_t a_area)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return false;
		}

		auto& effects = a_magicItem->listOfEffects;
		if (a_index >= effects.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, effects.size());
			return false;
		}

		auto* effect = effects[a_index];
		if (!effect) {
			return false;
		}

		effect->data.area = a_area;
		return true;
	}

	static std::int32_t GetNthEffectDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		std::uint32_t a_index)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return 0;
		}

		const auto& effects = a_magicItem->listOfEffects;
		if (a_index >= effects.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, effects.size());
			return 0;
		}

		const auto* effect = effects[a_index];
		if (!effect) {
			return 0;
		}

		return effect->data.duration;
	}

	static bool SetNthEffectDuration(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		std::uint32_t a_index,
		std::int32_t a_duration)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return false;
		}

		auto& effects = a_magicItem->listOfEffects;
		if (a_index >= effects.size()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INDEX_OUT_OF_RANGE, a_stackId, a_index, 0, effects.size());
			return false;
		}

		auto* effect = effects[a_index];
		if (!effect) {
			return false;
		}

		effect->data.duration = a_duration;
		return true;
	}

	static SpellType GetType_Spell(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kSpell; }
	static SpellType GetType_Disease(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kDisease; }
	static SpellType GetType_Power(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kPower; }
	static SpellType GetType_LesserPower(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kLesserPower; }
	static SpellType GetType_Ability(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kAbility; }
	static SpellType GetType_Poison(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kPoison; }
	static SpellType GetType_Enchantment(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kEnchantment; }
	static SpellType GetType_Potion(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kPotion; }
	static SpellType GetType_Alchemy(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kAlchemy; }
	static SpellType GetType_Ingredient(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kIngredient; }
	static SpellType GetType_Addiction(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return SpellType::kAddiction; }

	static CastingType GetCastingType_ConstantEffect(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CastingType::kConstantEffect; }
	static CastingType GetCastingType_FireAndForget(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CastingType::kFireAndForget; }
	static CastingType GetCastingType_Concentration(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return CastingType::kConcentration; }

	static DeliveryType GetDeliveryType_Self(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DeliveryType::kSelf; }
	static DeliveryType GetDeliveryType_Touch(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DeliveryType::kTouch; }
	static DeliveryType GetDeliveryType_Aimed(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DeliveryType::kAimed; }
	static DeliveryType GetDeliveryType_TargetActor(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DeliveryType::kTargetActor; }
	static DeliveryType GetDeliveryType_TargetLocation(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return DeliveryType::kTargetLocation; }

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		MagicItemFlags a_flag)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return false;
		}

		const auto* data = a_magicItem->GetData();
		if (!data) {
			return false;
		}

		return data->flags.any_atomic(a_flag);
	}

	static MagicItemFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return MagicItemFlags::kNone;
		}

		const auto* data = a_magicItem->GetData();
		if (!data) {
			return MagicItemFlags::kNone;
		}

		return data->flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		MagicItemFlags a_flags)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return;
		}

		auto* data = a_magicItem->GetData();
		if (!data) {
			return;
		}

		data->flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::MagicItem* a_magicItem,
		MagicItemFlags a_flag,
		bool a_set)
	{
		if (!a_magicItem) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MAGIC_ITEM_NULL, a_stackId);
			return;
		}

		auto* data = a_magicItem->GetData();
		if (!data) {
			return;
		}

		data->flags.set_atomic(a_set, a_flag);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetEffectCount);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEffect);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEffect);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEffectMagnitude);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEffectMagnitude);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEffectArea);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEffectArea);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNthEffectDuration);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNthEffectDuration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Spell);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Disease);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Power);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_LesserPower);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Ability);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Poison);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Enchantment);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Potion);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Alchemy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Ingredient);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetType_Addiction);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingType_ConstantEffect);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingType_FireAndForget);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCastingType_Concentration);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeliveryType_Self);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeliveryType_Touch);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeliveryType_Aimed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeliveryType_TargetActor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeliveryType_TargetLocation);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
	}
}
