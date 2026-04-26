#include "Plugin/Papyrus/Input.hpp"

namespace Plugin::Papyrus::Input
{
	static RE::INPUT_DEVICE GetDeviceType_Keyboard(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::INPUT_DEVICE::kKeyboard; }
	static RE::INPUT_DEVICE GetDeviceType_Mouse(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::INPUT_DEVICE::kMouse; }
	static RE::INPUT_DEVICE GetDeviceType_Gamepad(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RE::INPUT_DEVICE::kGamepad; }

	static bool IsGamepadConnected(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return F4SE::Inputs::IsGamepadConnected();
	}

	static bool IsKeyCodeValid(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_keyCode)
	{
		return F4SE::Inputs::IsKeyCodeValid(a_keyCode);
	}

	static bool IsControlNameValid(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_controlName,
		RE::INPUT_DEVICE a_deviceType)
	{
		const auto keyCode = F4SE::Inputs::GetKeyCodeByControlName(a_controlName, a_deviceType);
		return keyCode.has_value();
	}

	static RE::BSFixedString GetControlNameByKeyCode(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_keyCode)
	{
		auto result = F4SE::Inputs::GetControlNameByKeyCode(a_keyCode);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return {};
		}

		return std::move(result).value();
	}

	static std::uint32_t GetKeyCodeByControlName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_controlName,
		RE::INPUT_DEVICE a_deviceType)
	{
		const auto result = F4SE::Inputs::GetKeyCodeByControlName(a_controlName, a_deviceType);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return std::numeric_limits<std::uint32_t>::max();
		}

		return result.value();
	}

	static std::optional<std::vector<std::uint32_t>> GetPressedKeyCodes(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/)
	{
		auto result = F4SE::Inputs::GetPressedKeyCodes();
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return std::nullopt;
		}

		return std::move(result).value();
	}

	static std::optional<std::vector<RE::BSFixedString>> GetPressedControlNames(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::INPUT_DEVICE a_deviceType)
	{
		auto result = F4SE::Inputs::GetPressedControlNames(a_deviceType);
		if (!result.has_value()) [[unlikely]] {
			a_vm.PostError(result.error().message(), a_stackId);
			return std::nullopt;
		}

		return std::move(result).value();
	}

	static bool IsKeyPressed(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_keyCode)
	{
		return F4SE::Inputs::IsKeyPressed(a_keyCode);
	}

	static bool HoldKey(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_keyCode)
	{
		return F4SE::Inputs::HoldKey(a_keyCode);
	}

	static bool ReleaseKey(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_keyCode)
	{
		return F4SE::Inputs::ReleaseKey(a_keyCode);
	}

	static bool TapKey(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_keyCode)
	{
		return F4SE::Inputs::TapKey(a_keyCode);
	}

	static bool TapKeyForSeconds(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_keyCode,
		REX::Float32 a_durationSeconds)
	{
		return F4SE::Inputs::TapKey(a_keyCode, a_durationSeconds);
	}

	static bool IsControlPressed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_controlName,
		RE::INPUT_DEVICE a_deviceType)
	{
		const auto keyCode = F4SE::Inputs::GetKeyCodeByControlName(a_controlName, a_deviceType);
		if (!keyCode.has_value()) [[unlikely]] {
			a_vm.PostError(keyCode.error().message(), a_stackId);
			return false;
		}

		return F4SE::Inputs::IsKeyPressed(keyCode.value());
	}

	static bool HoldControl(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_controlName,
		RE::INPUT_DEVICE a_deviceType)
	{
		const auto keyCode = F4SE::Inputs::GetKeyCodeByControlName(a_controlName, a_deviceType);
		if (!keyCode.has_value()) [[unlikely]] {
			a_vm.PostError(keyCode.error().message(), a_stackId);
			return false;
		}

		return F4SE::Inputs::HoldKey(keyCode.value());
	}

	static bool ReleaseControl(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_controlName,
		RE::INPUT_DEVICE a_deviceType)
	{
		const auto keyCode = F4SE::Inputs::GetKeyCodeByControlName(a_controlName, a_deviceType);
		if (!keyCode.has_value()) [[unlikely]] {
			a_vm.PostError(keyCode.error().message(), a_stackId);
			return false;
		}

		return F4SE::Inputs::ReleaseKey(keyCode.value());
	}

	static bool TapControl(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_controlName,
		RE::INPUT_DEVICE a_deviceType)
	{
		const auto keyCode = F4SE::Inputs::GetKeyCodeByControlName(a_controlName, a_deviceType);
		if (!keyCode.has_value()) [[unlikely]] {
			a_vm.PostError(keyCode.error().message(), a_stackId);
			return false;
		}

		return F4SE::Inputs::TapKey(keyCode.value());
	}

	static bool TapControlForSeconds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_controlName,
		REX::Float32 a_durationSeconds,
		RE::INPUT_DEVICE a_deviceType)
	{
		const auto keyCode = F4SE::Inputs::GetKeyCodeByControlName(a_controlName, a_deviceType);
		if (!keyCode.has_value()) [[unlikely]] {
			a_vm.PostError(keyCode.error().message(), a_stackId);
			return false;
		}

		return F4SE::Inputs::TapKey(keyCode.value(), a_durationSeconds);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeviceType_Keyboard);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeviceType_Mouse);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeviceType_Gamepad);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsGamepadConnected);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsKeyCodeValid);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsControlNameValid);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetControlNameByKeyCode);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetKeyCodeByControlName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPressedKeyCodes);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPressedControlNames);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsKeyPressed);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, HoldKey);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ReleaseKey);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, TapKey);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, TapKeyForSeconds);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsControlPressed);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, HoldControl);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ReleaseControl);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, TapControl);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, TapControlForSeconds);
	}
}
