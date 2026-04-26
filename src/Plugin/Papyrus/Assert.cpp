#include "Plugin/Papyrus/Assert.hpp"

namespace Plugin::Papyrus::Assert::Impl
{
	template <class... Args>
	__forceinline static void PostWarning(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		std::string_view a_message,
		REX::FormatString<Args...> a_default,
		Args&&... a_args)
		requires((REX::formattable<Args, char> && ...))
	{
		if (a_message.empty()) {
			a_vm.PostWarning(a_default, a_stackId, std::forward<Args>(a_args)...);
			return;
		}

		const auto errorMessage = REX::TryCatch<REX::FormatError>(
			[a_message, ... a_args = std::forward<Args>(a_args)]() {
				return REX::VFormat(a_message, REX::CreateFormatArgs(a_args...));
			},
			[](const REX::FormatError& error) {
				return std::string(error.what());
			});

		a_vm.PostWarning(errorMessage, a_stackId);
	}
}

namespace Plugin::Papyrus::Assert
{
	static void IsTrue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_condition,
		std::string_view a_message)
	{
		if (a_condition) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected the condition to be true"sv);
	}

	static void IsFalse(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_condition,
		std::string_view a_message)
	{
		if (!a_condition) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected the condition to be false"sv);
	}

	static void IsNone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_value,
		std::string_view a_message)
	{
		if (a_value || !a_value->IsNone()) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected the value to be none"sv);
	}

	static void NotNone(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_value,
		std::string_view a_message)
	{
		if (a_value && !a_value->IsNone()) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected the value to not be none"sv);
	}

	static void Equals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right,
		std::string_view a_message)
	{
		if (RE::BSScript::UnwrapVariant(a_left).Equals(RE::BSScript::UnwrapVariant(a_right))) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected {} to be equal to {}"sv,
			RE::BSScript::UnwrapVariant(a_left).ToString(), RE::BSScript::UnwrapVariant(a_right).ToString());
	}

	static void NotEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right,
		std::string_view a_message)
	{
		if (!RE::BSScript::UnwrapVariant(a_left).Equals(RE::BSScript::UnwrapVariant(a_right))) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected {} to not be equal to {}"sv,
			RE::BSScript::UnwrapVariant(a_left).ToString(), RE::BSScript::UnwrapVariant(a_right).ToString());
	}

	static void DeepEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right,
		std::string_view a_message)
	{
		if (RE::BSScript::UnwrapVariant(a_left).DeepEquals(RE::BSScript::UnwrapVariant(a_right))) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected {} to be deeply equal to {}"sv,
			RE::BSScript::UnwrapVariant(a_left).ToString(), RE::BSScript::UnwrapVariant(a_right).ToString());
	}

	static void DeepNotEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right,
		std::string_view a_message)
	{
		if (!RE::BSScript::UnwrapVariant(a_left).DeepEquals(RE::BSScript::UnwrapVariant(a_right))) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected {} to not be deeply equal to {}"sv,
			RE::BSScript::UnwrapVariant(a_left).ToString(), RE::BSScript::UnwrapVariant(a_right).ToString());
	}

	static void RefEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right,
		std::string_view a_message)
	{
		if (RE::BSScript::UnwrapVariant(a_left).RefEquals(RE::BSScript::UnwrapVariant(a_right))) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected {} to have the same ref as {}"sv,
			RE::BSScript::UnwrapVariant(a_left).ToString(), RE::BSScript::UnwrapVariant(a_right).ToString());
	}

	static void RefNotEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right,
		std::string_view a_message)
	{
		if (!RE::BSScript::UnwrapVariant(a_left).RefEquals(RE::BSScript::UnwrapVariant(a_right))) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected {} to not have the same ref as {}"sv,
			RE::BSScript::UnwrapVariant(a_left).ToString(), RE::BSScript::UnwrapVariant(a_right).ToString());
	}

	static void TypeEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right,
		std::string_view a_message)
	{
		if (RE::BSScript::UnwrapVariant(a_left).GetTypeInfo().Equals(RE::BSScript::UnwrapVariant(a_right).GetTypeInfo())) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected {} to have the same type as {}"sv,
			RE::BSScript::UnwrapVariant(a_left).ToString(), RE::BSScript::UnwrapVariant(a_right).ToString());
	}

	static void TypeNotEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right,
		std::string_view a_message)
	{
		if (!RE::BSScript::UnwrapVariant(a_left).GetTypeInfo().Equals(RE::BSScript::UnwrapVariant(a_right).GetTypeInfo())) [[likely]] {
			return;
		}

		Impl::PostWarning(a_vm, a_stackId, a_message, "Expected {} to not have the same type as {}"sv,
			RE::BSScript::UnwrapVariant(a_left).ToString(), RE::BSScript::UnwrapVariant(a_right).ToString());
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsTrue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsFalse);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsNone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, NotNone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, NotEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepNotEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RefEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RefNotEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TypeEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TypeNotEquals);
	}
}
