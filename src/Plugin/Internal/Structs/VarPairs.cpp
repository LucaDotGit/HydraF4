#include "Plugin/Internal/Structs/VarPairs.hpp"

namespace RE::BSScript
{
	static constexpr auto KEY_KEY = "vKey"sv;
	static constexpr auto VALUE_KEY = "vValue"sv;

	auto variable_packer<::Plugin::Internal::Structs::VarPairs::VarPairData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::VarPairs::VarPair>();
	}

	void variable_packer<::Plugin::Internal::Structs::VarPairs::VarPairData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::VarPairs::VarPairData& a_val)
	{
		auto varPairStruct = ::Plugin::Internal::Structs::VarPairs::VarPair::Create();
		REX::Assert(static_cast<bool>(varPairStruct));

		const auto structLock = varPairStruct.Lock();
		varPairStruct.Insert_NoLock(KEY_KEY, RE::BSScript::CreateVariant(a_val.first));
		varPairStruct.Insert_NoLock(VALUE_KEY, RE::BSScript::CreateVariant(a_val.second));

		RE::BSScript::PackVariable(a_var, std::move(varPairStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::VarPairs::VarPairData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::VarPairs::VarPairData
	{
		auto varPairStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::VarPairs::VarPair>(a_var);
		if (!varPairStruct) {
			return {};
		}

		const auto structLock = varPairStruct.Lock();

		const auto* key = varPairStruct.FindOrDefault_NoLock<RE::BSScript::SharedVariant>(KEY_KEY);
		const auto* value = varPairStruct.FindOrDefault_NoLock<RE::BSScript::SharedVariant>(VALUE_KEY);

		return { RE::BSScript::UnwrapVariant(key), RE::BSScript::UnwrapVariant(value) };
	}
}
