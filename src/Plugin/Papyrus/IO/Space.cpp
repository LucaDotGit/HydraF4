#include "Plugin/Papyrus/IO/Space.hpp"

#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::IO::Space::Impl
{
	inline constexpr auto TOTAL_SPACE_KEY = "lTotalSpace"sv;
	inline constexpr auto FREE_SPACE_KEY = "lFreeSpace"sv;
	inline constexpr auto AVAILABLE_SPACE_KEY = "lAvailableSpace"sv;
}

namespace Plugin::Papyrus::IO::Space
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;

	static SpaceInfo GetCurrentSpaceInfo(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto currentDirectory = std::filesystem::path(REX::CURRENT_DIRECTORY);

		auto spaceError = REX::SystemError();
		auto spaceInfoData = std::filesystem::space(currentDirectory, spaceError);

		if (spaceError.value() != REX::ERROR_NUMBER_SUCCESS) {
			return nullptr;
		}

		auto spaceInfo = SpaceInfo::Create();

		spaceInfo.Insert(Impl::TOTAL_SPACE_KEY, LongStruct::FromUInt64(spaceInfoData.capacity));
		spaceInfo.Insert(Impl::FREE_SPACE_KEY, LongStruct::FromUInt64(spaceInfoData.free));
		spaceInfo.Insert(Impl::AVAILABLE_SPACE_KEY, LongStruct::FromUInt64(spaceInfoData.available));

		return spaceInfo;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetCurrentSpaceInfo);
	}
}
