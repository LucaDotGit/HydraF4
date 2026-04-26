#include "Plugin/Internal/Structs/FunctionRefs.hpp"

namespace RE::BSScript
{
	static constexpr auto OBJECT_KEY = "kObject"sv;
	static constexpr auto SCRIPT_NAME_KEY = "sScriptName"sv;
	static constexpr auto FUNCTION_NAME_KEY = "sFunctionName"sv;

	auto variable_packer<::Plugin::Internal::Structs::FunctionRefs::FunctionRefData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::FunctionRefs::FunctionRef>();
	}

	void variable_packer<::Plugin::Internal::Structs::FunctionRefs::FunctionRefData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::FunctionRefs::FunctionRefData& a_val)
	{
		auto functionRefStruct = ::Plugin::Internal::Structs::FunctionRefs::FunctionRef::Create();
		REX::Assert(static_cast<bool>(functionRefStruct));

		const auto structLock = functionRefStruct.Lock();
		functionRefStruct.Insert_NoLock(OBJECT_KEY, a_val.object);
		functionRefStruct.Insert_NoLock(SCRIPT_NAME_KEY, a_val.scriptName);
		functionRefStruct.Insert_NoLock(FUNCTION_NAME_KEY, a_val.functionName);

		RE::BSScript::PackVariable(a_var, std::move(functionRefStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::FunctionRefs::FunctionRefData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::FunctionRefs::FunctionRefData
	{
		auto functionRefStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::FunctionRefs::FunctionRef>(a_var);
		if (!functionRefStruct) {
			return {};
		}

		const auto structLock = functionRefStruct.Lock();

		const auto object = functionRefStruct.FindOrDefault_NoLock<RE::BSTSmartPointer<RE::BSScript::Object>>(OBJECT_KEY);
		const auto scriptName = functionRefStruct.FindOrDefault_NoLock<RE::BSFixedString>(SCRIPT_NAME_KEY);
		const auto functionName = functionRefStruct.FindOrDefault_NoLock<RE::BSFixedString>(FUNCTION_NAME_KEY);

		return {
			.object = object,
			.scriptName = scriptName,
			.functionName = functionName
		};
	}
}
