#include "Plugin/Papyrus/Forms/MiscObject.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::MiscObject::Impl
{
	static constexpr auto COMPONENT_KEY = "kComponent"sv;
	static constexpr auto COUNT_KEY = "iCount"sv;
}

namespace Plugin::Papyrus::Forms::MiscObject
{
	static std::optional<std::vector<ComponentEntry>> GetComponents(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectMISC* a_miscObject)
	{
		if (!a_miscObject) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MISC_OBJECT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto components = a_miscObject->GetComponents();
		if (!components.has_value()) {
			return std::nullopt;
		}

		auto result = std::vector<ComponentEntry>();
		result.reserve(components->size());

		for (auto i = 0ui32; i < components->size(); i++) {
			const auto& [component, value] = (*components)[i];

			auto entry = ComponentEntry::Create();
			entry.Insert(Impl::COMPONENT_KEY, component);
			entry.Insert(Impl::COUNT_KEY, value.int32);

			result.push_back(std::move(entry));
		}

		return result;
	}

	static void SetComponents(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectMISC* a_miscObject,
		std::vector<ComponentEntry> a_values)
	{
		if (!a_miscObject) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::MISC_OBJECT_NULL, a_stackId);
			return;
		}

		auto components = std::vector<RE::BSTPair<RE::TESForm*, RE::BGSTypedFormValuePair::SharedVal>>();
		components.reserve(a_values.size());

		for (const auto& entry : a_values) {
			if (!entry) {
				continue;
			}

			auto* component = entry.Find<RE::TESForm*>(Impl::COMPONENT_KEY).value();
			auto count = entry.Find<std::int32_t>(Impl::COUNT_KEY).value();

			components.emplace_back(component, count);
		}

		a_miscObject->SetComponents(components);

		auto displayIndices = RE::SimpleArray<std::uint8_t>(static_cast<std::uint32_t>(components.size()));

		std::ranges::iota(displayIndices.data(), displayIndices.data() + components.size(), 0ui8);
		a_miscObject->displayIndices = std::move(displayIndices);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetComponents);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetComponents);
	}
}