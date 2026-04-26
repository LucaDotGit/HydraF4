#include "Plugin/Papyrus/Translations.hpp"

#include "Plugin/Internal/Script/ScriptStrictString.hpp"

namespace Plugin::Papyrus::Translations
{
	static bool Contains(RE::BSScript::StaticTag /*a_staticTag*/,
		::Plugin::Internal::Script::ScriptStrictString a_key)
	{
		return F4SE::Translations::GetTranslator()->Contains(static_cast<std::string_view>(a_key.GetValue()));
	}

	static std::string GetValue(RE::BSScript::StaticTag /*a_staticTag*/,
		::Plugin::Internal::Script::ScriptStrictString a_key,
		::Plugin::Internal::Script::ScriptStrictString a_default)
	{
		auto result = F4SE::Translations::GetTranslator()->GetValue(static_cast<std::string_view>(a_key.GetValue()));
		if (!result.has_value()) {
			return std::string(a_default.GetValue());
		}

		return std::move(result).value();
	}

	static std::string_view GetLanguageCode(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::Translations::GetTranslator()->GetLanguageCode();
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, Contains);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetValue);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLanguageCode);
	}
}
