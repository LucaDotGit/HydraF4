#pragma once

namespace Plugin::Internal::Script::ScriptErrors
{
	inline constexpr auto OUT_OF_SANDBOX = "The specified path is outside the sandbox"sv;
	inline constexpr auto OUT_OF_SANDBOX_OLD_PATH = "The specified old path is outside the sandbox"sv;
	inline constexpr auto OUT_OF_SANDBOX_NEW_PATH = "The specified new path is outside the sandbox"sv;
	inline constexpr auto OUT_OF_SANDBOX_SOURCE_PATH = "The specified source path is outside the sandbox"sv;
	inline constexpr auto OUT_OF_SANDBOX_TARGET_PATH = "The specified target path is outside the sandbox"sv;
	inline constexpr auto OUT_OF_SANDBOX_PATH_01 = "The specified first path is outside the sandbox"sv;
	inline constexpr auto OUT_OF_SANDBOX_PATH_02 = "The specified second path is outside the sandbox"sv;

	inline constexpr auto SCRIPT_FUNCTION_REF_NULL = "The specified function ref cannot be none"sv;
	inline constexpr auto SCRIPT_FUNCTION_REF_INVALID = "The specified function ref is invalid"sv;
	inline constexpr auto SCRIPT_FUNCTION_REF_NON_INVOKABLE = "The specified function ref is not invokable"sv;

	inline constexpr auto SCRIPT_NAME_NOT_FOUND = R"(The specified script name "{}" does not exist)"sv;
	inline constexpr auto STRUCT_NAME_NOT_FOUND = R"(The specified struct name "{}" does not exist)"sv;
	inline constexpr auto FUNCTION_NAME_NOT_FOUND = R"(The specified function name "{}" does not exist)"sv;
	inline constexpr auto VARIABLE_NOT_FOUND = R"(The specified variable "{}" does not exist)"sv;
	inline constexpr auto PROPERTY_NOT_FOUND = R"(The specified property "{}" does not exist)"sv;

	inline constexpr auto SCRIPT_NAME_EMPTY = "The specified script name cannot be empty"sv;
	inline constexpr auto STRUCT_NAME_EMPTY = "The specified struct name cannot be empty"sv;
	inline constexpr auto FUNCTION_NAME_EMPTY = "The specified function name cannot be empty"sv;
	inline constexpr auto EVENT_NAME_EMPTY = "The specified event name cannot be empty"sv;

	inline constexpr auto SCRIPT_OBJECT_NULL = "The specified script object cannot be none"sv;
	inline constexpr auto STRUCT_NULL = "The specified struct cannot be none"sv;
	inline constexpr auto STRUCT_01_NULL = "The specified first struct cannot be none"sv;
	inline constexpr auto STRUCT_02_NULL = "The specified second struct cannot be none"sv;
	inline constexpr auto STRUCT_03_NULL = "The specified third struct cannot be none"sv;
	inline constexpr auto ARRAY_NULL = "The specified array cannot be none"sv;
	inline constexpr auto ARRAY_01_NULL = "The specified first array cannot be none"sv;
	inline constexpr auto ARRAY_02_NULL = "The specified second array cannot be none"sv;

	inline constexpr auto ACTIVATOR_NULL = "The specified activator cannot be none"sv;
	inline constexpr auto ACTIVE_EFFECT_NULL = "The specified active effect cannot be none"sv;
	inline constexpr auto ACTOR_BASE_NULL = "The specified base actor cannot be none"sv;
	inline constexpr auto ACTOR_NULL = "The specified actor cannot be none"sv;
	inline constexpr auto AIM_MODEL_NULL = "The specified aim model cannot be none"sv;
	inline constexpr auto ALIAS_NULL = "The specified alias cannot be none"sv;
	inline constexpr auto AMMO_NULL = "The specified ammo cannot be none"sv;
	inline constexpr auto ARMOR_ADDON_NULL = "The specified armor addon cannot be none"sv;
	inline constexpr auto BOOK_NULL = "The specified book cannot be none"sv;
	inline constexpr auto BOUND_OBJECT_NULL = "The specified bound object cannot be none"sv;
	inline constexpr auto CELL_NULL = "The specified cell cannot be none"sv;
	inline constexpr auto CLIMATE_NULL = "The specified climate cannot be none"sv;
	inline constexpr auto COLOR_FORM_NULL = "The specified color form cannot be none"sv;
	inline constexpr auto COMPONENT_NULL = "The specified component cannot be none"sv;
	inline constexpr auto CONSTRUCTIBLE_OBJECT_NULL = "The specified constructible object cannot be none"sv;
	inline constexpr auto CONTAINER_NULL = "The specified container cannot be none"sv;
	inline constexpr auto DAMAGE_TYPE_NULL = "The specified damage type cannot be none"sv;
	inline constexpr auto DOOR_NULL = "The specified door cannot be none"sv;
	inline constexpr auto ENCHANTMENT_NULL = "The specified enchantment cannot be none"sv;
	inline constexpr auto ENCOUNTER_ZONE_NULL = "The specified encounter zone cannot be none"sv;
	inline constexpr auto EXPLOSION_NULL = "The specified explosion cannot be none"sv;
	inline constexpr auto FACTION_NULL = "The specified faction cannot be none"sv;
	inline constexpr auto FLORA_NULL = "The specified flora cannot be none"sv;
	inline constexpr auto FORM_LIST_NULL = "The specified form list cannot be none"sv;
	inline constexpr auto FORM_NULL = "The specified form cannot be none"sv;
	inline constexpr auto GLOBAL_VARIABLE_NULL = "The specified global variable cannot be none"sv;
	inline constexpr auto HEAD_PART_NULL = "The specified head part cannot be none"sv;
	inline constexpr auto HOLOTAPE_NULL = "The specified holotape cannot be none"sv;
	inline constexpr auto IDLE_NULL = "The specified idle cannot be none"sv;
	inline constexpr auto INPUT_ENABLE_LAYER_NULL = "The specified input enable layer cannot be none"sv;
	inline constexpr auto INSTANCE_NAMING_RULE_NULL = "The specified instance naming rule cannot be none"sv;
	inline constexpr auto KEYWORD_NULL = "The specified keyword cannot be none"sv;
	inline constexpr auto LEVELED_LIST_NULL = "The specified leveled list cannot be none"sv;
	inline constexpr auto LIGHT_NULL = "The specified light cannot be none"sv;
	inline constexpr auto LOCATION_NULL = "The specified location cannot be none"sv;
	inline constexpr auto MAGIC_EFFECT_NULL = "The specified magic effect cannot be none"sv;
	inline constexpr auto MAGIC_ITEM_NULL = "The specified magic item cannot be none"sv;
	inline constexpr auto MESSAGE_NULL = "The specified message cannot be none"sv;
	inline constexpr auto MISC_OBJECT_NULL = "The specified misc object cannot be none"sv;
	inline constexpr auto MOVEMENT_TYPE_NULL = "The specified movement type cannot be none"sv;
	inline constexpr auto OBJECT_REFERENCE_NULL = "The specified object reference cannot be none"sv;
	inline constexpr auto OUTFIT_NULL = "The specified outfit cannot be none"sv;
	inline constexpr auto PERK_NULL = "The specified perk cannot be none"sv;
	inline constexpr auto POTION_NULL = "The specified potion cannot be none"sv;
	inline constexpr auto PROJECTILE_NULL = "The specified projectile cannot be none"sv;
	inline constexpr auto QUEST_NULL = "The specified outfit cannot be none"sv;
	inline constexpr auto SOUND_NULL = "The specified sound cannot be none"sv;
	inline constexpr auto SPELL_NULL = "The specified spell cannot be none"sv;
	inline constexpr auto WORLD_SPACE_NULL = "The specified world space cannot be none"sv;

	inline constexpr auto START_INDEX_NEGATIVE = "The specified start index cannot be negative"sv;
	inline constexpr auto COUNT_NEGATIVE = "The specified count cannot be negative"sv;
	inline constexpr auto SIZE_NEGATIVE = "The specified size cannot be negative"sv;
	inline constexpr auto TOTAL_SIZE_NEGATIVE = "The specified total size cannot be negative"sv;
	inline constexpr auto MINIMUM_GREATER_THAN_MAXIMUM = "The specified minimum value cannot be greater than the maximum one"sv;

	inline constexpr auto TYPE_ID_OUT_OF_RANGE = "The specified type ID {} is out of range ({} to {})"sv;
	inline constexpr auto COUNT_OUT_OF_RANGE = "The specified count {} is out of range ({} to {})"sv;
	inline constexpr auto INDEX_OUT_OF_RANGE = "The specified index {} is out of range ({} to {})"sv;
	inline constexpr auto START_INDEX_OUT_OF_RANGE = "The specified start index {} is out of range ({} to {})"sv;
	inline constexpr auto DECIMALS_OUT_OF_RANGE = "The specified decimal count {} is out of range ({} to {})"sv;
};
