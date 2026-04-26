#pragma once

namespace Plugin::SerializationManager
{
	inline constexpr auto SERIALIZATION_ID = REX::HashCrc32("Hydra"sv);

	void Setup(REX::NotNull<const F4SE::SerializationInterface*> a_serializer);
	void Load(const F4SE::SerializationInterface* a_serializer);
	void Save(const F4SE::SerializationInterface* a_serializer);
	void Revert(const F4SE::SerializationInterface* a_serializer);
}
