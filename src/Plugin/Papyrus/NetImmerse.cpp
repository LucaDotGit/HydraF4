#include "Plugin/Papyrus/NetImmerse.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Vectors3.hpp"
#include "Plugin/Internal/Structs/Vectors4.hpp"

namespace Plugin::Papyrus::NetImmerse
{
	using Vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3Data;
	using Vector4Struct = ::Plugin::Internal::Structs::Vectors4::Vector4Data;
}

namespace Plugin::Papyrus::NetImmerse::Impl
{
	static constexpr auto ROW00_KEY = "kRow00"sv;
	static constexpr auto ROW01_KEY = "kRow01"sv;
	static constexpr auto ROW02_KEY = "kRow02"sv;

	static constexpr auto ROTATION_KEY = "kRotation"sv;
	static constexpr auto POSITION_KEY = "kPosition"sv;
	static constexpr auto SCALE_KEY = "fScale"sv;

	template <class T>
	concept NiExtraData = std::derived_from<T, RE::NiExtraData>;

	[[nodiscard]] static RE::NiPointer<RE::NiAVObject> GetRef3DObject(RE::TESObjectREFR* a_ref, bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		if (a_firstPerson && a_ref->IsPlayerRef()) {
			const auto* player = RE::PlayerCharacter::GetSingleton();
			if (!player) [[unlikely]] {
				REX::Assert(false);
				return nullptr;
			}

			return player->firstPerson3D;
		}

		const auto* loadedData = a_ref->loadedData;
		if (!loadedData) {
			return nullptr;
		}

		auto ref3D = loadedData->data3D;
		if (!ref3D) {
			return nullptr;
		}

		return ref3D;
	}

	static void OnNiObjectChange(RE::NiPointer<RE::NiAVObject> a_node)
	{
		if (!a_node) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		F4SE::GetTaskInterface()->AddTask([a_node = std::move(a_node)]() {
			if (!a_node) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			auto updateData = RE::NiUpdateData();
			a_node->UpdateTransforms(updateData);
		});
	}

	[[nodiscard]] static NiTransform GetNodeTransform(
		const RE::NiTransform& a_transform)
	{
		auto rotation = NiMatrix4x3::Create();
		rotation.Insert(ROW00_KEY, Vector4Struct::FromNiPoint4(a_transform.rotate[0]));
		rotation.Insert(ROW01_KEY, Vector4Struct::FromNiPoint4(a_transform.rotate[1]));
		rotation.Insert(ROW02_KEY, Vector4Struct::FromNiPoint4(a_transform.rotate[2]));

		auto position = Vector3Struct::FromNiPoint3(a_transform.translate);

		auto result = NiTransform::Create();
		result.Insert(ROTATION_KEY, std::move(rotation));
		result.Insert(POSITION_KEY, position);
		result.Insert(SCALE_KEY, a_transform.scale);

		return result;
	}

	static void SetNodeTransform(
		const RE::NiPointer<RE::NiAVObject>& a_node,
		RE::NiTransform& a_transform,
		const NiTransform& a_value)
	{
		if (!a_node) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		auto rotation = a_value.Find<NiMatrix4x3>(ROTATION_KEY).value();
		auto position = a_value.Find<Vector3Struct>(POSITION_KEY).value();
		auto scale = a_value.Find<REX::Float32>(SCALE_KEY).value();

		const auto row0 = rotation.Find<Vector4Struct>(ROW00_KEY).value();
		const auto row1 = rotation.Find<Vector4Struct>(ROW01_KEY).value();
		const auto row2 = rotation.Find<Vector4Struct>(ROW02_KEY).value();

		auto newTransform = RE::NiTransform(
			RE::NiMatrix3{ Vector4Struct::ToNiPoint4(row0), Vector4Struct::ToNiPoint4(row1), Vector4Struct::ToNiPoint4(row2) },
			Vector3Struct::ToNiPoint3(position),
			scale);

		a_transform = newTransform;

		OnNiObjectChange(a_node);
	}

	template <NiExtraData ExtraDataT>
	__forceinline static bool IsNodeExtraData(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::TESObjectREFR* a_ref,
		const RE::BSFixedString& a_nodeName,
		const RE::BSFixedString& a_dataName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return false;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return false;
		}

		const auto* extraList = childObject->extra;
		if (!extraList) {
			return false;
		}

		const auto extraData = childObject->GetExtraData(a_dataName);
		if (!extraData) {
			return false;
		}

		return RE::dynamic_ni_pointer_cast<ExtraDataT>(extraData) != nullptr;
	}

	template <NiExtraData ExtraDataT, class ResultT>
	[[nodiscard]] __forceinline static ResultT GetNodeExtraData(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::TESObjectREFR* a_ref,
		const RE::BSFixedString& a_nodeName,
		const RE::BSFixedString& a_dataName,
		ResultT a_default,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return a_default;
		}

		const auto rootObject = GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return a_default;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return a_default;
		}

		const auto* extraList = childObject->extra;
		if (!extraList) {
			return a_default;
		}

		const auto extraData = childObject->GetExtraData(a_dataName);
		if (!extraData) {
			return a_default;
		}

		const auto extraDataType = RE::dynamic_ni_pointer_cast<ExtraDataT>(extraData);
		if (!extraDataType) {
			return a_default;
		}

		return extraDataType->data;
	}

	template <NiExtraData ExtraDataT, class ResultT>
	[[nodiscard]] __forceinline static bool SetNodeExtraData(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::TESObjectREFR* a_ref,
		const RE::BSFixedString& a_nodeName,
		const RE::BSFixedString& a_dataName,
		ResultT&& a_value,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto rootObject = GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return false;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return false;
		}

		const auto* extraList = childObject->extra;
		if (!extraList) {
			return false;
		}

		const auto extraData = childObject->GetExtraData(a_dataName);
		if (!extraData) {
			return false;
		}

		const auto extraDataType = RE::dynamic_ni_pointer_cast<ExtraDataT>(extraData);
		if (!extraDataType) {
			return false;
		}

		extraDataType->data = std::forward<ResultT>(a_value);
		OnNiObjectChange(childObject);

		return true;
	}

	template <NiExtraData ExtraDataT, class ResultT>
	[[nodiscard]] __forceinline static std::optional<std::vector<ResultT>> GetNodeExtraDataArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::TESObjectREFR* a_ref,
		const RE::BSFixedString& a_nodeName,
		const RE::BSFixedString& a_dataName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return std::nullopt;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return std::nullopt;
		}

		const auto* extraList = childObject->extra;
		if (!extraList) {
			return std::nullopt;
		}

		const auto extraData = childObject->GetExtraData(a_dataName);
		if (!extraData) {
			return std::nullopt;
		}

		const auto extraDataType = RE::dynamic_ni_pointer_cast<ExtraDataT>(extraData);
		if (!extraDataType) {
			return std::nullopt;
		}

		return std::vector<ResultT>{ extraDataType->data, extraDataType->data + extraDataType->size };
	}

	template <NiExtraData ExtraDataT, class ResultT>
	[[nodiscard]] __forceinline static bool SetNodeExtraDataArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		RE::TESObjectREFR* a_ref,
		const RE::BSFixedString& a_nodeName,
		const RE::BSFixedString& a_dataName,
		const std::vector<ResultT>& a_value,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return false;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return false;
		}

		const auto* extraList = childObject->extra;
		if (!extraList) {
			return false;
		}

		const auto extraData = childObject->GetExtraData(a_dataName);
		if (!extraData) {
			return false;
		}

		const auto extraDataType = RE::dynamic_ni_pointer_cast<ExtraDataT>(extraData);
		if (!extraDataType) {
			return false;
		}

		const auto size = static_cast<std::uint32_t>(a_value.size());

		if (extraDataType->data) {
			RE::ni_free(static_cast<void*>(extraDataType->data));
		}

		auto* data = RE::NiStlAllocator<ResultT>().allocate(size);
		std::ranges::copy(a_value, data);

		extraDataType->data = data;
		extraDataType->size = size;

		OnNiObjectChange(childObject);
		return true;
	}
}

namespace Plugin::Papyrus::NetImmerse
{
	static bool ContainsNode(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return false;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		return childObject != nullptr;
	}

	static std::optional<std::vector<RE::BSFixedString>> GetChildrenNames(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return std::nullopt;
		}

		const auto rootNode = RE::dynamic_ni_pointer_cast<RE::NiNode>(rootObject);
		if (!rootNode) {
			return std::nullopt;
		}

		const auto& nodeChildren = rootNode->children;

		auto result = std::vector<RE::BSFixedString>();
		result.reserve(nodeChildren.size());

		for (const auto& child : nodeChildren) {
			result.push_back(child->name);
		}

		return result;
	}

	static RE::BSFixedString GetNodeParentName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return {};
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return {};
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return {};
		}

		const auto parentObject = RE::NiPointer<RE::NiNode>(childObject->parent);
		if (!parentObject) {
			return {};
		}

		return parentObject->name;
	}

	static std::optional<std::vector<RE::BSFixedString>> GetNodeChildrenNames(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return std::nullopt;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return std::nullopt;
		}

		const auto childNode = RE::dynamic_ni_pointer_cast<RE::NiNode>(childObject);
		if (!rootObject) {
			return std::nullopt;
		}

		const auto& nodeChildren = childNode->children;

		auto result = std::vector<RE::BSFixedString>();
		result.reserve(nodeChildren.size());

		for (const auto& child : nodeChildren) {
			result.push_back(child->name);
		}

		return result;
	}

	static NiTransform GetNodeLocalTransform(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return nullptr;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return nullptr;
		}

		return Impl::GetNodeTransform(childObject->local);
	}

	static bool SetNodeLocalTransform(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		NiTransform a_transform,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return false;
		}

		auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return false;
		}

		Impl::SetNodeTransform(childObject, childObject->local, a_transform);
		return true;
	}

	static NiTransform GetNodeWorldTransform(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return nullptr;
		}

		const auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return nullptr;
		}

		return Impl::GetNodeTransform(childObject->world);
	}

	static bool SetNodeWorldTransform(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		NiTransform a_transform,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return false;
		}

		auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return false;
		}

		Impl::SetNodeTransform(childObject, childObject->world, a_transform);
		return true;
	}

	static std::optional<Vector3Struct> GetRelativeNodePosition(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_leftNodeName,
		RE::BSFixedString a_rightNodeName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return std::nullopt;
		}

		auto leftObject = RE::NiPointer(rootObject->GetObjectByName(a_leftNodeName));
		if (!leftObject) {
			return std::nullopt;
		}

		auto rightObject = RE::NiPointer(rootObject->GetObjectByName(a_rightNodeName));
		if (!rightObject) {
			return std::nullopt;
		}

		const auto leftTransform = leftObject->world.translate;
		const auto rightTransform = rightObject->world.translate;

		const auto transformX = rightTransform.x - leftTransform.x;
		const auto transformY = rightTransform.y - leftTransform.y;
		const auto transformZ = rightTransform.z - leftTransform.z;

		return Vector3Struct(transformX, transformY, transformZ);
	}

	static bool ContainsNodeExtraData(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return false;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return false;
		}

		auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return false;
		}

		auto* extraList = childObject->extra;
		if (!extraList) {
			return false;
		}

		const auto extraData = childObject->GetExtraData(a_dataName);
		return extraData != nullptr;
	}

	static std::optional<std::vector<RE::BSFixedString>> GetNodeExtraDataNames(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		bool a_firstPerson)
	{
		if (!a_ref) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return std::nullopt;
		}

		const auto rootObject = Impl::GetRef3DObject(a_ref, a_firstPerson);
		if (!rootObject) {
			return std::nullopt;
		}

		auto childObject = RE::NiPointer(rootObject->GetObjectByName(a_nodeName));
		if (!childObject) {
			return std::nullopt;
		}

		auto* extraList = childObject->extra;
		if (!extraList) {
			return std::nullopt;
		}

		auto result = std::vector<RE::BSFixedString>();
		result.reserve(extraList->dataArray.size());

		for (const auto& extraData : extraList->dataArray) {
			if (extraData) {
				result.push_back(extraData->name);
			}
		}

		return result;
	}

	static bool IsNodeExtraDataBool(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::IsNodeExtraData<RE::NiBooleanExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static bool GetNodeExtraDataBool(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_default,
		bool a_firstPerson)
	{
		return Impl::GetNodeExtraData<RE::NiBooleanExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_default, a_firstPerson);
	}

	static bool SetNodeExtraDataBool(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_value,
		bool a_firstPerson)
	{
		return Impl::SetNodeExtraData<RE::NiBooleanExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_value, a_firstPerson);
	}

	static bool IsNodeExtraDataInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::IsNodeExtraData<RE::NiIntegerExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static std::int32_t GetNodeExtraDataInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		std::int32_t a_default,
		bool a_firstPerson)
	{
		return Impl::GetNodeExtraData<RE::NiIntegerExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_default, a_firstPerson);
	}

	static bool SetNodeExtraDataInt(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		std::int32_t a_value,
		bool a_firstPerson)
	{
		return Impl::SetNodeExtraData<RE::NiIntegerExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_value, a_firstPerson);
	}

	static bool IsNodeExtraDataFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::IsNodeExtraData<RE::NiFloatExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static REX::Float32 GetNodeExtraDataFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		REX::Float32 a_default,
		bool a_firstPerson)
	{
		return Impl::GetNodeExtraData<RE::NiFloatExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_default, a_firstPerson);
	}

	static bool SetNodeExtraDataFloat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		REX::Float32 a_value,
		bool a_firstPerson)
	{
		return Impl::SetNodeExtraData<RE::NiFloatExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_value, a_firstPerson);
	}

	static bool IsNodeExtraDataString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::IsNodeExtraData<RE::NiStringExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static RE::BSFixedString GetNodeExtraDataString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		RE::BSFixedString a_default,
		bool a_firstPerson)
	{
		return Impl::GetNodeExtraData<RE::NiStringExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_default, a_firstPerson);
	}

	static bool SetNodeExtraDataString(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		RE::BSFixedString a_value,
		bool a_firstPerson)
	{
		return Impl::SetNodeExtraData<RE::NiStringExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_value, a_firstPerson);
	}

	static bool IsNodeExtraDataIntArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::IsNodeExtraData<RE::NiIntegersExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static std::optional<std::vector<std::int32_t>> GetNodeExtraDataIntArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::GetNodeExtraDataArray<RE::NiIntegersExtraData, std::int32_t>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static bool SetNodeExtraDataIntArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		std::vector<std::int32_t> a_values,
		bool a_firstPerson)
	{
		return Impl::SetNodeExtraDataArray<RE::NiIntegersExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_values, a_firstPerson);
	}

	static bool IsNodeExtraDataFloatArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::IsNodeExtraData<RE::NiFloatsExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static std::optional<std::vector<REX::Float32>> GetNodeExtraDataFloatArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::GetNodeExtraDataArray<RE::NiFloatsExtraData, REX::Float32>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static bool SetNodeExtraDataFloatArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		std::vector<REX::Float32> a_values,
		bool a_firstPerson)
	{
		return Impl::SetNodeExtraDataArray<RE::NiFloatsExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_values, a_firstPerson);
	}

	static bool IsNodeExtraDataStringArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::IsNodeExtraData<RE::NiStringsExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static std::optional<std::vector<RE::BSFixedString>> GetNodeExtraDataStringArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		bool a_firstPerson)
	{
		return Impl::GetNodeExtraDataArray<RE::NiStringsExtraData, RE::BSFixedString>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, a_firstPerson);
	}

	static bool SetNodeExtraDataStringArray(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		RE::BSFixedString a_dataName,
		std::vector<RE::BSFixedString> a_values,
		bool a_firstPerson)
	{
		using pointer_t = RE::BSFixedString::pointer;

		auto stringPtrs = std::vector<pointer_t>();
		stringPtrs.reserve(a_values.size());

		std::ranges::transform(a_values, stringPtrs.begin(), [](const RE::BSFixedString& a_string) noexcept {
			return const_cast<pointer_t>(a_string.data());
		});

		return Impl::SetNodeExtraDataArray<RE::NiStringsExtraData>(a_vm, a_stackId, a_ref, a_nodeName, a_dataName, stringPtrs, a_firstPerson);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsNode);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetChildrenNames);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeParentName);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeChildrenNames);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeLocalTransform);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeLocalTransform);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeWorldTransform);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeWorldTransform);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRelativeNodePosition);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ContainsNodeExtraData);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeExtraDataNames);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsNodeExtraDataBool);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeExtraDataBool);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeExtraDataBool);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsNodeExtraDataInt);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeExtraDataInt);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeExtraDataInt);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsNodeExtraDataFloat);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeExtraDataFloat);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeExtraDataFloat);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsNodeExtraDataString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeExtraDataString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeExtraDataString);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsNodeExtraDataIntArray);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeExtraDataIntArray);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeExtraDataIntArray);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsNodeExtraDataFloatArray);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeExtraDataFloatArray);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeExtraDataFloatArray);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, IsNodeExtraDataStringArray);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetNodeExtraDataStringArray);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetNodeExtraDataStringArray);
	}
}
