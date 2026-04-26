#include "Plugin/Internal/Structs/Uuids.hpp"

namespace RE::BSScript
{
	static constexpr auto DATA_01_KEY = "iData01"sv;
	static constexpr auto DATA_02_KEY = "iData02"sv;
	static constexpr auto DATA_03_KEY = "iData03"sv;
	static constexpr auto DATA_04_KEY = "iData04"sv;

	static_assert(sizeof(std::uint32_t) * 4 == sizeof(::Plugin::Internal::Structs::Uuids::UuidData));

	auto variable_packer<::Plugin::Internal::Structs::Uuids::UuidData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<Plugin::Internal::Structs::Uuids::Uuid>();
	}

	void variable_packer<::Plugin::Internal::Structs::Uuids::UuidData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::Uuids::UuidData& a_val)
	{
		auto uuidStruct = ::Plugin::Internal::Structs::Uuids::Uuid::Create();
		REX::Assert(static_cast<bool>(uuidStruct));

		const auto structLock = uuidStruct.Lock();
		uuidStruct.Insert_NoLock(DATA_01_KEY, reinterpret_cast<const std::uint32_t&>(a_val[sizeof(std::uint32_t) * 0]));
		uuidStruct.Insert_NoLock(DATA_02_KEY, reinterpret_cast<const std::uint32_t&>(a_val[sizeof(std::uint32_t) * 1]));
		uuidStruct.Insert_NoLock(DATA_03_KEY, reinterpret_cast<const std::uint32_t&>(a_val[sizeof(std::uint32_t) * 2]));
		uuidStruct.Insert_NoLock(DATA_04_KEY, reinterpret_cast<const std::uint32_t&>(a_val[sizeof(std::uint32_t) * 3]));

		RE::BSScript::PackVariable(a_var, std::move(uuidStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::Uuids::UuidData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::Uuids::UuidData
	{
		auto uuidStruct = RE::BSScript::UnpackVariable<::Plugin::Internal::Structs::Uuids::Uuid>(a_var);
		if (!uuidStruct) {
			return {};
		}

		const auto structLock = uuidStruct.Lock();

		auto dataArray = ::Plugin::Internal::Structs::Uuids::UuidData::container_type();

		reinterpret_cast<std::uint32_t&>(dataArray[sizeof(std::uint32_t) * 0]) = uuidStruct.FindOrDefault_NoLock<std::uint32_t>(DATA_01_KEY);
		reinterpret_cast<std::uint32_t&>(dataArray[sizeof(std::uint32_t) * 1]) = uuidStruct.FindOrDefault_NoLock<std::uint32_t>(DATA_02_KEY);
		reinterpret_cast<std::uint32_t&>(dataArray[sizeof(std::uint32_t) * 2]) = uuidStruct.FindOrDefault_NoLock<std::uint32_t>(DATA_03_KEY);
		reinterpret_cast<std::uint32_t&>(dataArray[sizeof(std::uint32_t) * 3]) = uuidStruct.FindOrDefault_NoLock<std::uint32_t>(DATA_04_KEY);

		return { dataArray };
	}
}
