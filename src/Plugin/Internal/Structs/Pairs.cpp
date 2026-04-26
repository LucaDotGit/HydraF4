#include "Plugin/Internal/Structs/Pairs.hpp"

namespace RE::BSScript
{
	static constexpr auto KEY_KEY = "sKey"sv;
	static constexpr auto VALUE_KEY = "vValue"sv;

	auto variable_packer<::Plugin::Internal::Structs::Pairs::PairData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::Pairs::Pair>();
	}

	void variable_packer<::Plugin::Internal::Structs::Pairs::PairData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Pairs::PairData& a_val)
	{
		auto pairStruct = ::Plugin::Internal::Structs::Pairs::Pair::Create();
		REX::Assert(static_cast<bool>(pairStruct));

		const auto structLock = pairStruct.Lock();
		pairStruct.Insert_NoLock(KEY_KEY, a_val.first);
		pairStruct.Insert_NoLock(VALUE_KEY, RE::BSScript::CreateVariant(a_val.second));

		RE::BSScript::PackVariable(a_var, std::move(pairStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Pairs::PairData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Pairs::PairData
	{
		auto pairStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Pairs::Pair>(a_var);
		if (!pairStruct) {
			return {};
		}

		const auto structLock = pairStruct.Lock();

		const auto key = pairStruct.FindOrDefault_NoLock<RE::BSFixedString>(KEY_KEY);
		const auto* value = pairStruct.FindOrDefault_NoLock<RE::BSScript::SharedVariant>(VALUE_KEY);

		return { key, RE::BSScript::UnwrapVariant(value) };
	}
}
