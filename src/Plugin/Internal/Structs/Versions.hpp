#pragma once

namespace Plugin::Internal::Structs::Versions
{
	using Version = RE::BSScript::StructWrapper<"Hydra:Versions", "Version">;

	using VersionData = REX::Version;
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Versions::VersionData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Versions::VersionData& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Versions::VersionData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Versions::VersionData>());
}
