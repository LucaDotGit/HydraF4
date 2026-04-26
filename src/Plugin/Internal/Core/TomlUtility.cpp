#include "Plugin/Internal/Core/TomlUtility.hpp"

namespace Plugin::Internal::Core::TomlUtility
{
	auto ToInt(const toml_t& a_value) -> std::optional<std::int32_t>
	{
		using value_type = toml::value_t;

		switch (a_value.type()) {
			case value_type::integer: {
				return static_cast<std::int32_t>(a_value.as_integer());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToUInt(const toml_t& a_value) -> std::optional<std::uint32_t>
	{
		using value_type = toml::value_t;

		switch (a_value.type()) {
			case value_type::integer: {
				return static_cast<std::uint32_t>(a_value.as_integer());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToFloat(const toml_t& a_value) -> std::optional<REX::Float32>
	{
		using value_type = toml::value_t;

		switch (a_value.type()) {
			case value_type::integer: {
				return static_cast<REX::Float32>(a_value.as_integer());
			}
			case value_type::floating: {
				return static_cast<REX::Float32>(a_value.as_floating());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToDouble(const toml_t& a_value) -> std::optional<REX::Float64>
	{
		using value_type = toml::value_t;

		switch (a_value.type()) {
			case value_type::integer: {
				return static_cast<REX::Float64>(a_value.as_integer());
			}
			case value_type::floating: {
				return static_cast<REX::Float64>(a_value.as_floating());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToString(const toml_t& a_value) -> std::optional<std::string>
	{
		using value_type = toml::value_t;

		switch (a_value.type()) {
			case value_type::string: {
				return a_value.as_string();
			}
			case value_type::offset_datetime: {
				return toml::to_string(a_value.as_offset_datetime());
			}
			case value_type::local_datetime: {
				return toml::to_string(a_value.as_local_datetime());
			}
			case value_type::local_date: {
				return toml::to_string(a_value.as_local_date());
			}
			case value_type::local_time: {
				return toml::to_string(a_value.as_local_time());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto FromArray(const RE::BSTSmartPointer<RE::BSScript::Array>& a_value) -> std::optional<toml_t>
	{
		if (!a_value) {
			return toml_t();
		}

		const auto arrayLock = RE::BSAutoLock(a_value->elementsLock);
		const auto arraySize = static_cast<std::uint32_t>(a_value->size());

		auto tomlArray = toml_t::array_type();
		tomlArray.reserve(arraySize);

		for (auto i = 0ui32; i < arraySize; i++) {
			const auto& scriptElement = (*a_value)[i];

			auto scriptValue = FromValue(scriptElement);
			if (!scriptValue.has_value()) {
				return std::nullopt;
			}

			tomlArray.emplace_back(std::move(scriptValue).value());
		}

		return tomlArray;
	}

	auto ToArray(const toml_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Array>>
	{
		if (a_value.is_empty()) {
			return RE::BSTSmartPointer<RE::BSScript::Array>(nullptr);
		}

		if (!a_value.is_array()) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto arraySize = static_cast<std::uint32_t>(a_value.size());

		auto scriptArray = RE::BSTSmartPointer<RE::BSScript::Array>();
		if (!scriptVM->CreateArray(a_typeInfo, arraySize, scriptArray) || !scriptArray) {
			return std::nullopt;
		}

		const auto arrayLock = RE::BSAutoLock(scriptArray->elementsLock);

		for (auto i = 0ui32; i < arraySize; i++) {
			auto& scriptElement = (*scriptArray)[i];

			auto scriptValue = ToValue(a_value.as_array()[i], scriptElement.GetTypeInfo());
			if (!scriptValue.has_value()) {
				continue;
			}

			if (!scriptElement.TryAssign(std::move(scriptValue).value())) [[unlikely]] {
				REX::Assert(false);
				continue;
			}
		}

		return scriptArray;
	}

	auto FromVar(RE::BSScript::SharedVariant a_value) -> std::optional<toml_t>
	{
		if (!a_value) {
			return toml_t();
		}

		return FromValue(*a_value);
	}

	auto ToVar(const toml_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::UniqueVariant>
	{
		if (a_value.is_empty()) {
			return static_cast<RE::BSScript::UniqueVariant>(nullptr);
		}

		if (a_typeInfo.IsVar()) {
			auto detectedValue = ToDetectedValue(a_value);
			if (!detectedValue.has_value()) {
				return std::nullopt;
			}

			return RE::BSScript::CreateVariant(std::move(detectedValue).value());
		}

		auto scriptValue = ToValue(a_value, a_typeInfo);
		if (!scriptValue.has_value()) {
			return std::nullopt;
		}

		return RE::BSScript::CreateVariant(std::move(scriptValue).value());
	}

	auto FromValue(const RE::BSScript::Variable& a_value) -> std::optional<toml_t>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_value.GetRawType()) {
			case RawType::kNone: {
				return toml_t();
			}
			case RawType::kBool: {
				return toml_t(a_value.GetBool());
			}
			case RawType::kInt: {
				return toml_t(a_value.GetInt());
			}
			case RawType::kFloat: {
				return toml_t(a_value.GetFloat());
			}
			case RawType::kString: {
				return toml_t(static_cast<std::string_view>(a_value.GetString()));
			}
			case RawType::kObject:
			case RawType::kStruct: {
				return std::nullopt;
			}
			case RawType::kVar: {
				return FromVar(a_value.GetVar());
			}
			case RawType::kArrayBool:
			case RawType::kArrayInt:
			case RawType::kArrayFloat:
			case RawType::kArrayString:
			case RawType::kArrayObject:
			case RawType::kArrayStruct:
			case RawType::kArrayVar: {
				return FromArray(a_value.GetArray());
			}
			default: {
				std::unreachable();
			}
		}
	}

	auto ToValue(const toml_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::Variable>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_typeInfo.GetRawType()) {
			case RawType::kNone: {
				if (!a_value.is_empty()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(nullptr);
			}
			case RawType::kBool: {
				if (!a_value.is_boolean()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(a_value.as_boolean());
			}
			case RawType::kInt: {
				auto int32 = ToInt(a_value);
				if (!int32.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(int32.value());
			}
			case RawType::kFloat: {
				auto float32 = ToFloat(a_value);
				if (!float32.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(float32.value());
			}
			case RawType::kString: {
				auto string = ToString(a_value);
				if (!string.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(RE::BSFixedString(std::move(string).value()));
			}
			case RawType::kObject:
			case RawType::kStruct: {
				return std::nullopt;
			}
			case RawType::kVar: {
				auto var = ToVar(a_value, a_typeInfo);
				if (!var.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(std::move(var).value());
			}
			case RawType::kArrayBool:
			case RawType::kArrayInt:
			case RawType::kArrayFloat:
			case RawType::kArrayString:
			case RawType::kArrayObject:
			case RawType::kArrayStruct:
			case RawType::kArrayVar: {
				auto array = ToArray(a_value, a_typeInfo);
				if (!array.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(std::move(array).value());
			}
			default: {
				std::unreachable();
			}
		}
	}

	auto ToDetectedValue(const toml_t& a_value) -> std::optional<RE::BSScript::Variable>
	{
		using value_type = toml::value_t;

		switch (a_value.type()) {
			case value_type::empty: {
				return RE::BSScript::Variable(nullptr);
			}
			case value_type::boolean: {
				return RE::BSScript::Variable(a_value.as_boolean());
			}
			case value_type::integer: {
				return RE::BSScript::Variable(static_cast<std::int32_t>(a_value.as_integer()));
			}
			case value_type::floating: {
				return RE::BSScript::Variable(static_cast<REX::Float32>(a_value.as_floating()));
			}
			case value_type::string: {
				return RE::BSScript::Variable(RE::BSFixedString(a_value.as_string()));
			}
			case value_type::offset_datetime: {
				auto datetimeString = toml::to_string(a_value.as_offset_datetime());
				return RE::BSScript::Variable(RE::BSFixedString(std::move(datetimeString)));
			}
			case value_type::local_datetime: {
				auto datetimeString = toml::to_string(a_value.as_local_datetime());
				return RE::BSScript::Variable(RE::BSFixedString(std::move(datetimeString)));
			}
			case value_type::local_date: {
				auto dateString = toml::to_string(a_value.as_local_date());
				return RE::BSScript::Variable(RE::BSFixedString(std::move(dateString)));
			}
			case value_type::local_time: {
				auto timeString = toml::to_string(a_value.as_local_time());
				return RE::BSScript::Variable(RE::BSFixedString(std::move(timeString)));
			}
			case value_type::array: {
				auto array = ToArray(a_value, RE::BSScript::TypeInfo::RawType::kVar);
				if (!array.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(std::move(array).value());
			}
			case value_type::table: {
				return RE::BSScript::Variable(nullptr);
			}
			default: {
				std::unreachable();
			}
		}
	}
}
