#include "Plugin/Papyrus/IO/Ini.hpp"

#include "Plugin/Internal/IO/IOSandbox.hpp"
#include "Plugin/Internal/IO/IniFile.hpp"
#include "Plugin/Internal/IOManager.hpp"
#include "Plugin/Internal/Save/SaveScriptVariantMap.hpp" // IWYU pragma: keep
#include "Plugin/Internal/SaveManager.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Script/ScriptVariantMap.hpp" // IWYU pragma: keep
#include "Plugin/Internal/ScriptManager.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Papyrus::IO::Ini
{
	static bool IsCached_TempMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto tempMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetTemporaryMap();
		return ::Plugin::Internal::IO::IniFile::IsCachedAsVariantMap(*(tempMap.get()), a_path);
	}

	static bool Cache_TempMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto tempMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetTemporaryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->CacheAsVariantMap(*(tempMap.get()), a_path);
	}

	static bool CacheTo_TempMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_sourcePath,
		RE::BSFixedString a_targetNamespace)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_sourcePath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_SOURCE_PATH, a_stackId);
			return false;
		}

		auto tempMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetTemporaryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->CacheAsVariantMapTo(*(tempMap.get()), a_sourcePath, a_targetNamespace);
	}

	static bool Uncache_TempMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto tempMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetTemporaryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->UncacheAsVariantMap(*(tempMap.get()), a_path);
	}

	static bool SaveCached_TempMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto tempMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetTemporaryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->SaveCachedAsVariantMap(*(tempMap.get()), a_path);
	}

	static bool SaveCachedTo_TempMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_sourceNamespace,
		std::filesystem::path a_targetPath)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_targetPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_TARGET_PATH, a_stackId);
			return false;
		}

		auto tempMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetTemporaryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->SaveCachedAsVariantMapTo(*(tempMap.get()), a_sourceNamespace, a_targetPath);
	}

	static bool IsCached_MemMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto memMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetMemoryMap();
		return ::Plugin::Internal::IO::IniFile::IsCachedAsVariantMap(*(memMap.get()), a_path);
	}

	static bool Cache_MemMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto memMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetMemoryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->CacheAsVariantMap(*(memMap.get()), a_path);
	}

	static bool CacheTo_MemMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_sourcePath,
		RE::BSFixedString a_targetNamespace)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_sourcePath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_SOURCE_PATH, a_stackId);
			return false;
		}

		auto memMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetMemoryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->CacheAsVariantMapTo(*(memMap.get()), a_sourcePath, a_targetNamespace);
	}

	static bool Uncache_MemMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto memMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetMemoryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->UncacheAsVariantMap(*(memMap.get()), a_path);
	}

	static bool SaveCached_MemMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto memMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetMemoryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->SaveCachedAsVariantMap(*(memMap.get()), a_path);
	}

	static bool SaveCachedTo_MemMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_sourceNamespace,
		std::filesystem::path a_targetPath)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_targetPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_TARGET_PATH, a_stackId);
			return false;
		}

		auto memMap = ::Plugin::GetInternalManager()->GetScriptManager()->GetMemoryMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->SaveCachedAsVariantMapTo(*(memMap.get()), a_sourceNamespace, a_targetPath);
	}

	static bool IsCached_SaveMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		const auto saveMap = ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap();
		return ::Plugin::Internal::IO::IniFile::IsCachedAsVariantMap(*(saveMap.get()), a_path);
	}

	static bool Cache_SaveMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto saveMap = ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->CacheAsVariantMap(*(saveMap.get()), a_path);
	}

	static bool CacheTo_SaveMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_sourcePath,
		RE::BSFixedString a_targetNamespace)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_sourcePath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_SOURCE_PATH, a_stackId);
			return false;
		}

		auto saveMap = ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->CacheAsVariantMapTo(*(saveMap.get()), a_sourcePath, a_targetNamespace);
	}

	static bool Uncache_SaveMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto saveMap = ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->UncacheAsVariantMap(*(saveMap.get()), a_path);
	}

	static bool SaveCached_SaveMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::filesystem::path a_path)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_path)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX, a_stackId);
			return false;
		}

		auto saveMap = ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->SaveCachedAsVariantMap(*(saveMap.get()), a_path);
	}

	static bool SaveCachedTo_SaveMap(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_sourceNamespace,
		std::filesystem::path a_targetPath)
	{
		if (!::Plugin::GetInternalManager()->GetIOManager()->GetIOSandbox()->IsInSandbox(a_targetPath)) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OUT_OF_SANDBOX_TARGET_PATH, a_stackId);
			return false;
		}

		auto saveMap = ::Plugin::GetInternalManager()->GetSaveManager()->GetScriptVariantMap();
		return ::Plugin::GetInternalManager()->GetIOManager()->GetIniFile()->SaveCachedAsVariantMapTo(*(saveMap.get()), a_sourceNamespace, a_targetPath);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsCached_TempMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Cache_TempMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CacheTo_TempMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Uncache_TempMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveCached_TempMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveCachedTo_TempMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsCached_MemMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Cache_MemMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CacheTo_MemMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Uncache_MemMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveCached_MemMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveCachedTo_MemMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsCached_SaveMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Cache_SaveMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CacheTo_SaveMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Uncache_SaveMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveCached_SaveMap);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SaveCachedTo_SaveMap);
	}
}
