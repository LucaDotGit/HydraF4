#pragma once

namespace Plugin::Internal::Structs::Colors
{
	using Color = RE::BSScript::StructWrapper<"Hydra:Colors", "Color">;

	class ColorData final
	{
	public:
		inline static constexpr auto DEFAULT_ALPHA_VALUE = std::numeric_limits<std::uint8_t>::max();

		[[nodiscard]] constexpr bool operator==(const ColorData&) const noexcept = default;
		[[nodiscard]] constexpr bool operator!=(const ColorData&) const noexcept = default;
		[[nodiscard]] constexpr auto operator<=>(const ColorData&) const noexcept = default;

		[[nodiscard]] static constexpr ColorData FromNiColor(const RE::NiColor& a_color) noexcept
		{
			return {
				.red = static_cast<std::uint8_t>(a_color.r * 255.0_f32),
				.green = static_cast<std::uint8_t>(a_color.g * 255.0_f32),
				.blue = static_cast<std::uint8_t>(a_color.b * 255.0_f32),
				.alpha = DEFAULT_ALPHA_VALUE
			};
		}

		[[nodiscard]] static constexpr RE::NiColor ToNiColor(const ColorData& a_color) noexcept
		{
			return RE::NiColor{
				static_cast<REX::Float32>(a_color.red) / 255.0_f32,
				static_cast<REX::Float32>(a_color.green) / 255.0_f32,
				static_cast<REX::Float32>(a_color.blue) / 255.0_f32
			};
		}

		[[nodiscard]] static constexpr ColorData FromNiColorA(const RE::NiColorA& a_color) noexcept
		{
			return {
				.red = static_cast<std::uint8_t>(a_color.r * 255.0_f32),
				.green = static_cast<std::uint8_t>(a_color.g * 255.0_f32),
				.blue = static_cast<std::uint8_t>(a_color.b * 255.0_f32),
				.alpha = static_cast<std::uint8_t>(a_color.a * 255.0_f32)
			};
		}

		[[nodiscard]] static constexpr RE::NiColorA ToNiColorA(const ColorData& a_color) noexcept
		{
			return RE::NiColorA{
				static_cast<REX::Float32>(a_color.red) / 255.0_f32,
				static_cast<REX::Float32>(a_color.green) / 255.0_f32,
				static_cast<REX::Float32>(a_color.blue) / 255.0_f32,
				static_cast<REX::Float32>(a_color.alpha) / 255.0_f32
			};
		}

		[[nodiscard]] static constexpr ColorData FromHexRgb(std::uint32_t a_value) noexcept
		{
			return {
				.red = static_cast<std::uint8_t>((a_value >> 16) & 0xFF),
				.green = static_cast<std::uint8_t>((a_value >> 8) & 0xFF),
				.blue = static_cast<std::uint8_t>(a_value & 0xFF),
				.alpha = DEFAULT_ALPHA_VALUE
			};
		}

		[[nodiscard]] constexpr std::uint32_t ToHexRgb() const noexcept
		{
			return (static_cast<std::uint32_t>(red) << 16) |
				   (static_cast<std::uint32_t>(green) << 8) |
				   static_cast<std::uint32_t>(blue);
		}

		[[nodiscard]] static constexpr ColorData FromHexRgba(std::uint32_t a_value) noexcept
		{
			return {
				.red = static_cast<std::uint8_t>((a_value >> 24) & 0xFF),
				.green = static_cast<std::uint8_t>((a_value >> 16) & 0xFF),
				.blue = static_cast<std::uint8_t>((a_value >> 8) & 0xFF),
				.alpha = static_cast<std::uint8_t>(a_value & 0xFF)
			};
		}

		[[nodiscard]] constexpr std::uint32_t ToHexRgba() const noexcept
		{
			return (static_cast<std::uint32_t>(red) << 24) |
				   (static_cast<std::uint32_t>(green) << 16) |
				   (static_cast<std::uint32_t>(blue) << 8) |
				   static_cast<std::uint32_t>(alpha);
		}

		[[nodiscard]] static constexpr ColorData FromHexBgr(std::uint32_t a_value) noexcept
		{
			return {
				.red = static_cast<std::uint8_t>(a_value & 0xFF),
				.green = static_cast<std::uint8_t>((a_value >> 8) & 0xFF),
				.blue = static_cast<std::uint8_t>((a_value >> 16) & 0xFF),
				.alpha = DEFAULT_ALPHA_VALUE
			};
		}

		[[nodiscard]] constexpr std::uint32_t ToHexBgr() const noexcept
		{
			return (static_cast<std::uint32_t>(blue) << 16) |
				   (static_cast<std::uint32_t>(green) << 8) |
				   static_cast<std::uint32_t>(red);
		}

		[[nodiscard]] static constexpr ColorData FromHexAbgr(std::uint32_t a_value) noexcept
		{
			return {
				.red = static_cast<std::uint8_t>(a_value & 0xFF),
				.green = static_cast<std::uint8_t>((a_value >> 8) & 0xFF),
				.blue = static_cast<std::uint8_t>((a_value >> 16) & 0xFF),
				.alpha = static_cast<std::uint8_t>((a_value >> 24) & 0xFF)
			};
		}

		[[nodiscard]] constexpr std::uint32_t ToHexAbgr() const noexcept
		{
			return (static_cast<std::uint32_t>(alpha) << 24) |
				   (static_cast<std::uint32_t>(blue) << 16) |
				   (static_cast<std::uint32_t>(green) << 8) |
				   static_cast<std::uint32_t>(red);
		}

		std::uint8_t red{ 0 };
		std::uint8_t green{ 0 };
		std::uint8_t blue{ 0 };
		std::uint8_t alpha{ DEFAULT_ALPHA_VALUE };
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Colors::ColorData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(RE::BSScript::Variable& a_var, const ::Plugin::Internal::Structs::Colors::ColorData& a_val);

		[[nodiscard]] static auto UnpackVariable(const RE::BSScript::Variable& a_var) -> ::Plugin::Internal::Structs::Colors::ColorData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Colors::ColorData>());
}
