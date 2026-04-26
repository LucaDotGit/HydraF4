#pragma once

namespace Plugin::Internal::Structs::StringPairs
{
	using StringPair = RE::BSScript::StructWrapper<"Hydra:StringPairs", "StringPair">;

	using StringPairData = std::pair<RE::BSFixedString, RE::BSFixedString>;
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::StringPairs::StringPairData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::StringPairs::StringPairData& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::StringPairs::StringPairData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::StringPairs::StringPairData>());
}
