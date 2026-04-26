#include "Plugin/Papyrus/Forms/ConstructibleObject.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::ConstructibleObject::Impl
{
	static constexpr auto COMPONENT_KEY = "kComponent"sv;
	static constexpr auto COUNT_KEY = "iCount"sv;
}

namespace Plugin::Papyrus::Forms::ConstructibleObject
{
	static RE::TESForm* GetCreatedObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_object->createdItem).load(std::memory_order_acquire);
	}

	static void SetCreatedObject(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object,
		RE::TESForm* a_value)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_object->createdItem).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetCreatedObjectCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_object->data.numConstructed).load(std::memory_order_acquire);
	}

	static void SetCreatedObjectCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object,
		std::uint16_t a_value)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_object->data.numConstructed).store(a_value, std::memory_order_release);
	}

	static std::uint16_t GetWorkshopPriority(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_object->data.workshopPriority).load(std::memory_order_acquire);
	}

	static void SetWorkshopPriority(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object,
		std::uint16_t a_value)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_object->data.workshopPriority).store(a_value, std::memory_order_release);
	}

	static RE::BGSKeyword* GetWorkbenchKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_object->benchKeyword).load(std::memory_order_acquire);
	}

	static void SetWorkbenchKeyword(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object,
		RE::BGSKeyword* a_value)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_object->benchKeyword).store(a_value, std::memory_order_release);
	}

	static std::optional<std::vector<RE::BGSKeyword*>> GetRecipeFilters(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return std::nullopt;
		}

		return a_object->GetRecipeKeywords();
	}

	static void SetRecipeFilters(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object,
		std::vector<RE::BGSKeyword*> a_values)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return;
		}

		a_object->SetRecipeKeywords(a_values);
	}

	static std::optional<std::vector<ComponentEntry>> GetComponents(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto objectComponents = a_object->GetComponents();
		if (!objectComponents.has_value()) {
			return std::nullopt;
		}

		auto result = std::vector<ComponentEntry>();
		result.reserve(objectComponents->size());

		for (const auto& [component, count] : *objectComponents) {
			auto entry = ComponentEntry::Create();
			entry.Insert(Impl::COMPONENT_KEY, RE::DynamicCast<RE::BGSComponent*>(component));
			entry.Insert(Impl::COUNT_KEY, count.int32);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static void SetComponents(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BGSConstructibleObject* a_object,
		std::vector<ComponentEntry> a_values)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::CONSTRUCTIBLE_OBJECT_NULL, a_stackId);
			return;
		}

		auto components = std::vector<RE::BSTPair<RE::TESForm*, RE::BGSTypedFormValuePair::SharedVal>>();
		components.reserve(a_values.size());

		for (const auto& entry : a_values) {
			auto* component = entry.Find<RE::BGSComponent*>(Impl::COMPONENT_KEY).value();
			auto count = entry.Find<std::uint32_t>(Impl::COUNT_KEY).value();

			components.emplace_back(component, count);
		}

		a_object->SetComponents(components);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCreatedObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCreatedObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCreatedObjectCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetCreatedObjectCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWorkshopPriority);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWorkshopPriority);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetWorkbenchKeyword);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetWorkbenchKeyword);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetRecipeFilters);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetRecipeFilters);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetComponents);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetComponents);
	}
}
