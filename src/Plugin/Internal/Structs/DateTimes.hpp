#pragma once

namespace Plugin::Internal::Structs::DateTimes
{
	using DateTime = RE::BSScript::StructWrapper<"Hydra:DateTimes", "DateTime">;

	class DateTimeData final
	{
	public:
		[[nodiscard]] constexpr bool operator==(const DateTimeData&) const noexcept = default;
		[[nodiscard]] constexpr bool operator!=(const DateTimeData&) const noexcept = default;
		[[nodiscard]] constexpr auto operator<=>(const DateTimeData&) const noexcept = default;

		std::uint32_t year{ 1 };
		std::uint32_t month{ 1 };
		std::uint32_t day{ 1 };
		std::uint32_t hour{ 0 };
		std::uint32_t minute{ 0 };
		std::uint32_t second{ 0 };
		std::uint32_t millisecond{ 0 };
		std::uint32_t microsecond{ 0 };
		std::uint32_t nanosecond{ 0 };
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::DateTimes::DateTimeData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(RE::BSScript::Variable& a_var, const ::Plugin::Internal::Structs::DateTimes::DateTimeData& a_val);

		[[nodiscard]] static auto UnpackVariable(const RE::BSScript::Variable& a_var) -> ::Plugin::Internal::Structs::DateTimes::DateTimeData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::DateTimes::DateTimeData>());
}
