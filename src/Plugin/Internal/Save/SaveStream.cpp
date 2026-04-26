#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	static constexpr auto EMPTY_SAVE_HANDLE = RE::BSScript::Internal::VirtualMachine::EMPTY_SAVE_HANDLE;

	using RawType = RE::BSScript::TypeInfo::RawType;

	SaveStream::SaveStream(
		REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> a_serializer)
		: _serializer(a_serializer)
	{
	}

	SaveStream::SaveStream(
		REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> a_serializer,
		std::size_t a_size)
		: _serializer(a_serializer),
		  _memoryStream(a_size)
	{
	}

	SaveStream::SaveStream(
		REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> a_serializer,
		std::shared_ptr<SaveStringPool> a_stringPool)
		: _serializer(a_serializer),
		  _stringPool(std::move(a_stringPool))
	{
	}

	SaveStream::SaveStream(
		REX::NotNull<REX::Observer<const F4SE::SerializationInterface*>> a_serializer,
		std::vector<value_type>&& a_data,
		std::shared_ptr<SaveStringPool> a_stringPool)
		: _serializer(a_serializer),
		  _memoryStream(std::move(a_data)),
		  _stringPool(std::move(a_stringPool))
	{
	}

	bool SaveStream::ReadValue(pointer a_buffer, size_type a_size) const
	{
		return _memoryStream.read(a_buffer, a_size);
	}

	bool SaveStream::WriteValue(const_pointer a_data, size_type a_size)
	{
		return _memoryStream.write(a_data, a_size);
	}

	bool SaveStream::ReadStringRaw(std::string& a_outValue) const
	{
		auto size = static_cast<std::uint32_t>(0);
		if (!_memoryStream.read(size)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.clear();
			return false;
		}

		if (size == 0) {
			a_outValue.clear();
			return true;
		}

		auto string = std::string(size, '\0');
		if (!_memoryStream.read(string.data(), size)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.clear();
			return false;
		}

		a_outValue = string;
		return true;
	}

	bool SaveStream::ReadStringRaw(RE::BSFixedString& a_outValue) const
	{
		auto rawResult = std::string();
		if (!ReadStringRaw(rawResult)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.clear();
			return false;
		}

		a_outValue = RE::BSFixedString(rawResult);
		return true;
	}

	bool SaveStream::ReadStringRaw(RE::BSFixedStringCS& a_outValue) const
	{
		auto rawResult = std::string();
		if (!ReadStringRaw(rawResult)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.clear();
			return false;
		}

		a_outValue = RE::BSFixedStringCS(rawResult);
		return true;
	}

	bool SaveStream::WriteStringRaw(std::string_view a_value)
	{
		const auto size = static_cast<std::uint32_t>(a_value.size());
		if (!_memoryStream.write(size)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (size == 0) {
			return true;
		}

		if (!_memoryStream.write<char>(a_value)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool SaveStream::ReadStringPool(RE::BSFixedString& a_outValue) const
	{
		if (!_stringPool) {
			a_outValue.clear();
			return false;
		}

		auto csResult = RE::BSFixedStringCS();
		if (!ReadStringPool(csResult)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.clear();
			return false;
		}

		a_outValue = csResult;
		return true;
	}

	bool SaveStream::WriteStringPool(const RE::BSFixedString& a_value)
	{
		if (!_stringPool) {
			return false;
		}

		const auto csValue = RE::BSFixedStringCS(a_value);
		return WriteStringPool(csValue);
	}

	bool SaveStream::ReadStringPool(RE::BSFixedStringCS& a_outValue) const
	{
		if (!_stringPool) {
			a_outValue.clear();
			return false;
		}

		auto index = static_cast<std::uint32_t>(0);
		if (!_memoryStream.read(index)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.clear();
			return false;
		}

		if (!_stringPool->GetString(index, a_outValue)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.clear();
			return false;
		}

		return true;
	}

	bool SaveStream::WriteStringPool(const RE::BSFixedStringCS& a_value)
	{
		if (!_stringPool) {
			return false;
		}

		const auto index = _stringPool->AddString(a_value);
		if (!_memoryStream.write(index)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool SaveStream::ReadString(RE::BSFixedString& a_outValue) const
	{
		if (_stringPool) {
			return ReadStringPool(a_outValue);
		}

		return ReadStringRaw(a_outValue);
	}

	bool SaveStream::WriteString(const RE::BSFixedString& a_value)
	{
		if (_stringPool) {
			return WriteStringPool(a_value);
		}

		return WriteStringRaw(a_value);
	}

	bool SaveStream::ReadString(RE::BSFixedStringCS& a_outValue) const
	{
		if (_stringPool) {
			return ReadStringPool(a_outValue);
		}

		return ReadStringRaw(a_outValue);
	}

	bool SaveStream::WriteString(const RE::BSFixedStringCS& a_value)
	{
		if (_stringPool) {
			return WriteStringPool(a_value);
		}

		return WriteStringRaw(a_value);
	}

	bool SaveStream::ReadObject(RE::BSTSmartPointer<RE::BSScript::Object>& a_outValue) const
	{
		auto readHandle = EMPTY_SAVE_HANDLE;
		if (!_memoryStream.read(readHandle)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.reset();
			return false;
		}

		if (readHandle == 0) {
			a_outValue.reset();
			return true;
		}

		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		if (!internalVM->GetObjectBySaveHandle(readHandle, a_outValue) || !a_outValue) [[unlikely]] {
			REX::Assert(false);
			a_outValue.reset();
			return false;
		}

		return true;
	}

	bool SaveStream::WriteObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_value)
	{
		if (!a_value) {
			return _memoryStream.write(EMPTY_SAVE_HANDLE);
		}

		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		const auto saveHandle = internalVM->GetSaveHandleForObject(a_value.get());
		if (saveHandle == 0) [[unlikely]] {
			REX::Assert(false);
			return _memoryStream.write(EMPTY_SAVE_HANDLE);
		}

		if (!_memoryStream.write(saveHandle)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool SaveStream::ReadStruct(RE::BSTSmartPointer<RE::BSScript::Struct>& a_outValue) const
	{
		auto readHandle = EMPTY_SAVE_HANDLE;
		if (!_memoryStream.read(readHandle)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.reset();
			return false;
		}

		if (readHandle == 0) {
			a_outValue.reset();
			return true;
		}

		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		if (!internalVM->GetStructBySaveHandle(readHandle, a_outValue) || !a_outValue) [[unlikely]] {
			REX::Assert(false);
			a_outValue.reset();
			return false;
		}

		return true;
	}

	bool SaveStream::WriteStruct(const RE::BSTSmartPointer<RE::BSScript::Struct>& a_value)
	{
		if (!a_value) {
			return _memoryStream.write(EMPTY_SAVE_HANDLE);
		}

		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		const auto saveHandle = internalVM->GetSaveHandleForStruct(a_value.get());
		if (saveHandle == 0) [[unlikely]] {
			REX::Assert(false);
			return _memoryStream.write(EMPTY_SAVE_HANDLE);
		}

		if (!_memoryStream.write(saveHandle)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool SaveStream::ReadArray(RE::BSTSmartPointer<RE::BSScript::Array>& a_outValue) const
	{
		auto readHandle = EMPTY_SAVE_HANDLE;
		if (!_memoryStream.read(readHandle)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.reset();
			return false;
		}

		if (readHandle == 0) {
			a_outValue.reset();
			return true;
		}

		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		if (!internalVM->GetArrayBySaveHandle(readHandle, a_outValue) || !a_outValue) [[unlikely]] {
			REX::Assert(false);
			a_outValue.reset();
			return false;
		}

		return true;
	}

	bool SaveStream::WriteArray(const RE::BSTSmartPointer<RE::BSScript::Array>& a_value)
	{
		if (!a_value) {
			return _memoryStream.write(EMPTY_SAVE_HANDLE);
		}

		const auto internalVM = RE::GameVM::GetInternalVM();
		if (!internalVM) [[unlikely]] {
			REX::Fail("Failed to get the game's internal script VM."sv);
		}

		const auto saveHandle = internalVM->GetSaveHandleForArray(a_value.get());
		if (saveHandle == 0) [[unlikely]] {
			REX::Assert(false);
			return _memoryStream.write(EMPTY_SAVE_HANDLE);
		}

		if (!_memoryStream.write(saveHandle)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool SaveStream::ReadVar(RE::BSScript::UniqueVariant& a_outValue) const
	{
		auto value = RE::BSScript::Variable();
		if (!ReadValue(value)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.reset();
			return false;
		}

		a_outValue = RE::BSScript::CreateVariant(std::move(value));
		return true;
	}

	bool SaveStream::WriteVar(RE::BSScript::SharedVariant a_value)
	{
		return WriteValue(RE::BSScript::UnwrapVariant(a_value));
	}

	bool SaveStream::ReadValue(RE::BSScript::Variable& a_outValue) const
	{
		auto typeId = static_cast<RawType>(0);
		if (!_memoryStream.read(typeId)) [[unlikely]] {
			REX::Assert(false);
			a_outValue.reset();
			return false;
		}

		switch (typeId) {
			case RawType::kNone: {
				auto value = std::nullptr_t();
				if (!_memoryStream.read(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = value;
				return true;
			}
			case RawType::kBool: {
				auto value = bool();
				if (!_memoryStream.read(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = value;
				return true;
			}
			case RawType::kInt: {
				auto value = std::int32_t();
				if (!_memoryStream.read(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = value;
				return true;
			}
			case RawType::kFloat: {
				auto value = REX::Float32();
				if (!_memoryStream.read(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = value;
				return true;
			}
			case RawType::kString: {
				auto value = RE::BSFixedString();
				if (!ReadString(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = std::move(value);
				return true;
			}
			case RawType::kObject: {
				auto value = RE::BSTSmartPointer<RE::BSScript::Object>();
				if (!ReadObject(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = std::move(value);
				return true;
			}
			case RawType::kStruct: {
				auto value = RE::BSTSmartPointer<RE::BSScript::Struct>();
				if (!ReadStruct(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = std::move(value);
				return true;
			}
			case RawType::kVar: {
				auto value = RE::BSScript::UniqueVariant();
				if (!ReadVar(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = std::move(value);
				return true;
			}
			case RawType::kArrayBool:
			case RawType::kArrayInt:
			case RawType::kArrayFloat:
			case RawType::kArrayString:
			case RawType::kArrayObject:
			case RawType::kArrayStruct:
			case RawType::kArrayVar: {
				auto value = RE::BSTSmartPointer<RE::BSScript::Array>();
				if (!ReadArray(value)) [[unlikely]] {
					REX::Assert(false);
					a_outValue.reset();
					return false;
				}

				a_outValue = std::move(value);
				return true;
			}
			default: {
				std::unreachable();
			}
		}
	}

	bool SaveStream::WriteValue(const RE::BSScript::Variable& a_value)
	{
		const auto typeId = a_value.GetRawType();
		if (!_memoryStream.write(typeId)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		switch (typeId) {
			case RawType::kNone: {
				const auto value = a_value.GetNone();
				return _memoryStream.write(value);
			}
			case RawType::kBool: {
				const auto value = a_value.GetBool();
				return _memoryStream.write(value);
			}
			case RawType::kInt: {
				const auto value = a_value.GetInt();
				return _memoryStream.write(value);
			}
			case RawType::kFloat: {
				const auto value = a_value.GetFloat();
				return _memoryStream.write(value);
			}
			case RawType::kString: {
				const auto value = a_value.GetString();
				return WriteString(value);
			}
			case RawType::kObject: {
				const auto value = a_value.GetObject();
				return WriteObject(value);
			}
			case RawType::kStruct: {
				const auto value = a_value.GetStruct();
				return WriteStruct(value);
			}
			case RawType::kVar: {
				const auto* value = a_value.GetVar();
				return WriteVar(value);
			}
			case RawType::kArrayBool:
			case RawType::kArrayInt:
			case RawType::kArrayFloat:
			case RawType::kArrayString:
			case RawType::kArrayObject:
			case RawType::kArrayStruct:
			case RawType::kArrayVar: {
				const auto value = a_value.GetArray();
				return WriteArray(value);
			}
			default: {
				std::unreachable();
			}
		}
	}

	auto SaveStream::ResolveObjectHandle(RE::BSScript::ObjectHandle a_objectHandle) const -> std::optional<RE::BSScript::ObjectHandle>
	{
		return _serializer->ResolveHandle(a_objectHandle);
	}

	auto SaveStream::ResolveFormId(RE::TESFormID a_formId) const -> std::optional<RE::TESFormID>
	{
		return _serializer->ResolveFormID(a_formId);
	}
}
