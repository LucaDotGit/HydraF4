#pragma once

namespace Plugin::Internal::Structs::Pairs
{
	using Pair = RE::BSScript::StructWrapper<"Hydra:Pairs", "Pair">;

	using PairData = std::pair<RE::BSFixedString, RE::BSScript::Variable>;
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::Pairs::PairData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Pairs::PairData& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Pairs::PairData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::Pairs::PairData>());
}