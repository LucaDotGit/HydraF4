#include "Plugin/Internal/Structs/Versions.hpp"

namespace RE::BSScript
{
	static constexpr auto MAJOR_KEY = "iMajor"sv;
	static constexpr auto MINOR_KEY = "iMinor"sv;
	static constexpr auto PATCH_KEY = "iPatch"sv;
	static constexpr auto BUILD_KEY = "iBuild"sv;

	auto variable_packer<::Plugin::Internal::Structs::Versions::VersionData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<Plugin::Internal::Structs::Versions::Version>();
	}

	void variable_packer<::Plugin::Internal::Structs::Versions::VersionData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Versions::VersionData& a_val)
	{
		auto versionStruct = ::Plugin::Internal::Structs::Versions::Version::Create();
		REX::Assert(static_cast<bool>(versionStruct));

		const auto structLock = versionStruct.Lock();
		versionStruct.Insert_NoLock(MAJOR_KEY, a_val.GetMajor());
		versionStruct.Insert_NoLock(MINOR_KEY, a_val.GetMinor());
		versionStruct.Insert_NoLock(PATCH_KEY, a_val.GetPatch());
		versionStruct.Insert_NoLock(BUILD_KEY, a_val.GetBuild());

		RE::BSScript::PackVariable(a_var, std::move(versionStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Versions::VersionData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Versions::VersionData
	{
		auto versionStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Versions::Version>(a_var);
		if (!versionStruct) {
			return {};
		}

		const auto structLock = versionStruct.Lock();

		const auto major = versionStruct.FindOrDefault_NoLock<std::uint16_t>(MAJOR_KEY);
		const auto minor = versionStruct.FindOrDefault_NoLock<std::uint16_t>(MINOR_KEY);
		const auto patch = versionStruct.FindOrDefault_NoLock<std::uint16_t>(PATCH_KEY);
		const auto build = versionStruct.FindOrDefault_NoLock<std::uint16_t>(BUILD_KEY);

		return ::Plugin::Internal::Structs::Versions::VersionData{ major, minor, patch, build };
	}
}
