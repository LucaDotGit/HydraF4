#include "Plugin/Internal/Structs/Colors.hpp"

namespace RE::BSScript
{
	static constexpr auto RED_KEY = "iRed"sv;
	static constexpr auto GREEN_KEY = "iGreen"sv;
	static constexpr auto BLUE_KEY = "iBlue"sv;
	static constexpr auto ALPHA_KEY = "iAlpha"sv;

	auto variable_packer<::Plugin::Internal::Structs::Colors::ColorData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::Colors::Color>();
	}

	void variable_packer<::Plugin::Internal::Structs::Colors::ColorData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Colors::ColorData& a_val)
	{
		auto colorStruct = ::Plugin::Internal::Structs::Colors::Color::Create();
		REX::Assert(static_cast<bool>(colorStruct));

		const auto structLock = colorStruct.Lock();
		colorStruct.Insert_NoLock(RED_KEY, a_val.red);
		colorStruct.Insert_NoLock(GREEN_KEY, a_val.green);
		colorStruct.Insert_NoLock(BLUE_KEY, a_val.blue);
		colorStruct.Insert_NoLock(ALPHA_KEY, a_val.alpha);

		RE::BSScript::PackVariable(a_var, std::move(colorStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Colors::ColorData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Colors::ColorData
	{
		auto colorStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Colors::Color>(a_var);
		if (!colorStruct) {
			return {};
		}

		const auto structLock = colorStruct.Lock();

		const auto red = colorStruct.FindOrDefault_NoLock<std::uint8_t>(RED_KEY);
		const auto green = colorStruct.FindOrDefault_NoLock<std::uint8_t>(GREEN_KEY);
		const auto blue = colorStruct.FindOrDefault_NoLock<std::uint8_t>(BLUE_KEY);
		const auto alpha = colorStruct.FindOrDefault_NoLock<std::uint8_t>(ALPHA_KEY);

		return {
			.red = red,
			.green = green,
			.blue = blue,
			.alpha = alpha
		};
	}
}
