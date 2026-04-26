#include "Plugin/Papyrus/IO/Permissions.hpp"

#include "Plugin/Internal/IO/IOMutex.hpp"
#include "Plugin/Internal/IO/IOSandbox.hpp"
#include "Plugin/Internal/IOManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::IO::Permissions
{
	using PermissionFlags = std::filesystem::perms;
	using PermissionOptions = std::filesystem::perm_options;
}

namespace Plugin::Papyrus::IO::Permissions::Impl
{
	static constexpr auto ALL_READ_FLAGS = PermissionFlags::owner_read | PermissionFlags::group_read | PermissionFlags::others_read;
	static constexpr auto ALL_WRITE_FLAGS = PermissionFlags::owner_write | PermissionFlags::group_write | PermissionFlags::others_write;
	static constexpr auto ALL_EXECUTE_FLAGS = PermissionFlags::owner_exec | PermissionFlags::group_exec | PermissionFlags::others_exec;
	static constexpr auto ALL_FLAGS = PermissionFlags::owner_all | PermissionFlags::group_all | PermissionFlags::others_all;

	[[nodiscard]] __forceinline static bool ChangePermissionFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId,
		std::filesystem::path a_path,
		PermissionFlags a_flags,
		PermissionOptions a_options)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto permsError = REX::SystemError();
		std::filesystem::permissions(a_path, a_flags, a_options, permsError);
		return permsError.value() == REX::ERROR_NUMBER_SUCCESS;
	}
}

namespace Plugin::Papyrus::IO::Permissions
{
	static PermissionFlags GetFlag_OwnerRead(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::owner_read; }
	static PermissionFlags GetFlag_OwnerWrite(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::owner_write; }
	static PermissionFlags GetFlag_OwnerExecute(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::owner_exec; }
	static PermissionFlags GetFlag_OwnerAll(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::owner_all; }

	static PermissionFlags GetFlag_GroupRead(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::group_read; }
	static PermissionFlags GetFlag_GroupWrite(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::group_write; }
	static PermissionFlags GetFlag_GroupExecute(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::group_exec; }
	static PermissionFlags GetFlag_GroupAll(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::group_all; }

	static PermissionFlags GetFlag_OthersRead(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::others_read; }
	static PermissionFlags GetFlag_OthersWrite(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::others_write; }
	static PermissionFlags GetFlag_OthersExecute(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::others_exec; }
	static PermissionFlags GetFlag_OthersAll(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return PermissionFlags::others_all; }

	static PermissionFlags GetFlag_AllRead(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return Impl::ALL_READ_FLAGS; }
	static PermissionFlags GetFlag_AllWrite(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return Impl::ALL_WRITE_FLAGS; }
	static PermissionFlags GetFlag_AllExecute(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return Impl::ALL_EXECUTE_FLAGS; }
	static PermissionFlags GetFlag_All(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return Impl::ALL_FLAGS; }

	static PermissionFlags GetPermissionFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		PermissionFlags a_default)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return a_default;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto statusError = REX::SystemError();
		const auto status = std::filesystem::status(a_path, statusError);

		if (statusError.value() != REX::ERROR_NUMBER_SUCCESS) {
			return a_default;
		}

		return status.permissions();
	}

	static bool SetPermissionFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		PermissionFlags a_flags)
	{
		return Impl::ChangePermissionFlags(a_vm, a_stackId, a_path, a_flags, PermissionOptions::replace);
	}

	static bool AddPermissionFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		PermissionFlags a_flags)
	{
		return Impl::ChangePermissionFlags(a_vm, a_stackId, a_path, a_flags, PermissionOptions::add);
	}

	static bool RemovePermissionFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		PermissionFlags a_flags)
	{
		return Impl::ChangePermissionFlags(a_vm, a_stackId, a_path, a_flags, PermissionOptions::remove);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_OwnerRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_OwnerWrite);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_OwnerExecute);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_OwnerAll);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_GroupRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_GroupWrite);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_GroupExecute);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_GroupAll);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_OthersRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_OthersWrite);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_OthersExecute);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_OthersAll);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllRead);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllWrite);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_AllExecute);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_All);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetPermissionFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetPermissionFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AddPermissionFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RemovePermissionFlags);
	}
}
