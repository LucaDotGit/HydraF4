#pragma once

#include "Plugin/Internal/Save/SaveStringPool.hpp"

namespace Plugin::Internal::Save
{
	class SaveStream final
	{
	public:
		using value_type = REX::MemoryStream::value_type;
		using size_type = REX::MemoryStream::size_type;
		using difference_type = REX::MemoryStream::difference_type;
		using pointer = REX::MemoryStream::pointer;
		using const_pointer = REX::MemoryStream::const_pointer;
		using reference = REX::MemoryStream::reference;
		using const_reference = REX::MemoryStream::const_reference;
		using iterator = REX::MemoryStream::iterator;
		using const_iterator = REX::MemoryStream::const_iterator;
		using reverse_iterator = REX::MemoryStream::reverse_iterator;
		using const_reverse_iterator = REX::MemoryStream::const_reverse_iterator;

		explicit SaveStream(
			REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> a_serializer);

		explicit SaveStream(
			REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> a_serializer,
			std::size_t a_size);

		explicit SaveStream(
			REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> a_serializer,
			std::shared_ptr<SaveStringPool> a_stringPool);

		explicit SaveStream(
			REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> a_serializer,
			std::vector<value_type>&& a_data,
			std::shared_ptr<SaveStringPool> a_stringPool);

		~SaveStream() noexcept = default;

		SaveStream(const SaveStream&) = default;
		SaveStream(SaveStream&&) noexcept = default;

		SaveStream& operator=(const SaveStream&) = default;
		SaveStream& operator=(SaveStream&&) noexcept = default;

		[[nodiscard]] operator std::span<value_type>() noexcept { return _memoryStream; }
		[[nodiscard]] operator std::span<const value_type>() const noexcept { return _memoryStream; }

		[[nodiscard]] reference operator[](size_type a_index) { return _memoryStream[a_index]; }
		[[nodiscard]] const_reference operator[](size_type a_index) const { return _memoryStream[a_index]; }

		[[nodiscard]] reference at(size_type a_index) { return _memoryStream.at(a_index); }
		[[nodiscard]] const_reference at(size_type a_index) const { return _memoryStream.at(a_index); }

		[[nodiscard]] reference front() noexcept { return _memoryStream.front(); }
		[[nodiscard]] const_reference front() const noexcept { return _memoryStream.front(); }

		[[nodiscard]] reference back() noexcept { return _memoryStream.back(); }
		[[nodiscard]] const_reference back() const noexcept { return _memoryStream.back(); }

		[[nodiscard]] pointer data() noexcept { return _memoryStream.data(); }
		[[nodiscard]] const_pointer data() const noexcept { return _memoryStream.data(); }

		[[nodiscard]] iterator begin() noexcept { return _memoryStream.begin(); }
		[[nodiscard]] const_iterator begin() const noexcept { return _memoryStream.begin(); }
		[[nodiscard]] const_iterator cbegin() const noexcept { return _memoryStream.cbegin(); }

		[[nodiscard]] iterator end() noexcept { return _memoryStream.end(); }
		[[nodiscard]] const_iterator end() const noexcept { return _memoryStream.end(); }
		[[nodiscard]] const_iterator cend() const noexcept { return _memoryStream.cend(); }

		[[nodiscard]] reverse_iterator rbegin() noexcept { return _memoryStream.rbegin(); }
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept { return _memoryStream.rbegin(); }
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept { return _memoryStream.crbegin(); }

		[[nodiscard]] reverse_iterator rend() noexcept { return _memoryStream.rend(); }
		[[nodiscard]] const_reverse_iterator rend() const noexcept { return _memoryStream.rend(); }
		[[nodiscard]] const_reverse_iterator crend() const noexcept { return _memoryStream.crend(); }

		[[nodiscard]] size_type size() const noexcept { return _memoryStream.size(); }
		[[nodiscard]] size_type capacity() const noexcept { return _memoryStream.capacity(); }
		[[nodiscard]] constexpr size_type max_size() const noexcept { return _memoryStream.max_size(); }

		[[nodiscard]] bool empty() const noexcept { return _memoryStream.empty(); }

		[[nodiscard]] auto& GetMemoryStream() noexcept { return _memoryStream; }
		[[nodiscard]] const auto& GetMemoryStream() const noexcept { return _memoryStream; }
		[[nodiscard]] auto GetStringPool() const noexcept { return _stringPool; }

		bool ReadValue(pointer a_buffer, size_type a_size) const;

		template <REX::trivially_writable T>
		bool ReadValue(T* a_buffer, size_type a_size) const;

		template <REX::trivially_writable T>
		bool ReadValue(T& a_value) const;

		template <REX::trivially_writable T>
		bool ReadValue(std::span<T> a_buffer) const;

		bool WriteValue(const_pointer a_data, size_type a_size);

		template <REX::trivially_readable T>
		bool WriteValue(const T* a_data, size_type a_size);

		template <REX::trivially_readable T>
		bool WriteValue(const T& a_value);

		template <REX::trivially_readable T>
		bool WriteValue(std::span<const T> a_data);

		bool ReadStringRaw(std::string& a_outValue) const;
		bool ReadStringRaw(RE::BSFixedString& a_outValue) const;
		bool ReadStringRaw(RE::BSFixedStringCS& a_outValue) const;

		bool WriteStringRaw(std::string_view a_value);

		bool ReadStringPool(RE::BSFixedString& a_outValue) const;
		bool WriteStringPool(const RE::BSFixedString& a_value);

		bool ReadStringPool(RE::BSFixedStringCS& a_outValue) const;
		bool WriteStringPool(const RE::BSFixedStringCS& a_value);

		bool ReadString(RE::BSFixedString& a_outValue) const;
		bool WriteString(const RE::BSFixedString& a_value);

		bool ReadString(RE::BSFixedStringCS& a_outValue) const;
		bool WriteString(const RE::BSFixedStringCS& a_value);

		bool ReadObject(RE::BSTSmartPointer<RE::BSScript::Object>& a_outValue) const;
		bool WriteObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_value);

		bool ReadStruct(RE::BSTSmartPointer<RE::BSScript::Struct>& a_outValue) const;
		bool WriteStruct(const RE::BSTSmartPointer<RE::BSScript::Struct>& a_value);

		bool ReadArray(RE::BSTSmartPointer<RE::BSScript::Array>& a_outValue) const;
		bool WriteArray(const RE::BSTSmartPointer<RE::BSScript::Array>& a_value);

		bool ReadVar(RE::BSScript::UniqueVariant& a_outValue) const;
		bool WriteVar(RE::BSScript::SharedVariant a_value);

		bool ReadValue(RE::BSScript::Variable& a_outValue) const;
		bool WriteValue(const RE::BSScript::Variable& a_value);

		auto ResolveObjectHandle(RE::BSScript::ObjectHandle a_objectHandle) const -> std::optional<RE::BSScript::ObjectHandle>;
		auto ResolveFormId(RE::TESFormID a_formId) const -> std::optional<RE::TESFormID>;

	private:
		REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> _serializer;
		REX::MemoryStream _memoryStream;
		std::shared_ptr<SaveStringPool> _stringPool;
	};
}

#include "Plugin/Internal/Save/SaveStream.inl"
