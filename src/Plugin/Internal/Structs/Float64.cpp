#include "Plugin/Internal/Structs/Float64.hpp"

namespace RE::BSScript
{
	static constexpr auto LOW_PART_KEY = "iLowPart"sv;
	static constexpr auto HIGH_PART_KEY = "iHighPart"sv;

	auto variable_packer<::Plugin::Internal::Structs::Float64::DoubleData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<Plugin::Internal::Structs::Float64::Double>();
	}

	void variable_packer<::Plugin::Internal::Structs::Float64::DoubleData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Float64::DoubleData& a_val)
	{
		auto doubleStruct = ::Plugin::Internal::Structs::Float64::Double::Create();
		REX::Assert(static_cast<bool>(doubleStruct));

		const auto structLock = doubleStruct.Lock();
		doubleStruct.Insert_NoLock(LOW_PART_KEY, a_val.lowPart);
		doubleStruct.Insert_NoLock(HIGH_PART_KEY, a_val.highPart);

		RE::BSScript::PackVariable(a_var, std::move(doubleStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Float64::DoubleData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Float64::DoubleData
	{
		const auto doubleStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Float64::Double>(a_var);
		if (!doubleStruct) {
			return {};
		}

		const auto structLock = doubleStruct.Lock();
		const auto lowPart = doubleStruct.FindOrDefault_NoLock<std::uint32_t>(LOW_PART_KEY);
		const auto highPart = doubleStruct.FindOrDefault_NoLock<std::int32_t>(HIGH_PART_KEY);

		return {
			.lowPart = lowPart,
			.highPart = highPart
		};
	}
}