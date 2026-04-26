#include "Plugin/Papyrus/Forms/MovementType.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::MovementType
{
	using MaxSpeeds = RE::Movement::MaxSpeeds::SpeedType;
	using SpeedDirection = RE::Movement::SPEED_DIRECTION;
}

namespace Plugin::Papyrus::Forms::MovementType::Impl
{
	static constexpr auto STANDING_KEY = "kStanding"sv;
	static constexpr auto WALKING_KEY = "kWalking"sv;
	static constexpr auto RUNNING_KEY = "kRunning"sv;
	static constexpr auto SPRINTING_KEY = "kSprinting"sv;

	[[nodiscard]] static SpeedEntry GetSpeedEntry(const RE::BGSMovementType* a_movementType, SpeedDirection a_direction) noexcept
	{
		REX::Assert(a_movementType != nullptr);

		const auto speeds = std::atomic_ref(a_movementType->movementTypeData.defaultData.speeds[std::to_underlying(a_direction)]).load(std::memory_order_acquire);

		auto entry = SpeedEntry::Create();
		entry.Insert(STANDING_KEY, speeds[std::to_underlying(MaxSpeeds::kStanding)]);
		entry.Insert(WALKING_KEY, speeds[std::to_underlying(MaxSpeeds::kWalking)]);
		entry.Insert(RUNNING_KEY, speeds[std::to_underlying(MaxSpeeds::kRunning)]);
		entry.Insert(SPRINTING_KEY, speeds[std::to_underlying(MaxSpeeds::kSprinting)]);
		return entry;
	}

	static void SetSpeedEntry(RE::BGSMovementType* a_movementType, const SpeedEntry& a_entry, SpeedDirection a_direction) noexcept
	{
		REX::Assert(a_movementType != nullptr);
		REX::Assert(a_entry != nullptr);

		auto speeds = std::array<REX::Float32, std::to_underlying(SpeedDirection::kTotal)>();
		speeds[std::to_underlying(MaxSpeeds::kStanding)] = a_entry.Find<REX::Float32>(STANDING_KEY).value();
		speeds[std::to_underlying(MaxSpeeds::kWalking)] = a_entry.Find<REX::Float32>(WALKING_KEY).value();
		speeds[std::to_underlying(MaxSpeeds::kRunning)] = a_entry.Find<REX::Float32>(RUNNING_KEY).value();
		speeds[std::to_underlying(MaxSpeeds::kSprinting)] = a_entry.Find<REX::Float32>(SPRINTING_KEY).value();

		std::atomic_ref(a_movementType->movementTypeData.defaultData.speeds[std::to_underlying(a_direction)]).store(speeds, std::memory_order_release);
	}
}

namespace Plugin::Papyrus::Forms::MovementType
{
	static RE::BSFixedString GetName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return {};
		}

		return a_movementType->movementTypeData.typeName;
	}

	static void SetName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		RE::BSFixedString a_value)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		a_movementType->movementTypeData.typeName = std::move(a_value);
	}

	static SpeedEntry GetLeftSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return nullptr;
		}

		return Impl::GetSpeedEntry(a_movementType, SpeedDirection::kLeft);
	}

	static void SetLeftSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		SpeedEntry a_entry)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		Impl::SetSpeedEntry(a_movementType, a_entry, SpeedDirection::kLeft);
	}

	static SpeedEntry GetRightSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return nullptr;
		}

		return Impl::GetSpeedEntry(a_movementType, SpeedDirection::kRight);
	}

	static void SetRightSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		SpeedEntry a_entry)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		Impl::SetSpeedEntry(a_movementType, a_entry, SpeedDirection::kRight);
	}

	static SpeedEntry GetForwardSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return nullptr;
		}

		return Impl::GetSpeedEntry(a_movementType, SpeedDirection::kForward);
	}

	static void SetForwardSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		SpeedEntry a_entry)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		Impl::SetSpeedEntry(a_movementType, a_entry, SpeedDirection::kForward);
	}

	static SpeedEntry GetBackwardSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return nullptr;
		}

		return Impl::GetSpeedEntry(a_movementType, SpeedDirection::kBackward);
	}

	static void SetBackwardSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		SpeedEntry a_entry)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		Impl::SetSpeedEntry(a_movementType, a_entry, SpeedDirection::kBackward);
	}

	static SpeedEntry GetPitchSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return nullptr;
		}

		return Impl::GetSpeedEntry(a_movementType, SpeedDirection::kPitch);
	}

	static void SetPitchSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		SpeedEntry a_entry)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		Impl::SetSpeedEntry(a_movementType, a_entry, SpeedDirection::kPitch);
	}

	static SpeedEntry GetRollSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return nullptr;
		}

		return Impl::GetSpeedEntry(a_movementType, SpeedDirection::kRoll);
	}

	static void SetRollSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		SpeedEntry a_entry)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		Impl::SetSpeedEntry(a_movementType, a_entry, SpeedDirection::kRoll);
	}

	static SpeedEntry GetYawSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return nullptr;
		}

		return Impl::GetSpeedEntry(a_movementType, SpeedDirection::kYaw);
	}

	static void SetYawSpeed(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		SpeedEntry a_entry)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		if (!a_entry) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		Impl::SetSpeedEntry(a_movementType, a_entry, SpeedDirection::kYaw);
	}

	static REX::Float32 GetFloatHeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_movementType->movementTypeData.floatHeight).load(std::memory_order_acquire);
	}

	static void SetFloatHeight(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		REX::Float32 a_value)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_movementType->movementTypeData.floatHeight).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetFlightAngleGain(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_movementType->movementTypeData.flightAngleGain).load(std::memory_order_acquire);
	}

	static void SetFlightAngleGain(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSMovementType* a_movementType,
		REX::Float32 a_value)
	{
		if (!a_movementType) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MOVEMENT_TYPE_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_movementType->movementTypeData.flightAngleGain).store(a_value, std::memory_order_release);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLeftSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLeftSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRightSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRightSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetForwardSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetForwardSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBackwardSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetBackwardSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPitchSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPitchSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRollSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRollSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetYawSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetYawSpeed);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFloatHeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFloatHeight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlightAngleGain);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlightAngleGain);
	}
}
