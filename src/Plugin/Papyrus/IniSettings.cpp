#include "Plugin/Papyrus/IniSettings.hpp"

#include "Plugin/Internal/Game/GameIniSettings.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Colors.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::IniSettings
{
	using ColorStruct = ::Plugin::Internal::Structs::Colors::ColorData;
	using SettingType = RE::Setting::SETTING_TYPE;

	static std::vector<RE::BSFixedString> GetSettings(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSettings();
	}

	static std::vector<RE::BSFixedString> GetSections(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSections();
	}

	static bool ContainsSetting(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->ContainsSetting(a_setting);
	}

	static bool ContainsSection(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_section)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->ContainsSection(a_section);
	}

	static bool GetBool(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		bool a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting || !setting->IsBinary()) {
			return a_default;
		}

		return setting->GetBinary();
	}

	static bool SetBool(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		bool a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting || !setting->IsBinary()) {
			return false;
		}

		setting->SetBinary(a_value);
		return true;
	}

	static char GetChar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		char a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting) {
			return a_default;
		}

		switch (setting->GetType()) {
			case SettingType::kChar: {
				return setting->GetChar();
			}
			case SettingType::kUChar: {
				return static_cast<char>(setting->GetUChar());
			}
			default: {
				return a_default;
			}
		}
	}

	static bool SetChar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		char a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting) {
			return false;
		}

		switch (setting->GetType()) {
			case SettingType::kChar: {
				setting->SetChar(a_value);
				return true;
			}
			case SettingType::kUChar: {
				setting->SetUChar(static_cast<std::uint8_t>(a_value));
				return true;
			}
			default: {
				return false;
			}
		}
	}

	static std::int32_t GetInt(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		std::int32_t a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting) {
			return a_default;
		}

		switch (setting->GetType()) {
			case SettingType::kInt: {
				return setting->GetInt();
			}
			case SettingType::kUInt: {
				return static_cast<std::int32_t>(setting->GetUInt());
			}
			default: {
				return a_default;
			}
		}
	}

	static bool SetInt(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		std::int32_t a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting) {
			return false;
		}

		switch (setting->GetType()) {
			case SettingType::kInt: {
				setting->SetInt(a_value);
				return true;
			}
			case SettingType::kUInt: {
				setting->SetUInt(static_cast<std::uint32_t>(a_value));
				return true;
			}
			default: {
				return false;
			}
		}
	}

	static REX::Float32 GetFloat(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		REX::Float32 a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting || !setting->IsFloat()) {
			return a_default;
		}

		return setting->GetFloat();
	}

	static bool SetFloat(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		REX::Float32 a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting || !setting->IsFloat()) {
			return false;
		}

		setting->SetFloat(a_value);
		return true;
	}

	static RE::BSFixedString GetString(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		RE::BSFixedString a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting || !setting->IsString()) {
			return a_default;
		}

		return setting->GetString();
	}

	static bool SetString(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		RE::BSFixedString a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting || !setting->IsString()) {
			return false;
		}

		setting->SetString(a_value);
		return true;
	}

	static std::optional<ColorStruct> GetColor(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting) {
			return std::nullopt;
		}

		switch (setting->GetType()) {
			case SettingType::kRGB: {
				const auto rgb = setting->GetRGB();
				return ColorStruct{
					.red = rgb[0],
					.green = rgb[1],
					.blue = rgb[2]
				};
			}
			case SettingType::kRGBA: {
				const auto rgba = setting->GetRGBA();
				return ColorStruct{
					.red = rgba[0],
					.green = rgba[1],
					.blue = rgba[2],
					.alpha = rgba[3]
				};
			}
			default: {
				return std::nullopt;
			}
		}
	}

	static bool SetColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting,
		std::optional<ColorStruct> a_value)
	{
		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting) {
			return false;
		}

		switch (setting->GetType()) {
			case SettingType::kRGB: {
				const auto rgb = std::array{ a_value->red, a_value->green, a_value->blue };
				setting->SetRGB(rgb);
				break;
			}
			case SettingType::kRGBA: {
				const auto rgba = std::array{ a_value->red, a_value->green, a_value->blue, a_value->alpha };
				setting->SetRGBA(rgba);
				break;
			}
			default: {
				return false;
			}
		}

		return true;
	}

	static bool SaveSetting(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_setting)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetIniSettings()->GetSetting(a_setting);
		if (!setting) {
			return false;
		}

		return Plugin::Internal::Game::GameIniSettings::SaveSettingAsync(setting);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSettings);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSections);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsSetting);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsSection);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetBool);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetBool);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetChar);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetChar);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetInt);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetInt);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFloat);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetFloat);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetColor);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveSetting);
	}
}
