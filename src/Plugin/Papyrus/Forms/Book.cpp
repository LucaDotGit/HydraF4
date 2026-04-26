#include "Plugin/Papyrus/Forms/Book.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"

namespace Plugin::Papyrus::Forms::Book
{
	using BookFlags = RE::OBJ_BOOK::Flags;

	static bool GetHasBeenRead(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return false;
		}

		return a_book->data.flags.any_atomic(BookFlags::kHasBeenRead);
	}

	static void SetHasBeenRead(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book,
		bool a_value)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return;
		}

		a_book->data.flags.set_atomic(a_value, BookFlags::kHasBeenRead);
		a_book->AddChange(RE::TESObjectBOOK::ChangeFlags::kRead);
	}

	static std::uint32_t GetTextOffsetX(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_book->data.textOffsetX).load(std::memory_order_acquire);
	}

	static void SetTextOffsetX(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book,
		std::uint32_t a_value)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_book->data.textOffsetX).store(a_value, std::memory_order_release);
	}

	static std::uint32_t GetTextOffsetY(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_book->data.textOffsetY).load(std::memory_order_acquire);
	}

	static void SetTextOffsetY(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book,
		std::uint32_t a_value)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_book->data.textOffsetY).store(a_value, std::memory_order_release);
	}

	static RE::TESObjectSTAT* GetInventoryModel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_book->inventoryModel).load(std::memory_order_acquire);
	}

	static void SetInventoryModel(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book,
		RE::TESObjectSTAT* a_value)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_book->inventoryModel).store(a_value, std::memory_order_release);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book,
		BookFlags a_flag)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return false;
		}

		return a_book->data.flags.any_atomic(a_flag);
	}

	static BookFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return BookFlags::kNone;
		}

		return a_book->data.flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book,
		BookFlags a_flags)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return;
		}

		a_book->data.flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectBOOK* a_book,
		BookFlags a_flag,
		bool a_set)
	{
		if (!a_book) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::BOOK_NULL, a_stackId);
			return;
		}

		a_book->data.flags.set_atomic(a_set, a_flag);
	}

	static BookFlags GetFlag_HasBeenRead(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return BookFlags::kHasBeenRead; }
	static BookFlags GetFlag_DisallowTaking(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return BookFlags::kCannotTake; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHasBeenRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetHasBeenRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTextOffsetX);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTextOffsetX);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTextOffsetY);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetTextOffsetY);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInventoryModel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetInventoryModel);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasBeenRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_DisallowTaking);
	}
}