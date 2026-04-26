#include "Plugin/Internal/Core/IniUtility.hpp"

namespace Plugin::Internal::Core::IniUtility
{
	void SetupFile(ini_t& a_file)
	{
		a_file.SetAllowKeyOnly(ALLOW_KEY_ONLY);
		a_file.SetUnicode(USE_UNICODE);
		a_file.SetQuotes(USE_QUOTES);
	}

	auto ToInt(const value_t& a_value) -> std::optional<std::int32_t>
	{
		{
			const auto* intValue = std::get_if<std::int64_t>(std::addressof(a_value));
			if (intValue) {
				return static_cast<std::int32_t>(*intValue);
			}
		}

		{
			const auto* uintValue = std::get_if<std::uint64_t>(std::addressof(a_value));
			if (uintValue) {
				return static_cast<std::int32_t>(*uintValue);
			}
		}

		return std::nullopt;
	}

	auto ToUInt(const value_t& a_value) -> std::optional<std::uint32_t>
	{
		{
			const auto* intValue = std::get_if<std::int64_t>(std::addressof(a_value));
			if (intValue) {
				return static_cast<std::uint32_t>(*intValue);
			}
		}

		{
			const auto* uintValue = std::get_if<std::uint64_t>(std::addressof(a_value));
			if (uintValue) {
				return static_cast<std::uint32_t>(*uintValue);
			}
		}

		return std::nullopt;
	}

	auto ToFloat(const value_t& a_value) -> std::optional<REX::Float32>
	{
		{
			const auto* intValue = std::get_if<std::int64_t>(std::addressof(a_value));
			if (intValue) {
				return static_cast<REX::Float32>(*intValue);
			}
		}

		{
			const auto* uintValue = std::get_if<std::uint64_t>(std::addressof(a_value));
			if (uintValue) {
				return static_cast<REX::Float32>(*uintValue);
			}
		}

		{
			const auto* floatValue = std::get_if<REX::Float64>(std::addressof(a_value));
			if (floatValue) {
				return static_cast<REX::Float32>(*floatValue);
			}
		}

		return std::nullopt;
	}

	auto ToDouble(const value_t& a_value) -> std::optional<REX::Float64>
	{
		{
			const auto* intValue = std::get_if<std::int64_t>(std::addressof(a_value));
			if (intValue) {
				return static_cast<REX::Float64>(*intValue);
			}
		}

		{
			const auto* uintValue = std::get_if<std::uint64_t>(std::addressof(a_value));
			if (uintValue) {
				return static_cast<REX::Float64>(*uintValue);
			}
		}

		{
			const auto* floatValue = std::get_if<REX::Float64>(std::addressof(a_value));
			if (floatValue) {
				return static_cast<REX::Float64>(*floatValue);
			}
		}

		return std::nullopt;
	}

	auto FromVar(RE::BSScript::SharedVariant a_value) -> std::optional<value_t>
	{
		if (!a_value) {
			return value_t(nullptr);
		}

		return FromValue(*a_value);
	}

	auto ToVar(const value_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::UniqueVariant>
	{
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

	auto FromValue(const RE::BSScript::Variable& a_value) -> std::optional<value_t>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_value.GetRawType()) {
			case RawType::kNone: {
				return value_t(a_value.GetNone());
			}
			case RawType::kBool: {
				return value_t(a_value.GetBool());
			}
			case RawType::kInt: {
				return value_t(a_value.GetInt());
			}
			case RawType::kFloat: {
				return value_t(a_value.GetFloat());
			}
			case RawType::kString: {
				return value_t(static_cast<std::string_view>(a_value.GetString()));
			}
			case RawType::kObject:
			case RawType::kStruct: {
				return false;
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
				return false;
			}
			default: {
				std::unreachable();
			}
		}
	}

	auto ToValue(const value_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::Variable>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_typeInfo.GetRawType()) {
			case RawType::kNone: {
				const auto* value = std::get_if<std::nullptr_t>(std::addressof(a_value));
				if (!value) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(*value);
			}
			case RawType::kBool: {
				const auto* value = std::get_if<bool>(std::addressof(a_value));
				if (!value) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(*value);
			}
			case RawType::kInt: {
				const auto* value = std::get_if<std::int64_t>(std::addressof(a_value));
				if (!value) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(static_cast<std::int32_t>(*value));
			}
			case RawType::kFloat: {
				const auto* value = std::get_if<REX::Float64>(std::addressof(a_value));
				if (!value) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(static_cast<REX::Float32>(*value));
			}
			case RawType::kString: {
				const auto* value = std::get_if<std::string_view>(std::addressof(a_value));
				if (!value) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(RE::BSFixedString(*value));
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
				return std::nullopt;
			}
			default: {
				std::unreachable();
			}
		}
	}

	auto ToDetectedValue(const value_t& a_value) -> std::optional<RE::BSScript::Variable>
	{
		{
			const auto* value = std::get_if<std::nullptr_t>(std::addressof(a_value));
			if (value) {
				return RE::BSScript::Variable(nullptr);
			}
		}

		{
			const auto* value = std::get_if<bool>(std::addressof(a_value));
			if (value) {
				return RE::BSScript::Variable(*value);
			}
		}

		{
			const auto* value = std::get_if<std::int64_t>(std::addressof(a_value));
			if (value) {
				return RE::BSScript::Variable(static_cast<std::int32_t>(*value));
			}
		}

		{
			const auto* value = std::get_if<std::uint64_t>(std::addressof(a_value));
			if (value) {
				return RE::BSScript::Variable(static_cast<std::uint32_t>(*value));
			}
		}

		{
			const auto* value = std::get_if<REX::Float64>(std::addressof(a_value));
			if (value) {
				return RE::BSScript::Variable(static_cast<REX::Float32>(*value));
			}
		}

		{
			const auto* value = std::get_if<std::string_view>(std::addressof(a_value));
			if (value) {
				return RE::BSScript::Variable(RE::BSFixedString(*value));
			}
		}

		std::unreachable();
	}
}
