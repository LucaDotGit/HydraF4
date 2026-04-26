#include "Plugin/Internal/Save/SaveStringPool.hpp"

#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	SaveStringPool::SaveStringPool(std::uint32_t a_type, std::uint32_t a_version)
		: ISaveObject(a_type, a_version)
	{
	}

	SaveStringPool::~SaveStringPool() noexcept = default;

	bool SaveStringPool::Load(const F4SE::SerializationInterface& a_serializer, const std::shared_ptr<SaveStringPool>& a_stringPool)
	{
		return ISaveObject::Load(a_serializer, a_stringPool);
	}

	bool SaveStringPool::Save(const F4SE::SerializationInterface& a_serializer, const SaveStream& a_stream) const
	{
		return ISaveObject::Save(a_serializer, a_stream);
	}

	bool SaveStringPool::Load(const SaveStream& a_stream)
	{
		const auto poolLock = std::scoped_lock(_mutex);

		auto mapSize = static_cast<size_type>(0);
		if (!a_stream.ReadValue(mapSize)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (mapSize == 0) {
			return true;
		}

		_stringIndexMap.reserve(mapSize);
		_strings.reserve(mapSize);

		for (auto i = static_cast<size_type>(0); i < mapSize; i++) {
			auto key = value_type();
			if (!a_stream.ReadStringRaw(key)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			_stringIndexMap.emplace(key, i);
			_strings.push_back(std::move(key));
		}

		REX::Assert(_stringIndexMap.size() == mapSize);
		REX::Assert(_strings.size() == mapSize);

		return true;
	}

	bool SaveStringPool::Save(SaveStream& a_stream) const
	{
		const auto poolLock = std::shared_lock(_mutex);

		if (_strings.empty()) {
			return true;
		}

		REX::Assert(a_stream.GetMemoryStream().empty());

		const auto indexCount = static_cast<size_type>(_strings.size());
		const auto totalByteSize = static_cast<size_type>(sizeof(size_type) + (indexCount * sizeof(size_type)) + _totalByteSize);
		a_stream.GetMemoryStream().resize(totalByteSize);

		if (!a_stream.WriteValue(indexCount)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		for (const auto& string : _strings) {
			if (!a_stream.WriteStringRaw(string)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}
		}

		REX::Assert(a_stream.GetMemoryStream().size() == totalByteSize);
		REX::Assert(a_stream.GetMemoryStream().position() == totalByteSize);

		return true;
	}

	void SaveStringPool::Revert() noexcept
	{
		Clear();
	}

	bool SaveStringPool::IsEmpty() const noexcept
	{
		const auto poolLock = std::shared_lock(_mutex);
		return _strings.empty();
	}

	SaveStringPool::size_type SaveStringPool::GetSize() const noexcept
	{
		const auto poolLock = std::shared_lock(_mutex);
		return static_cast<size_type>(_strings.size());
	}

	bool SaveStringPool::Contains(size_type a_index) const noexcept
	{
		const auto poolLock = std::shared_lock(_mutex);
		return a_index < static_cast<size_type>(_strings.size());
	}

	bool SaveStringPool::Contains(const value_type& a_key) const noexcept
	{
		const auto poolLock = std::shared_lock(_mutex);
		return _stringIndexMap.contains(a_key);
	}

	bool SaveStringPool::GetString(size_type a_index, value_type& a_outString) const noexcept
	{
		const auto poolLock = std::shared_lock(_mutex);

		if (a_index >= _strings.size()) {
			a_outString.clear();
			return false;
		}

		a_outString = _strings[a_index];
		return true;
	}

	bool SaveStringPool::GetIndex(const value_type& a_key, size_type& a_outIndex) const noexcept
	{
		const auto poolLock = std::shared_lock(_mutex);

		const auto keyIt = _stringIndexMap.find(a_key);
		if (keyIt == _stringIndexMap.end()) {
			return false;
		}

		a_outIndex = keyIt->second;
		return true;
	}

	SaveStringPool::size_type SaveStringPool::AddString(const value_type& a_key)
	{
		const auto poolLock = std::scoped_lock(_mutex);

		const auto&& [stringIt, hasInserted] = _stringIndexMap.emplace(a_key, static_cast<size_type>(_stringIndexMap.size()));
		if (!hasInserted) {
			return stringIt->second;
		}

		_strings.push_back(a_key);
		_totalByteSize += static_cast<size_type>(a_key.size());

		return stringIt->second;
	}

	void SaveStringPool::Reserve(size_type a_size)
	{
		const auto poolLock = std::scoped_lock(_mutex);

		_stringIndexMap.reserve(a_size);
		_strings.reserve(a_size);
	}

	void SaveStringPool::Clear() noexcept
	{
		const auto poolLock = std::scoped_lock(_mutex);

		_stringIndexMap.clear();
		_stringIndexMap.rehash(0);

		_strings.clear();
		_strings.shrink_to_fit();

		_totalByteSize = 0;
	}
}
