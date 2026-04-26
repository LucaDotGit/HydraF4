#pragma once

namespace Plugin::Internal::Structs::Vectors4
{
	using Vector4 = RE::BSScript::StructWrapper<"Hydra:Vectors4", "Vector4">;

	class Vector4Data final
	{
	public:
		[[nodiscard]] constexpr bool operator==(const Vector4Data&) const noexcept = default;
		[[nodiscard]] constexpr bool operator!=(const Vector4Data&) const noexcept = default;
		[[nodiscard]] constexpr auto operator<=>(const Vector4Data&) const noexcept = default;

		[[nodiscard]] static constexpr Vector4Data FromNiPoint4(const RE::NiPoint4& a_point) noexcept
		{
			return {
				.x = a_point.x,
				.y = a_point.y,
				.z = a_point.z,
				.w = a_point.w
			};
		}

		[[nodiscard]] static constexpr RE::NiPoint4 ToNiPoint4(const Vector4Data& a_vector) noexcept
		{
			return RE::NiPoint4{
				a_vector.x,
				a_vector.y,
				a_vector.z,
				a_vector.w
			};
		}

		REX::Float32 x{ 0.0_f32 };
		REX::Float32 y{ 0.0_f32 };
		REX::Float32 z{ 0.0_f32 };
		REX::Float32 w{ 0.0_f32 };
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Vectors4::Vector4Data>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Vectors4::Vector4Data& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Vectors4::Vector4Data;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Vectors4::Vector4Data>());
}
