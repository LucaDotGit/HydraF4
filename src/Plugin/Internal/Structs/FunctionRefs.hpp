#pragma once

namespace Plugin::Internal::Structs::FunctionRefs
{
	using FunctionRef = RE::BSScript::StructWrapper<"Hydra:FunctionRefs", "FunctionRef">;

	class FunctionRefData final
	{
	public:
		[[nodiscard]] constexpr bool operator==(const FunctionRefData&) const noexcept = default;
		[[nodiscard]] constexpr bool operator!=(const FunctionRefData&) const noexcept = default;
		[[nodiscard]] constexpr auto operator<=>(const FunctionRefData&) const noexcept = default;

		[[nodiscard]] static constexpr FunctionRefData CreateLocalRef(
			const RE::BSTSmartPointer<RE::BSScript::Object>& a_object,
			const RE::BSFixedString& a_functionName) noexcept
		{
			return { .object = a_object, .functionName = a_functionName };
		}

		[[nodiscard]] static constexpr FunctionRefData CreateGlobalRef(
			const RE::BSFixedString& a_scriptName,
			const RE::BSFixedString& a_functionName) noexcept
		{
			return { .scriptName = a_scriptName, .functionName = a_functionName };
		}

		[[nodiscard]] static constexpr RE::BSScript::ObjectHandle GetObjectHandle(const FunctionRefData& a_functionRef) noexcept
		{
			return a_functionRef.object ? a_functionRef.object->GetHandle() : RE::GameScript::HandlePolicy::EMPTY_HANDLE;
		}

		RE::BSTSmartPointer<RE::BSScript::Object> object;
		RE::BSFixedString scriptName;
		RE::BSFixedString functionName;
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Structs::FunctionRefs::FunctionRefData>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::FunctionRefs::FunctionRefData& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::FunctionRefs::FunctionRefData;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Structs::FunctionRefs::FunctionRefData>());
}
