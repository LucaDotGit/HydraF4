#include "Plugin/Internal/Save/ISaveObject.hpp"

#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	ISaveObject::ISaveObject(std::uint32_t a_type, std::uint32_t a_version)
		: _type(a_type),
		  _version(a_version)
	{
	}

	ISaveObject::~ISaveObject() noexcept = default;

	bool ISaveObject::Load(const F4SE::SerializationInterface& a_serializer, const std::shared_ptr<SaveStringPool>& a_stringPool)
	{
		auto bytes = std::vector<std::byte>();
		if (!a_serializer.ReadRecord(GetType(), GetVersion(), bytes)) {
			return true;
		}

		const auto stream = SaveStream(std::addressof(a_serializer), std::move(bytes), a_stringPool);
		if (!Load(stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool ISaveObject::Save(const F4SE::SerializationInterface& a_serializer, const SaveStream& a_stream) const
	{
		if (a_stream.GetMemoryStream().empty()) {
			return true;
		}

		if (!a_serializer.WriteRecord(GetType(), GetVersion(), std::span(a_stream.GetMemoryStream()))) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}
}
