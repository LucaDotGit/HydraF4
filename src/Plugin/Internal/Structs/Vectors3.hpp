#pragma once

namespace Plugin::Internal::Structs::Vectors3
{
	using Vector3 = RE::BSScript::StructWrapper<"Hydra:Vectors3", "Vector3">;

	class Vector3Data final
	{
	public:
		[[nodiscard]] constexpr bool operator==(const Vector3Data&) const noexcept = default;
		[[nodiscard]] constexpr bool operator!=(const Vector3Data&) const noexcept = default;
		[[nodiscard]] constexpr auto operator<=>(const Vector3Data&) const noexcept = default;

		[[nodiscard]] static constexpr Vector3Data FromNiPoint3(const RE::NiPoint3& a_point) noexcept
		{
			return {
				.x = a_point.x,
				.y = a_point.y,
				.z = a_point.z
			};
		}

		[[nodiscard]] static constexpr Vector3Data FromNiPoint3A(const RE::NiPoint3A& a_point) noexcept
		{
			return {
				.x = a_point.x,
				.y = a_point.y,
				.z = a_point.z
			};
		}

		[[nodiscard]] static constexpr RE::NiPoint3 ToNiPoint3(const Vector3Data& a_vector) noexcept
		{
			return RE::NiPoint3{
				a_vector.x,
				a_vector.y,
				a_vector.z
			};
		}

		[[nodiscard]] static constexpr RE::NiPoint3A ToNiPoint3A(const Vector3Data& a_vector) noexcept
		{
			return RE::NiPoint3A{
				a_vector.x,
				a_vector.y,
				a_vector.z
			};
		}

		REX::Float32 x{ 0.0_f32 };
		REX::Float32 y{ 0.0_f32 };
		REX::Float32 z{ 0.0_f32 };
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Vectors3::Vector3Data>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Vectors3::Vector3Data& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Vectors3::Vector3Data;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Vectors3::Vector3Data>());
}
