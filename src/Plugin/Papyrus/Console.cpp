#include "Plugin/Papyrus/Console.hpp"

#include "Plugin/Internal/Core/StringUtility.hpp"

namespace Plugin::Papyrus::Console::Impl
{
	[[nodiscard]] __forceinline static ::Scaleform::Ptr<RE::Console> GetConsoleMenu()
	{
		const auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return ui->GetMenu<RE::Console>();
	}
}

namespace Plugin::Papyrus::Console
{
	static RE::BSTArray<RE::ObjectRefHandle> GetSelectableRefs(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return RE::Console::GetPickRefs();
	}

	static RE::ObjectRefHandle GetSelectedRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return RE::Console::GetCurrentPickREFR();
	}

	static bool SetSelectedRef(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::ObjectRefHandle a_ref)
	{
		const auto consoleMenu = Impl::GetConsoleMenu();
		if (!consoleMenu) {
			return false;
		}

		consoleMenu->SetCurrentPickREFR(a_ref);
		return true;
	}

	static bool ClearSelectedRef(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto consoleMenu = Impl::GetConsoleMenu();
		if (!consoleMenu) {
			return false;
		}

		auto pickRef = RE::ObjectRefHandle();
		consoleMenu->SetCurrentPickREFR(pickRef);

		return true;
	}

	static std::string GetHistory(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		auto* consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) [[unlikely]] {
			REX::Assert(false);
			return {};
		}

		auto history = consoleLog->GetHistory();
		if (!history.has_value()) {
			return {};
		}

		return std::move(history).value();
	}

	static void SetHistory(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_history)
	{
		auto* consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		consoleLog->SetHistory(a_history);
	}

	static void Clear(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		auto* consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		consoleLog->Clear();
	}

	static void Write(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_text)
	{
		auto* consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		consoleLog->Print(a_text);
	}

	static void WriteLine(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_text)
	{
		auto* consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		consoleLog->PrintLine(a_text);
	}

	static void WriteFormat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_format,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		auto* consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		const auto arrayLock = a_args.Lock();

		auto text = ::Plugin::Internal::Core::StringUtility::Format(a_args, a_format);
		if (!text.has_value()) [[unlikely]] {
			a_vm.PostError(text.error().what(), a_stackId);
			return;
		}

		consoleLog->Print(text.value());
	}

	static void WriteLineFormat(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_format,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
	{
		auto* consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		const auto arrayLock = a_args.Lock();

		auto text = ::Plugin::Internal::Core::StringUtility::Format(a_args, a_format);
		if (!text.has_value()) [[unlikely]] {
			a_vm.PostError(text.error().what(), a_stackId);
			return;
		}

		consoleLog->PrintLine(text.value());
	}

	static bool ExecuteSingleLineCommand(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_command,
		RE::TESObjectREFR* a_targetRef,
		bool a_silent)
	{
		return RE::Script::ExecuteSingleLineConsoleCommand(a_command, a_targetRef, a_silent);
	}

	static bool ExecuteMultiLineCommand(RE::BSScript::StaticTag /*a_staticTag*/,
		std::string_view a_command,
		RE::TESObjectREFR* a_targetRef,
		bool a_silent)
	{
		return RE::Script::ExecuteMultiLineConsoleCommand(a_command, a_targetRef, a_silent);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSelectableRefs);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetSelectedRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetSelectedRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, ClearSelectedRef);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetHistory);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, SetHistory);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, Clear);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Write);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, WriteLine);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, WriteFormat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, WriteLineFormat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ExecuteSingleLineCommand);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ExecuteMultiLineCommand);
	}
}
