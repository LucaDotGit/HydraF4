#pragma once

namespace Plugin::Internal::Structs::Vectors2
{
	using Vector2 = RE::BSScript::StructWrapper<"Hydra:Vectors2", "Vector2">;

	class Vector2Data final
	{
	public:
		[[nodiscard]] constexpr bool operator==(const Vector2Data&) const noexcept = default;
		[[nodiscard]] constexpr bool operator!=(const Vector2Data&) const noexcept = default;
		[[nodiscard]] constexpr auto operator<=>(const Vector2Data&) const noexcept = default;

		[[nodiscard]] static constexpr Vector2Data FromNiPoint2(const RE::NiPoint2& a_point) noexcept
		{
			return {
				.x = a_point.x,
				.y = a_point.y
			};
		}

		[[nodiscard]] static constexpr RE::NiPoint2 ToNiPoint2(const Vector2Data& a_vector) noexcept
		{
			return RE::NiPoint2{
				a_vector.x,
				a_vector.y
			};
		}

		REX::Float32 x{ 0.0_f32 };
		REX::Float32 y{ 0.0_f32 };
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Vectors2::Vector2Data>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Vectors2::Vector2Data& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Vectors2::Vector2Data;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Vectors2::Vector2Data>());
}
