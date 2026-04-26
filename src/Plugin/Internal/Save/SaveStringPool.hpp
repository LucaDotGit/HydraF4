#pragma once

#include "Plugin/Internal/Save/ISaveObject.hpp"

namespace Plugin::Internal::Save
{
	class SaveStringPool final
		: public ISaveObject
	{
	public:
		using value_type = RE::BSFixedStringCS;
		using size_type = RE::BSFixedStringCS::size_type;
		using difference_type = RE::BSFixedStringCS::difference_type;

		SaveStringPool(std::uint32_t a_type, std::uint32_t a_version);
		~SaveStringPool() noexcept override;

		SaveStringPool(const SaveStringPool&) = delete;
		SaveStringPool(SaveStringPool&&) = delete;

		SaveStringPool& operator=(const SaveStringPool&) = delete;
		SaveStringPool& operator=(SaveStringPool&&) = delete;

		bool Load(const F4SE::SerializationInterface& a_serializer, const std::shared_ptr<SaveStringPool>& a_stringPool) override;
		bool Save(const F4SE::SerializationInterface& a_serializer, const SaveStream& a_stream) const override;

		bool Load(const SaveStream& a_stream) override;
		bool Save(SaveStream& a_stream) const override;
		void Revert() noexcept override;

		[[nodiscard]] bool IsEmpty() const noexcept;
		[[nodiscard]] size_type GetSize() const noexcept;

		[[nodiscard]] bool Contains(size_type a_index) const noexcept;
		[[nodiscard]] bool Contains(const value_type& a_key) const noexcept;

		bool GetString(size_type a_index, value_type& a_outString) const noexcept;
		bool GetIndex(const value_type& a_key, size_type& a_outIndex) const noexcept;

		size_type AddString(const value_type& a_key);

		void Reserve(size_type a_size);
		void Clear() noexcept;

	private:
		mutable std::shared_mutex _mutex;
		std::unordered_map<value_type, size_type> _stringIndexMap;
		std::vector<value_type> _strings;
		size_type _totalByteSize{ 0 };
	};
}
