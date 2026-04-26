#include "Plugin/Internal/Structs/Vectors4.hpp"

namespace RE::BSScript
{
	static constexpr auto X_KEY = "fX"sv;
	static constexpr auto Y_KEY = "fY"sv;
	static constexpr auto Z_KEY = "fZ"sv;
	static constexpr auto W_KEY = "fW"sv;

	auto variable_packer<::Plugin::Internal::Structs::Vectors4::Vector4Data>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::Vectors4::Vector4>();
	}

	void variable_packer<::Plugin::Internal::Structs::Vectors4::Vector4Data>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Vectors4::Vector4Data& a_val)
	{
		auto vector4Struct = ::Plugin::Internal::Structs::Vectors4::Vector4::Create();
		REX::Assert(static_cast<bool>(vector4Struct));

		const auto structLock = vector4Struct.Lock();
		vector4Struct.Insert_NoLock(X_KEY, a_val.x);
		vector4Struct.Insert_NoLock(Y_KEY, a_val.y);
		vector4Struct.Insert_NoLock(Z_KEY, a_val.z);
		vector4Struct.Insert_NoLock(W_KEY, a_val.w);

		RE::BSScript::PackVariable(a_var, std::move(vector4Struct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Vectors4::Vector4Data>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Vectors4::Vector4Data
	{
		auto vector4Struct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Vectors4::Vector4>(a_var);
		if (!vector4Struct) {
			return {};
		}

		const auto structLock = vector4Struct.Lock();

		const auto x = vector4Struct.FindOrDefault_NoLock<REX::Float32>(X_KEY);
		const auto y = vector4Struct.FindOrDefault_NoLock<REX::Float32>(Y_KEY);
		const auto z = vector4Struct.FindOrDefault_NoLock<REX::Float32>(Z_KEY);
		const auto w = vector4Struct.FindOrDefault_NoLock<REX::Float32>(W_KEY);

		return {
			.x = x,
			.y = y,
			.z = z,
			.w = w
		};
	}
}
