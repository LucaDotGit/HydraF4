#include "Plugin/Internal/Structs/Vectors2.hpp"

namespace RE::BSScript
{
	static constexpr auto X_KEY = "fX"sv;
	static constexpr auto Y_KEY = "fY"sv;

	auto variable_packer<::Plugin::Internal::Structs::Vectors2::Vector2Data>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::Vectors2::Vector2>();
	}

	void variable_packer<::Plugin::Internal::Structs::Vectors2::Vector2Data>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Vectors2::Vector2Data& a_val)
	{
		auto vector2Struct = ::Plugin::Internal::Structs::Vectors2::Vector2::Create();
		REX::Assert(static_cast<bool>(vector2Struct));

		const auto structLock = vector2Struct.Lock();
		vector2Struct.Insert_NoLock(X_KEY, a_val.x);
		vector2Struct.Insert_NoLock(Y_KEY, a_val.y);

		RE::BSScript::PackVariable(a_var, std::move(vector2Struct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Vectors2::Vector2Data>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Vectors2::Vector2Data
	{
		auto vector2Struct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Vectors2::Vector2>(a_var);
		if (!vector2Struct) {
			return {};
		}

		const auto structLock = vector2Struct.Lock();

		const auto x = vector2Struct.FindOrDefault_NoLock<REX::Float32>(X_KEY);
		const auto y = vector2Struct.FindOrDefault_NoLock<REX::Float32>(Y_KEY);

		return {
			.x = x,
			.y = y
		};
	}
}
