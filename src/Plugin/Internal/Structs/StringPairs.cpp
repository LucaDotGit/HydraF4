#include "Plugin/Internal/Structs/StringPairs.hpp"

namespace RE::BSScript
{
	static constexpr auto KEY_KEY = "sKey"sv;
	static constexpr auto VALUE_KEY = "sValue"sv;

	auto variable_packer<::Plugin::Internal::Structs::StringPairs::StringPairData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::StringPairs::StringPair>();
	}

	void variable_packer<::Plugin::Internal::Structs::StringPairs::StringPairData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::StringPairs::StringPairData& a_val)
	{
		auto stringPairStruct = ::Plugin::Internal::Structs::StringPairs::StringPair::Create();
		REX::Assert(static_cast<bool>(stringPairStruct));

		const auto structLock = stringPairStruct.Lock();
		stringPairStruct.Insert_NoLock(KEY_KEY, a_val.first);
		stringPairStruct.Insert_NoLock(VALUE_KEY, a_val.second);

		RE::BSScript::PackVariable(a_var, std::move(stringPairStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::StringPairs::StringPairData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::StringPairs::StringPairData
	{
		auto stringPairStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::StringPairs::StringPair>(a_var);
		if (!stringPairStruct) {
			return {};
		}

		const auto structLock = stringPairStruct.Lock();

		const auto key = stringPairStruct.FindOrDefault_NoLock<RE::BSFixedString>(KEY_KEY);
		const auto value = stringPairStruct.FindOrDefault_NoLock<RE::BSFixedString>(VALUE_KEY);

		return { key, value };
	}
}
