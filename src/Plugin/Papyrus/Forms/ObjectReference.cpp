#include "Plugin/Papyrus/Forms/ObjectReference.hpp"

// NOLINTBEGIN(bugprone-suspicious-include)
#include "Plugin/Papyrus/Forms/ObjectReference_Extra.cpp"
// NOLINTEND(bugprone-suspicious-include)

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Vectors3.hpp"

namespace Plugin::Papyrus::Forms::ObjectReference::Impl
{
	[[nodiscard]] __forceinline static bool ValidateSetPosition(RE::TESObjectREFR* a_ref, REX::Float32 a_oldPosition, REX::Float32 a_newPosition)
	{
		REX::Assert(a_ref != nullptr);

		return !a_ref->IsPlayerRef() || std::abs(a_oldPosition - a_newPosition) <= RE::GridCellArray::CELL_SIZE;
	}

	[[nodiscard]] __forceinline static bool ValidateModPosition(RE::TESObjectREFR* a_ref, REX::Float32 a_newPosition)
	{
		REX::Assert(a_ref != nullptr);

		return !a_ref->IsPlayerRef() || std::abs(a_newPosition) <= RE::GridCellArray::CELL_SIZE;
	}

	static void UpdatePosition(RE::TESObjectREFR* a_ref)
	{
		REX::Assert(a_ref != nullptr);

		F4SE::GetTaskInterface()->AddTask([refHandle = a_ref->GetHandle()]() {
			if (!refHandle) {
				return;
			}

			constexpr auto WARP = true;
			refHandle->Update3DPosition(WARP);
		});
	}
}

namespace Plugin::Papyrus::Forms::ObjectReference
{
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;

	using RecordFlags = RE::TESObjectREFR::RecordFlags;

	static RE::TESBoundObject* GetBaseObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_ref->data.objectReference).load(std::memory_order_acquire);
	}

	static void SetBaseObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::TESForm* a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return;
		}

		if (!a_value) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::FORM_NULL, a_stackId);
			return;
		}

		auto* baseObject = RE::DynamicCast<RE::TESBoundObject*>(a_value);
		if (!baseObject) [[unlikely]] {
			a_vm.PostError("The specified base object is not a bound object"sv, a_stackId);
			return;
		}

		a_ref->SetObjectReference(baseObject);
	}

	static RecordFlags GetRecordFlag_IsCollisionDisabled(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RecordFlags::kCollisionsDisabled; }
	static RecordFlags GetRecordFlag_IsInitiallyDead(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RecordFlags::kStartsDead; }
	static RecordFlags GetRecordFlag_IsInitiallyUnconscious(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RecordFlags::kStartUnconscious; }
	static RecordFlags GetRecordFlag_IsPersistent(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RecordFlags::kPersistent; }
	static RecordFlags GetRecordFlag_IsInitiallyDisabled(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RecordFlags::kInitiallyDisabled; }
	static RecordFlags GetRecordFlag_DoesRespawn(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RecordFlags::kRespawns; }
	static RecordFlags GetRecordFlag_IsHarvested(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RecordFlags::kHarvested; }
	static RecordFlags GetRecordFlag_IsFullLod(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RecordFlags::kIsFullLOD; }

	static REX::Float32 GetPositionX(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_ref->GetPositionX();
	}

	static bool SetPositionX(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto oldPosition = a_ref->GetPositionX();
		if (!Impl::ValidateSetPosition(a_ref, oldPosition, a_value)) {
			return false;
		}

		a_ref->SetPositionX(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static bool ModPositionX(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		if (!Impl::ValidateModPosition(a_ref, a_value)) {
			return false;
		}

		a_ref->ModPositionX(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static REX::Float32 GetPositionY(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_ref->GetPositionY();
	}

	static bool SetPositionY(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto oldPosition = a_ref->GetPositionY();
		if (!Impl::ValidateSetPosition(a_ref, oldPosition, a_value)) {
			return false;
		}

		a_ref->SetPositionY(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static bool ModPositionY(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		if (!Impl::ValidateModPosition(a_ref, a_value)) {
			return false;
		}

		a_ref->ModPositionY(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static REX::Float32 GetPositionZ(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_ref->GetPositionZ();
	}

	static bool SetPositionZ(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto oldPosition = a_ref->GetPositionZ();
		if (!Impl::ValidateSetPosition(a_ref, oldPosition, a_value)) {
			return false;
		}

		a_ref->SetPositionZ(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static bool ModPositionZ(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		if (!Impl::ValidateModPosition(a_ref, a_value)) {
			return false;
		}

		a_ref->ModPositionZ(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static REX::Float32 GetRotationX(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_ref->GetAngleX();
	}

	static bool SetRotationX(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		a_ref->SetAngleX(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static bool ModRotationX(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		a_ref->ModAngleX(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static REX::Float32 GetRotationY(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_ref->GetAngleY();
	}

	static bool SetRotationY(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		a_ref->SetAngleY(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static bool ModRotationY(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		a_ref->ModAngleY(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static REX::Float32 GetRotationZ(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_ref->GetAngleZ();
	}

	static bool SetRotationZ(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		a_ref->SetAngleZ(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static bool ModRotationZ(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		REX::Float32 a_value)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		a_ref->ModAngleZ(a_value);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static std::optional<Vector3Struct> GetPosition(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		return Vector3Struct::FromNiPoint3(a_ref->GetPosition());
	}

	static bool SetPosition(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		std::optional<Vector3Struct> a_position)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		if (!a_position.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto oldPosition = a_ref->GetPosition();
		const auto newPosition = Vector3Struct::ToNiPoint3A(a_position.value());

		if (!Impl::ValidateSetPosition(a_ref, oldPosition.x, newPosition.x) ||
			!Impl::ValidateSetPosition(a_ref, oldPosition.y, newPosition.y) ||
			!Impl::ValidateSetPosition(a_ref, oldPosition.z, newPosition.z)) {
			return false;
		}

		a_ref->SetPosition(newPosition);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static std::optional<Vector3Struct> GetRotation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		return Vector3Struct::FromNiPoint3(a_ref->GetAngle());
	}

	static bool SetRotation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		std::optional<Vector3Struct> a_rotation)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		if (!a_rotation.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto newRotation = Vector3Struct::ToNiPoint3A(a_rotation.value());

		a_ref->SetAngle(newRotation);
		Impl::UpdatePosition(a_ref);

		return true;
	}

	static REX::Float32 GetHeadingRotation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::TESObjectREFR* a_targetRef)
	{
		if (!a_ref || !a_targetRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return 0.0_f32;
		}

		return a_ref->GetHeadingAngle(a_targetRef->GetPosition());
	}

	static bool SetHeadingRotation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::TESObjectREFR* a_targetRef,
		REX::Float32 a_offset)
	{
		if (!a_ref || !a_targetRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		a_ref->SetHeadingAngle(a_targetRef->GetPosition(), a_offset);
		return true;
	}

	static bool MoveAround(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::TESObjectREFR* a_targetRef,
		REX::Float32 a_distance,
		REX::Float32 a_angle,
		REX::Float32 a_heightOffset,
		REX::Float32 a_rotationOffset,
		bool a_faceTarget,
		bool a_retainRotation)
	{
		if (!a_ref || !a_targetRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		if (!Impl::ValidateModPosition(a_ref, a_distance)) {
			return false;
		}

		constexpr auto CORRECTION_ANGLE = 90.0_f32;

		const auto targetAngleZ = a_targetRef->GetAngleZ();
		const auto angleZ = targetAngleZ + a_angle - CORRECTION_ANGLE;
		const auto radians = REX::DegToRad(angleZ);

		const auto directionX = std::cos(radians);
		const auto directionY = std::sin(radians);

		const auto offsetX = a_distance * directionX;
		const auto offsetY = -(a_distance * directionY);

		const auto x = a_targetRef->GetPositionX() + offsetX;
		const auto y = a_targetRef->GetPositionY() + offsetY;
		const auto z = a_targetRef->GetPositionZ() + a_heightOffset;

		const auto position = RE::NiPoint3A{ x, y, z };
		a_ref->SetPosition(position);

		if (a_faceTarget) {
			a_ref->SetHeadingAngle(a_targetRef->GetPosition(), a_angle);
		}
		else if (a_rotationOffset != 0.0_f32) {
			a_ref->SetAngleZ(targetAngleZ + a_rotationOffset);
		}

		if (!a_retainRotation) {
			a_ref->SetAngleX(0.0_f32);
			a_ref->SetAngleY(0.0_f32);
		}

		Impl::UpdatePosition(a_ref);
		return true;
	}

	static RE::TESObjectCELL* GetCurrentCell(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_ref->parentCell).load(std::memory_order_acquire);
	}

	static RE::BGSLocation* GetCurrentLocation(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		return a_ref->GetCurrentLocation();
	}

	static RE::BGSLocation* GetLocationByMapMarker(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_markerRef)
	{
		if (!a_markerRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		const auto& locations = dataHandler->GetFormArray<RE::BGSLocation>();
		for (auto* location : locations) {
			if (!location) {
				continue;
			}

			const auto markerHandle = location->worldLocMarker;
			if (markerHandle && markerHandle == a_markerRef) {
				return location;
			}
		}

		return nullptr;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetBaseObject);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetBaseObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsCollisionDisabled);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsInitiallyDead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsInitiallyUnconscious);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsPersistent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsInitiallyDisabled);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_DoesRespawn);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsHarvested);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsFullLod);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPositionX);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPositionX);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ModPositionX);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPositionY);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPositionY);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ModPositionY);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPositionZ);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPositionZ);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ModPositionZ);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRotationX);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetRotationX);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ModRotationX);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRotationY);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetRotationY);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ModRotationY);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRotationZ);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetRotationZ);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ModRotationZ);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetPosition);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetPosition);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRotation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetRotation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetHeadingRotation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetHeadingRotation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, MoveAround);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentCell);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetCurrentLocation);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetLocationByMapMarker);

		ObjectReference::Extra::RegisterFunctions(a_vm);
	}
}
