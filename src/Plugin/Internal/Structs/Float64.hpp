#pragma once

namespace Plugin::Internal::Structs::Float64
{
	using Double = RE::BSScript::StructWrapper<"Hydra:Float64", "Double">;

	class DoubleData final
	{
	public:
		[[nodiscard]] constexpr bool operator==(const DoubleData& a_rhs) const noexcept
		{
			return value == a_rhs.value;
		}

		[[nodiscard]] constexpr bool operator!=(const DoubleData& a_rhs) const noexcept
		{
			return !(*this == a_rhs);
		}

		[[nodiscard]] constexpr auto operator<=>(const DoubleData& a_rhs) const noexcept
		{
			return value <=> a_rhs.value;
		}

		[[nodiscard]] static constexpr DoubleData FromBool(bool a_value) noexcept
		{
			return FromFloat64(static_cast<REX::Float64>(a_value));
		}

		[[nodiscard]] static constexpr bool ToBool(const DoubleData& a_data) noexcept
		{
			return static_cast<bool>(ToFloat64(a_data));
		}

		[[nodiscard]] static constexpr DoubleData FromInt32(std::int32_t a_value) noexcept
		{
			return FromFloat64(static_cast<REX::Float64>(a_value));
		}

		[[nodiscard]] static constexpr std::int32_t ToInt32(const DoubleData& a_data) noexcept
		{
			return static_cast<std::int32_t>(ToFloat64(a_data));
		}

		[[nodiscard]] static constexpr DoubleData FromUInt32(std::uint32_t a_value) noexcept
		{
			return FromFloat64(static_cast<REX::Float64>(a_value));
		}

		[[nodiscard]] static constexpr std::uint32_t ToUInt32(const DoubleData& a_data) noexcept
		{
			return static_cast<std::uint32_t>(ToFloat64(a_data));
		}

		[[nodiscard]] static constexpr DoubleData FromInt64(std::int64_t a_value) noexcept
		{
			return FromFloat64(static_cast<REX::Float64>(a_value));
		}

		[[nodiscard]] static constexpr std::int64_t ToInt64(const DoubleData& a_data) noexcept
		{
			return static_cast<std::int64_t>(ToFloat64(a_data));
		}

		[[nodiscard]] static constexpr DoubleData FromUInt64(std::uint64_t a_value) noexcept
		{
			return FromFloat64(static_cast<REX::Float64>(a_value));
		}

		[[nodiscard]] static constexpr std::uint64_t ToUInt64(const DoubleData& a_data) noexcept
		{
			return static_cast<std::uint64_t>(ToFloat64(a_data));
		}

		[[nodiscard]] static constexpr DoubleData FromFloat32(REX::Float32 a_value) noexcept
		{
			return FromFloat64(static_cast<REX::Float64>(a_value));
		}

		[[nodiscard]] static constexpr REX::Float32 ToFloat32(const DoubleData& a_data) noexcept
		{
			return static_cast<REX::Float32>(ToFloat64(a_data));
		}

		[[nodiscard]] static constexpr DoubleData FromFloat64(REX::Float64 a_value) noexcept
		{
			const auto&& [lowPart, highPart] = REX::SplitFromX64(a_value);
			return { .lowPart = lowPart, .highPart = highPart };
		}

		[[nodiscard]] static constexpr REX::Float64 ToFloat64(const DoubleData& a_data) noexcept
		{
			return REX::CombineToX64<REX::Float64>(a_data.lowPart, a_data.highPart);
		}

		union
		{
			struct
			{
				std::uint32_t lowPart;
				std::int32_t highPart;
			};
			REX::Float64 value;
		};
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Float64::DoubleData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Float64::DoubleData& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Float64::DoubleData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Float64::DoubleData>());
}
