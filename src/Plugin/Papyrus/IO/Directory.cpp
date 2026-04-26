#include "Plugin/Papyrus/IO/Directory.hpp"

#include "Plugin/Internal/IO/IOMutex.hpp"
#include "Plugin/Internal/IO/IOSandbox.hpp"
#include "Plugin/Internal/IOManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::IO::Directory
{
	static std::optional<std::vector<std::filesystem::path>> GetDirectories(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::filesystem::path a_searchPattern,
		bool a_recursive)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return std::nullopt;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto result = REX::GetDirectoriesInDirectory(a_path, a_searchPattern, a_recursive);
		if (!result.has_value()) [[unlikely]] {
			if (result.error() != REX::PosixErrorCode::no_such_file_or_directory) {
				a_vm.PostError(result.error().message(), a_stackId);
			}

			return std::nullopt;
		}

		return std::move(result).value();
	}

	static std::optional<std::vector<std::filesystem::path>> GetFiles(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::filesystem::path a_searchPattern,
		bool a_recursive)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return std::nullopt;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto result = REX::GetFilesInDirectory(a_path, a_searchPattern, a_recursive);
		if (!result.has_value()) [[unlikely]] {
			if (result.error() != REX::PosixErrorCode::no_such_file_or_directory) {
				a_vm.PostError(result.error().message(), a_stackId);
			}

			return std::nullopt;
		}

		return std::move(result).value();
	}

	static bool Exists(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto isDirError = REX::SystemError();
		return std::filesystem::is_directory(a_path, isDirError);
	}

	static bool IsEmpty(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto isDirError = REX::SystemError();
		if (!std::filesystem::is_directory(a_path, isDirError)) {
			return false;
		}

		auto isEmptyError = REX::SystemError();
		return std::filesystem::is_empty(a_path, isEmptyError);
	}

	static bool Create(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto createError = REX::SystemError();
		return std::filesystem::create_directories(a_path, createError);
	}

	static bool Move(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_oldPath,
		std::filesystem::path a_newPath)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_oldPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_OLD_PATH, a_stackId);
			return false;
		}

		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_newPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_NEW_PATH, a_stackId);
			return false;
		}

		const auto normalizedOldPath = a_oldPath.lexically_normal();
		const auto normalizedNewPath = a_newPath.lexically_normal();

		if (normalizedOldPath == normalizedNewPath) {
			return true;
		}

		const auto oldIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(normalizedOldPath);
		const auto newIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(normalizedNewPath);

		auto isDirError = REX::SystemError();
		if (!std::filesystem::is_directory(normalizedOldPath, isDirError)) {
			return false;
		}

		auto moveError = REX::SystemError();
		std::filesystem::rename(normalizedOldPath, normalizedNewPath, moveError);
		return moveError.value() == REX::ERROR_NUMBER_SUCCESS;
	}

	static bool Copy(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_oldPath,
		std::filesystem::path a_newPath,
		bool a_overwrite)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_oldPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_OLD_PATH, a_stackId);
			return false;
		}

		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_newPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_NEW_PATH, a_stackId);
			return false;
		}

		const auto normalizedOldPath = a_oldPath.lexically_normal();
		const auto normalizedNewPath = a_newPath.lexically_normal();

		if (normalizedOldPath == normalizedNewPath) {
			return true;
		}

		const auto oldIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(normalizedOldPath);
		const auto newIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(normalizedNewPath);

		auto isDirError = REX::SystemError();
		if (!std::filesystem::is_directory(normalizedOldPath, isDirError)) {
			return false;
		}

		using CopyOptions = std::filesystem::copy_options;

		auto copyOptions = REX::EnumSet(CopyOptions::recursive);
		copyOptions.set(a_overwrite, CopyOptions::overwrite_existing);

		auto copyError = REX::SystemError();
		std::filesystem::copy(normalizedOldPath, normalizedNewPath, copyOptions.get(), copyError);
		return copyError.value() == REX::ERROR_NUMBER_SUCCESS;
	}

	static std::int32_t Delete(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return 0;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto isDirError = REX::SystemError();
		if (!std::filesystem::is_directory(a_path, isDirError)) {
			return 0;
		}

		auto deleteError = REX::SystemError();
		const auto deleteCount = std::filesystem::remove_all(a_path, deleteError);

		if (deleteError.value() != REX::ERROR_NUMBER_SUCCESS) {
			return 0;
		}

		return static_cast<std::int32_t>(deleteCount);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDirectories);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFiles);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Exists);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsEmpty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Create);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Move);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Delete);
	}
}
