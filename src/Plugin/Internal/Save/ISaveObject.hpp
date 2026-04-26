#pragma once

namespace Plugin::Internal::Save
{
	class SaveStream;
	class SaveStringPool;

	class ISaveObject
	{
	public:
		ISaveObject(std::uint32_t a_type, std::uint32_t a_version);
		virtual ~ISaveObject() noexcept;

		ISaveObject(const ISaveObject&) = default;
		ISaveObject(ISaveObject&&) noexcept = default;

		ISaveObject& operator=(const ISaveObject&) = default;
		ISaveObject& operator=(ISaveObject&&) noexcept = default;

		[[nodiscard]] virtual std::uint32_t GetType() const noexcept { return _type; }
		[[nodiscard]] virtual std::uint32_t GetVersion() const noexcept { return _version; }

		virtual bool Load(const F4SE::SerializationInterface& a_serializer, const std::shared_ptr<SaveStringPool>& a_stringPool);
		virtual bool Save(const F4SE::SerializationInterface& a_serializer, const SaveStream& a_stream) const;

		virtual bool Load(const SaveStream& a_stream) = 0;
		virtual bool Save(SaveStream& a_stream) const = 0;
		virtual void Revert() noexcept = 0;

	protected:
		using json_t = nlohmann::json;

		std::uint32_t _type{ 0 };
		std::uint32_t _version{ 0 };
	};
}
