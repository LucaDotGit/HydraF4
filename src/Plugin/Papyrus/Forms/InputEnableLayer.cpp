#include "Plugin/Papyrus/Forms/InputEnableLayer.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::InputEnableLayer
{
	static std::uint32_t GetId(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSInputEnableLayer> a_inputLayer)
	{
		if (!a_inputLayer) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::INPUT_ENABLE_LAYER_NULL, a_stackId);
			return std::numeric_limits<std::uint32_t>::max();
		}

		return std::atomic_ref(a_inputLayer->layerID).load(std::memory_order_acquire);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetId);
	}
}
