#include "Plugin/Internal/Structs/Vectors3.hpp"

namespace RE::BSScript
{
	static constexpr auto X_KEY = "fX"sv;
	static constexpr auto Y_KEY = "fY"sv;
	static constexpr auto Z_KEY = "fZ"sv;

	auto variable_packer<::Plugin::Internal::Structs::Vectors3::Vector3Data>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::Vectors3::Vector3>();
	}

	void variable_packer<::Plugin::Internal::Structs::Vectors3::Vector3Data>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Vectors3::Vector3Data& a_val)
	{
		auto vector3Struct = ::Plugin::Internal::Structs::Vectors3::Vector3::Create();
		REX::Assert(static_cast<bool>(vector3Struct));

		const auto structLock = vector3Struct.Lock();
		vector3Struct.Insert_NoLock(X_KEY, a_val.x);
		vector3Struct.Insert_NoLock(Y_KEY, a_val.y);
		vector3Struct.Insert_NoLock(Z_KEY, a_val.z);

		RE::BSScript::PackVariable(a_var, std::move(vector3Struct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Vectors3::Vector3Data>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Vectors3::Vector3Data
	{
		auto vector3Struct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Vectors3::Vector3>(a_var);
		if (!vector3Struct) {
			return {};
		}

		const auto structLock = vector3Struct.Lock();

		const auto x = vector3Struct.FindOrDefault_NoLock<REX::Float32>(X_KEY);
		const auto y = vector3Struct.FindOrDefault_NoLock<REX::Float32>(Y_KEY);
		const auto z = vector3Struct.FindOrDefault_NoLock<REX::Float32>(Z_KEY);

		return {
			.x = x,
			.y = y,
			.z = z
		};
	}
}
