#pragma once

namespace Plugin::Internal::Structs::Uuids
{
	using Uuid = RE::BSScript::StructWrapper<"Hydra:Uuids", "Uuid">;

	using UuidData = REX::Uuid;
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Uuids::UuidData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Uuids::UuidData& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Uuids::UuidData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Uuids::UuidData>());

}
