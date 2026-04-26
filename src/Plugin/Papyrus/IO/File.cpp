#include "Plugin/Papyrus/IO/File.hpp"

#include "Plugin/Internal/IO/IOMutex.hpp"
#include "Plugin/Internal/IO/IOSandbox.hpp"
#include "Plugin/Internal/IOManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::IO::File
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;

	static bool IsReadable(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto fileStream = std::fstream(a_path, std::ios::in);
		return fileStream.is_open();
	}

	static bool IsWritable(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto isFileError = REX::SystemError();
		if (!std::filesystem::is_regular_file(a_path, isFileError)) {
			return false;
		}

		auto fileStream = std::fstream(a_path, std::ios::out | std::ios::app);
		return fileStream.is_open();
	}

	static bool IsReadWritable(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto isFileError = REX::SystemError();
		if (!std::filesystem::is_regular_file(a_path, isFileError)) {
			return false;
		}

		auto fileStream = std::fstream(a_path, std::ios::in | std::ios::out | std::ios::app);
		return fileStream.is_open();
	}

	static std::optional<LongStruct> GetSize(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::optional<LongStruct> a_default)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return a_default;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto fileSizeError = REX::SystemError();
		const auto size = std::filesystem::file_size(a_path, fileSizeError);

		if (fileSizeError.value() != REX::ERROR_NUMBER_SUCCESS) {
			return a_default;
		}

		return LongStruct::FromUInt64(size);
	}

	static bool SetSize(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::optional<LongStruct> a_size)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		if (!a_size.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto sizeError = REX::SystemError();
		std::filesystem::resize_file(a_path, LongStruct::ToUInt64(a_size.value()), sizeError);
		return sizeError.value() == REX::ERROR_NUMBER_SUCCESS;
	}

	static std::string ReadAllText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::string_view a_default)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return std::string(a_default);
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto fileStream = std::fstream(a_path, std::ios::in);
		if (!fileStream.is_open()) {
			return std::string(a_default);
		}

		auto text = std::string{ std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>() };
		if (!fileStream.good()) {
			return std::string(a_default);
		}

		return text;
	}

	static std::optional<std::vector<std::string>> ReadAllLines(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return std::nullopt;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto fileStream = std::fstream(a_path, std::ios::in);
		if (!fileStream.is_open()) {
			return std::nullopt;
		}

		auto lines = std::vector<std::string>();
		for (auto line = std::string(); std::getline(fileStream, line);) {
			if (!fileStream.good()) {
				return std::nullopt;
			}

			lines.push_back(std::move(line));
		}

		return lines;
	}

	static bool WriteAllText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::string_view a_text)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto fileStream = std::fstream(a_path, std::ios::out | std::ios::trunc);
		if (!fileStream.is_open()) {
			return false;
		}

		REX::Print(fileStream, "{}"sv, a_text);
		return fileStream.good();
	}

	static bool WriteAllLines(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::vector<RE::BSFixedString> a_lines)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto fileStream = std::fstream(a_path, std::ios::out | std::ios::trunc);
		if (!fileStream.is_open()) {
			return false;
		}

		for (const auto& line : a_lines) {
			REX::PrintLine(fileStream, "{}"sv, line);
			if (!fileStream.good()) {
				return false;
			}
		}

		return fileStream.good();
	}

	static bool AppendAllText(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::string_view a_text)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto fileStream = std::fstream(a_path, std::ios::in | std::ios::out | std::ios::app);
		if (!fileStream.is_open()) {
			return false;
		}

		REX::Print(fileStream, "{}"sv, a_text);
		return fileStream.good();
	}

	static bool AppendAllLines(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::vector<RE::BSFixedString> a_lines)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto fileStream = std::fstream(a_path, std::ios::in | std::ios::out | std::ios::app);
		if (!fileStream.is_open()) {
			return false;
		}

		for (const auto& line : a_lines) {
			REX::PrintLine(fileStream, "{}"sv, line);
			if (!fileStream.good()) {
				return false;
			}
		}

		return fileStream.good();
	}

	static bool AppendLine(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::string_view a_line)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto fileStream = std::fstream(a_path, std::ios::in | std::ios::out | std::ios::app);
		if (!fileStream.is_open()) {
			return false;
		}

		REX::PrintLine(fileStream, "{}"sv, a_line);
		return fileStream.good();
	}

	static bool Exists(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto isFileError = REX::SystemError();
		return std::filesystem::is_regular_file(a_path, isFileError);
	}

	static bool IsEmpty(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_path);

		auto isFileError = REX::SystemError();
		if (!std::filesystem::is_regular_file(a_path, isFileError)) {
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

		auto fileStream = std::fstream(a_path, std::ios::out | std::ios::trunc);
		return fileStream.is_open();
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

		auto isFileError = REX::SystemError();
		if (!std::filesystem::is_regular_file(normalizedOldPath, isFileError)) {
			return false;
		}

		auto renameError = REX::SystemError();
		std::filesystem::rename(normalizedOldPath, normalizedNewPath, renameError);
		return renameError.value() == REX::ERROR_NUMBER_SUCCESS;
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

		if (a_oldPath.lexically_normal() == a_newPath.lexically_normal()) {
			return true;
		}

		const auto oldIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(a_oldPath);
		const auto newIOLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_newPath);

		auto isFileError = REX::SystemError();
		if (!std::filesystem::is_regular_file(a_oldPath, isFileError)) {
			return false;
		}

		using CopyOptions = std::filesystem::copy_options;

		auto copyOptions = REX::EnumSet(CopyOptions::none);
		copyOptions.set(a_overwrite, CopyOptions::overwrite_existing);

		auto copyError = REX::SystemError();
		return std::filesystem::copy_file(a_oldPath, a_newPath, copyOptions.get(), copyError);
	}

	static bool Delete(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto ioLock = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockUnique(a_path);

		auto isFileError = REX::SystemError();
		if (!std::filesystem::is_regular_file(a_path, isFileError)) {
			return false;
		}

		auto deleteError = REX::SystemError();
		return std::filesystem::remove(a_path, deleteError);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsReadable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsWritable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsReadWritable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSize);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSize);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ReadAllText);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ReadAllLines);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, WriteAllText);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, WriteAllLines);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AppendAllText);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AppendAllLines);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, AppendLine);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Exists);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsEmpty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Create);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Move);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Delete);
	}
}
