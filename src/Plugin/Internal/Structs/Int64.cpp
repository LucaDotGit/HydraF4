#include "Plugin/Internal/Structs/Int64.hpp"

namespace RE::BSScript
{
	static constexpr auto LOW_PART_KEY = "iLowPart"sv;
	static constexpr auto HIGH_PART_KEY = "iHighPart"sv;

	auto variable_packer<::Plugin::Internal::Structs::Int64::LongData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<Plugin::Internal::Structs::Int64::Long>();
	}

	void variable_packer<::Plugin::Internal::Structs::Int64::LongData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Int64::LongData& a_val)
	{
		auto longStruct = ::Plugin::Internal::Structs::Int64::Long::Create();
		REX::Assert(static_cast<bool>(longStruct));

		const auto structLock = longStruct.Lock();
		longStruct.Insert_NoLock(LOW_PART_KEY, a_val.lowPart);
		longStruct.Insert_NoLock(HIGH_PART_KEY, a_val.highPart);

		RE::BSScript::PackVariable(a_var, std::move(longStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Int64::LongData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Int64::LongData
	{
		const auto longStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Int64::Long>(a_var);
		if (!longStruct) {
			return {};
		}

		const auto structLock = longStruct.Lock();
		const auto lowPart = longStruct.FindOrDefault_NoLock<std::uint32_t>(LOW_PART_KEY);
		const auto highPart = longStruct.FindOrDefault_NoLock<std::int32_t>(HIGH_PART_KEY);

		return {
			.lowPart = lowPart,
			.highPart = highPart
		};
	}
}
