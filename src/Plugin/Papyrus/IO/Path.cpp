#include "Plugin/Papyrus/IO/Path.hpp"

#include "Plugin/Internal/IO/IOMutex.hpp"
#include "Plugin/Internal/IO/IOSandbox.hpp"
#include "Plugin/Internal/IOManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::IO::Path
{
	static RE::BSFixedString CurrentDirectory(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto CURRENT_DIRECTORY = RE::BSFixedString(REX::CURRENT_DIRECTORY);
		return CURRENT_DIRECTORY;
	}

	static RE::BSFixedString ParentDirectory(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto PARENT_DIRECTORY = RE::BSFixedString(REX::PARENT_DIRECTORY);
		return PARENT_DIRECTORY;
	}

	static RE::BSFixedString ExtensionSeparator(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto EXTENSION_SEPARATOR = RE::BSFixedString(REX::EXTENSION_SEPARATOR);
		return EXTENSION_SEPARATOR;
	}

	static RE::BSFixedString DirectorySeparator(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		static const auto DIRECTORY_SEPARATOR = RE::BSFixedString(REX::DIRECTORY_SEPARATOR);
		return DIRECTORY_SEPARATOR;
	}

	static std::vector<std::string_view> GetInvalidPathChars(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return { REX::INVALID_PATH_CHARS.begin(), REX::INVALID_PATH_CHARS.end() };
	}

	static std::vector<std::string_view> GetInvalidFileNameChars(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return { REX::INVALID_FILE_NAME_CHARS.begin(), REX::INVALID_FILE_NAME_CHARS.end() };
	}

	static bool IsEmpty(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.empty();
	}

	static bool IsAbsolute(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.is_absolute();
	}

	static bool IsRelative(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.is_relative();
	}

	static bool IsInSandbox(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path);
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path01,
		std::filesystem::path a_path02)
	{
		const auto normalizedPath01 = a_path01.lexically_normal();
		const auto normalizedPath02 = a_path02.lexically_normal();

		return normalizedPath01.compare(normalizedPath02);
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path01,
		std::filesystem::path a_path02)
	{
		const auto normalizedPath01 = a_path01.lexically_normal();
		const auto normalizedPath02 = a_path02.lexically_normal();

		return normalizedPath01 == normalizedPath02;
	}

	static bool ResolvingEquals(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path01,
		std::filesystem::path a_path02)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path01)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_PATH_01, a_stackId);
			return false;
		}

		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path02)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_PATH_02, a_stackId);
			return false;
		}

		const auto normalizedPath01 = a_path01.lexically_normal();
		const auto normalizedPath02 = a_path02.lexically_normal();

		if (normalizedPath01 == normalizedPath02) {
			return true;
		}

		const auto ioLock01 = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(normalizedPath01);
		const auto ioLock02 = ::Plugin::GetInternalManager()->GetIOManager()->GetIOMutex()->LockShared(normalizedPath02);

		auto resolveError = REX::SystemError();
		return std::filesystem::equivalent(a_path01, a_path02, resolveError);
	}

	static std::filesystem::path GetRootName(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.root_name();
	}

	static std::filesystem::path GetRootDirectoryName(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.root_directory();
	}

	static std::filesystem::path GetRootPath(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.root_path();
	}

	static std::filesystem::path GetParent(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.parent_path();
	}

	static std::filesystem::path GetRelative(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::filesystem::path a_relativeTo)
	{
		const auto normalizedPath = a_path.lexically_normal();
		const auto normalizedRelativeTo = a_relativeTo.lexically_normal();

		return normalizedPath.lexically_relative(normalizedRelativeTo);
	}

	static std::filesystem::path GetDirectoryName(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.parent_path();
	}

	static std::filesystem::path GetFileName(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.filename();
	}

	static std::filesystem::path GetFileNameWithoutExtension(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.stem();
	}

	static std::filesystem::path GetExtension(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.extension();
	}

	static std::filesystem::path ChangeFileName(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::filesystem::path a_fileName)
	{
		a_path.replace_filename(a_fileName);
		return a_path;
	}

	static std::filesystem::path ChangeExtension(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path,
		std::filesystem::path a_extension)
	{
		a_path.replace_extension(a_extension);
		return a_path;
	}

	static std::filesystem::path TrimEndingSeparator(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (a_path.has_filename()) {
			return a_path;
		}

		return a_path.parent_path();
	}

	static std::filesystem::path Normalize(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		return a_path.lexically_normal();
	}

	static std::vector<std::filesystem::path> Split(RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		auto parts = std::vector<std::filesystem::path>();
		for (const auto& part : a_path) {
			parts.push_back(part);
		}

		return parts;
	}

	static std::filesystem::path Join(RE::BSScript::StaticTag /*a_staticTag*/,
		std::vector<std::filesystem::path> a_paths)
	{
		auto result = std::filesystem::path();
		for (const auto& a_path : a_paths) {
			result /= a_path;
		}

		return result;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CurrentDirectory);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ParentDirectory);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ExtensionSeparator);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DirectorySeparator);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInvalidPathChars);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInvalidFileNameChars);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsEmpty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsAbsolute);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRelative);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsInSandbox);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ResolvingEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRootName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRootDirectoryName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRootPath);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetParent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRelative);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDirectoryName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFileName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFileNameWithoutExtension);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetExtension);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ChangeFileName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ChangeExtension);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TrimEndingSeparator);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Normalize);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Split);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Join);
	}
}
