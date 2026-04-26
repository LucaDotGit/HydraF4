#pragma once

namespace Plugin::Internal::Structs::VarPairs
{
	using VarPair = RE::BSScript::StructWrapper<"Hydra:VarPairs", "VarPair">;

	using VarPairData = std::pair<RE::BSScript::Variable, RE::BSScript::Variable>;
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::VarPairs::VarPairData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::VarPairs::VarPairData& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::VarPairs::VarPairData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::VarPairs::VarPairData>());
}
