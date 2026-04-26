#include "Plugin/Papyrus/GameSettings.hpp"

#include "Plugin/Internal/Game/GameInternalSettings.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::GameSettings
{
	static std::vector<RE::BSFixedString> GetKeys(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSortedKeys();
	}

	static bool Contains(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key)
	{
		return ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->ContainsSetting(a_key);
	}

	static bool GetBool(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		bool a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSetting(a_key);
		if (!setting || !setting->IsBinary()) {
			return a_default;
		}

		return setting->GetBinary();
	}

	static bool SetBool(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		bool a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSetting(a_key);
		if (!setting || !setting->IsBinary()) {
			return false;
		}

		setting->SetBinary(a_value);
		return true;
	}

	static std::int32_t GetInt(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		std::int32_t a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSetting(a_key);
		if (!setting) {
			return a_default;
		}

		switch (setting->GetType()) {
			case RE::Setting::SETTING_TYPE::kInt: {
				return setting->GetInt();
			}
			case RE::Setting::SETTING_TYPE::kUInt: {
				return static_cast<std::int32_t>(setting->GetUInt());
			}
			default: {
				return a_default;
			}
		}
	}

	static bool SetInt(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		std::int32_t a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSetting(a_key);
		if (!setting) {
			return false;
		}

		switch (setting->GetType()) {
			case RE::Setting::SETTING_TYPE::kInt: {
				setting->SetInt(a_value);
				return true;
			}
			case RE::Setting::SETTING_TYPE::kUInt: {
				setting->SetUInt(static_cast<std::uint32_t>(a_value));
				return true;
			}
			default: {
				return false;
			}
		}
	}

	static REX::Float32 GetFloat(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		REX::Float32 a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSetting(a_key);
		if (!setting || !setting->IsFloat()) {
			return a_default;
		}

		return setting->GetFloat();
	}

	static bool SetFloat(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		REX::Float32 a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSetting(a_key);
		if (!setting || !setting->IsFloat()) {
			return false;
		}

		setting->SetFloat(a_value);
		return true;
	}

	static RE::BSFixedString GetString(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		RE::BSFixedString a_default)
	{
		const auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSetting(a_key);
		if (!setting || !setting->IsString()) {
			return a_default;
		}

		return setting->GetString();
	}

	static bool SetString(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_key,
		RE::BSFixedString a_value)
	{
		auto* setting = ::Plugin::GetInternalManager()->GetGameManager()->GetInternalSettings()->GetSetting(a_key);
		if (!setting || !setting->IsString()) {
			return false;
		}

		setting->SetString(a_value);
		return true;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Contains);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetKeys);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetBool);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetBool);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetInt);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetInt);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetFloat);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetFloat);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetString);
	}
}
