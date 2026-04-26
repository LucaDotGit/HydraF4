#pragma once

namespace Plugin::Internal::Structs::Int64
{
	using Long = RE::BSScript::StructWrapper<"Hydra:Int64", "Long">;

	class LongData final
	{
	public:
		[[nodiscard]] constexpr bool operator==(const LongData& a_rhs) const noexcept
		{
			return value == a_rhs.value;
		}

		[[nodiscard]] constexpr bool operator!=(const LongData& a_rhs) const noexcept
		{
			return !(*this == a_rhs);
		}

		[[nodiscard]] constexpr auto operator<=>(const LongData& a_rhs) const noexcept
		{
			return value <=> a_rhs.value;
		}

		[[nodiscard]] static constexpr LongData FromBool(bool a_value) noexcept
		{
			return FromInt64(static_cast<std::int64_t>(a_value));
		}

		[[nodiscard]] static constexpr bool ToBool(const LongData& a_data) noexcept
		{
			return static_cast<bool>(ToInt64(a_data));
		}

		[[nodiscard]] static constexpr LongData FromInt32(std::int32_t a_value) noexcept
		{
			return FromInt64(static_cast<std::int64_t>(a_value));
		}

		[[nodiscard]] static constexpr std::int32_t ToInt32(const LongData& a_data) noexcept
		{
			return static_cast<std::int32_t>(ToInt64(a_data));
		}

		[[nodiscard]] static constexpr LongData FromUInt32(std::uint32_t a_value) noexcept
		{
			return FromInt64(static_cast<std::int64_t>(a_value));
		}

		[[nodiscard]] static constexpr std::uint32_t ToUInt32(const LongData& a_data) noexcept
		{
			return static_cast<std::uint32_t>(ToInt64(a_data));
		}

		[[nodiscard]] static constexpr LongData FromInt64(std::int64_t a_value) noexcept
		{
			const auto&& [lowPart, highPart] = REX::SplitFromX64(a_value);
			return { .lowPart = lowPart, .highPart = highPart };
		}

		[[nodiscard]] static constexpr std::int64_t ToInt64(const LongData& a_data) noexcept
		{
			return REX::CombineToX64<std::int64_t>(a_data.lowPart, a_data.highPart);
		}

		[[nodiscard]] static constexpr LongData FromUInt64(std::uint64_t a_value) noexcept
		{
			return FromInt64(static_cast<std::int64_t>(a_value));
		}

		[[nodiscard]] static constexpr std::uint64_t ToUInt64(const LongData& a_data) noexcept
		{
			return static_cast<std::uint64_t>(ToInt64(a_data));
		}

		[[nodiscard]] static constexpr LongData FromFloat32(REX::Float32 a_value) noexcept
		{
			return FromInt64(static_cast<std::int64_t>(a_value));
		}

		[[nodiscard]] static constexpr REX::Float32 ToFloat32(const LongData& a_data) noexcept
		{
			return static_cast<REX::Float32>(ToInt64(a_data));
		}

		[[nodiscard]] static constexpr LongData FromFloat64(REX::Float64 a_value) noexcept
		{
			return FromInt64(static_cast<std::int64_t>(a_value));
		}

		[[nodiscard]] static constexpr REX::Float64 ToFloat64(const LongData& a_data) noexcept
		{
			return static_cast<REX::Float64>(ToInt64(a_data));
		}

		union
		{
			struct
			{
				std::uint32_t lowPart;
				std::int32_t highPart;
			};
			std::int64_t value;
		};
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Int64::LongData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(RE::BSScript::Variable& a_var, const ::Plugin::Internal::Structs::Int64::LongData& a_val);

		[[nodiscard]] static auto UnpackVariable(const RE::BSScript::Variable& a_var) -> ::Plugin::Internal::Structs::Int64::LongData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Int64::LongData>());
}
