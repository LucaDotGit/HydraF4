#pragma once

#include "Plugin/Internal/Events/IEventSource.hpp"

namespace Plugin::Internal::Events
{
	class ButtonUpDownEvent final
	{
	public:
		[[nodiscard]] constexpr bool IsDown() const noexcept { return IsPressed() && !IsRepeating(); }
		[[nodiscard]] constexpr bool IsUp() const noexcept { return !IsPressed() && IsRepeating(); }
		[[nodiscard]] constexpr bool IsPressed() const noexcept { return analogValue > 0.0_f32; }
		[[nodiscard]] constexpr bool IsRepeating() const noexcept { return heldSeconds > 0.0_f32; }
		[[nodiscard]] constexpr bool IsHeldDown(REX::Float32 a_heldDownSecs) const noexcept { return IsPressed() && a_heldDownSecs <= heldSeconds; }
		[[nodiscard]] constexpr bool IsReleased(REX::Float32 a_heldDownSecs) const noexcept { return !IsPressed() && a_heldDownSecs <= heldSeconds; }

		REX::Enum<RE::INPUT_DEVICE> deviceType;
		REX::Enum<RE::BS_BUTTON_CODE> buttonCode;
		RE::BSFixedString controlName;
		REX::Float32 analogValue;
		REX::Float32 heldSeconds;
	};

	extern template class IEventSource<ButtonUpDownEvent>;
	using ButtonUpDownSource = IEventSource<ButtonUpDownEvent>;
}
