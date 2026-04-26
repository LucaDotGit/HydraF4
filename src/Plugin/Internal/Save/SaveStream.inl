#pragma once

#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	template <REX::trivially_writable T>
	bool SaveStream::ReadValue(T* a_buffer, size_type a_size) const
	{
		return _memoryStream.read(a_buffer, a_size);
	}

	template <REX::trivially_writable T>
	bool SaveStream::ReadValue(T& a_value) const
	{
		return _memoryStream.read(a_value);
	}

	template <REX::trivially_writable T>
	bool SaveStream::ReadValue(std::span<T> a_buffer) const
	{
		return _memoryStream.read(a_buffer);
	}

	template <REX::trivially_readable T>
	bool SaveStream::WriteValue(const T* a_data, size_type a_size)
	{
		return _memoryStream.write(a_data, a_size);
	}

	template <REX::trivially_readable T>
	bool SaveStream::WriteValue(const T& a_value)
	{
		return _memoryStream.write(a_value);
	}

	template <REX::trivially_readable T>
	bool SaveStream::WriteValue(std::span<const T> a_data)
	{
		return _memoryStream.write(a_data);
	}
}
