#include "Plugin/Papyrus/IO/Link.hpp"

#include "Plugin/Internal/IO/IOMutex.hpp"
#include "Plugin/Internal/IO/IOSandbox.hpp"
#include "Plugin/Internal/IOManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::IO::Link
{
	static bool IsSymbolicLink(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto isSymlinkError = REX::SystemError();
		return std::filesystem::is_symlink(a_path, isSymlinkError);
	}

	static std::int32_t GetHardLinkCount(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return 0;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto hardLinkCountError = REX::SystemError();
		const auto count = std::filesystem::hard_link_count(a_path, hardLinkCountError);

		if (hardLinkCountError.value() != REX::ERROR_NUMBER_SUCCESS) {
			return 0;
		}

		return static_cast<std::int32_t>(count);
	}

	static bool CreateSymbolicLink(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_sourcePath,
		std::filesystem::path a_targetPath)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_sourcePath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_SOURCE_PATH, a_stackId);
			return false;
		}

		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_targetPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_TARGET_PATH, a_stackId);
			return false;
		}

		const auto normalizedSourcePath = a_sourcePath.lexically_normal();
		const auto normalizedTargetPath = a_targetPath.lexically_normal();

		if (normalizedSourcePath == normalizedTargetPath) {
			return false;
		}

		const auto sourceIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(normalizedSourcePath);
		const auto targetIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(normalizedTargetPath);

		auto createError = REX::SystemError();
		std::filesystem::create_symlink(normalizedSourcePath, normalizedTargetPath, createError);
		return createError.value() == REX::ERROR_NUMBER_SUCCESS;
	}

	static bool CreateHardLink(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_sourcePath,
		std::filesystem::path a_targetPath)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_sourcePath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_SOURCE_PATH, a_stackId);
			return false;
		}

		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_targetPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_TARGET_PATH, a_stackId);
			return false;
		}

		const auto normalizedSourcePath = a_sourcePath.lexically_normal();
		const auto normalizedTargetPath = a_targetPath.lexically_normal();

		if (normalizedSourcePath == normalizedTargetPath) {
			return false;
		}

		const auto sourceIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(normalizedSourcePath);
		const auto targetIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(normalizedTargetPath);

		auto createError = REX::SystemError();
		std::filesystem::create_hard_link(normalizedSourcePath, normalizedTargetPath, createError);
		return createError.value() == REX::ERROR_NUMBER_SUCCESS;
	}

	static bool CopySymbolicLink(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_sourcePath,
		std::filesystem::path a_targetPath)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_sourcePath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_SOURCE_PATH, a_stackId);
			return false;
		}

		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_targetPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_TARGET_PATH, a_stackId);
			return false;
		}

		const auto normalizedSourcePath = a_sourcePath.lexically_normal();
		const auto normalizedTargetPath = a_targetPath.lexically_normal();

		if (normalizedSourcePath == normalizedTargetPath) {
			return false;
		}

		const auto sourceIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(normalizedSourcePath);
		const auto targetIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(normalizedTargetPath);

		auto copyError = REX::SystemError();
		std::filesystem::copy_symlink(normalizedSourcePath, normalizedTargetPath, copyError);
		return copyError.value() == REX::ERROR_NUMBER_SUCCESS;
	}

	static std::filesystem::path ResolveSymbolicLink(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::filesystem::path a_default)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return a_default;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto resolveError = REX::SystemError();
		auto resolvedPath = std::filesystem::read_symlink(a_path, resolveError);

		if (resolveError.value() != REX::ERROR_NUMBER_SUCCESS) {
			return a_default;
		}

		return resolvedPath;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsSymbolicLink);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHardLinkCount);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CreateSymbolicLink);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CreateHardLink);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CopySymbolicLink);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ResolveSymbolicLink);
	}
}
