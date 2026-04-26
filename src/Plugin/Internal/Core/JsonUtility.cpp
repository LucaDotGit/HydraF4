#include "Plugin/Internal/Core/JsonUtility.hpp"

namespace Plugin::Internal::Core::JsonUtility
{
	const json_t::parser_callback_t& GetParserCallback() noexcept
	{
		return REX::Json::GetParserCallback<json_t>();
	}

	auto ToInt(const json_t& a_value) -> std::optional<std::int32_t>
	{
		using value_type = json_t::value_t;

		switch (a_value.type()) {
			case value_type::number_integer: {
				return static_cast<std::int32_t>(a_value.get<json_t::number_integer_t>());
			}
			case value_type::number_unsigned: {
				return static_cast<std::int32_t>(a_value.get<json_t::number_unsigned_t>());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToUInt(const json_t& a_value) -> std::optional<std::uint32_t>
	{
		using value_type = json_t::value_t;

		switch (a_value.type()) {
			case value_type::number_integer: {
				return static_cast<std::uint32_t>(a_value.get<json_t::number_integer_t>());
			}
			case value_type::number_unsigned: {
				return static_cast<std::uint32_t>(a_value.get<json_t::number_unsigned_t>());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToFloat(const json_t& a_value) -> std::optional<REX::Float32>
	{
		using value_type = json_t::value_t;

		switch (a_value.type()) {
			case value_type::number_integer: {
				return static_cast<REX::Float32>(a_value.get<json_t::number_integer_t>());
			}
			case value_type::number_unsigned: {
				return static_cast<REX::Float32>(a_value.get<json_t::number_unsigned_t>());
			}
			case value_type::number_float: {
				return static_cast<REX::Float32>(a_value.get<json_t::number_float_t>());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToDouble(const json_t& a_value) -> std::optional<REX::Float64>
	{
		using value_type = json_t::value_t;

		switch (a_value.type()) {
			case value_type::number_integer: {
				return static_cast<REX::Float64>(a_value.get<json_t::number_integer_t>());
			}
			case value_type::number_unsigned: {
				return static_cast<REX::Float64>(a_value.get<json_t::number_unsigned_t>());
			}
			case value_type::number_float: {
				return static_cast<REX::Float64>(a_value.get<json_t::number_float_t>());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto FromFormData(const Core::TypeSerializer::FormData& a_value) -> json_t
	{
		auto formString = TypeSerializer::SerializeFormData(a_value);
		return { std::move(formString) };
	}

	auto ToFormData(const json_t& a_value) -> std::optional<Core::TypeSerializer::FormData>
	{
		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto formString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeFormData(formString);
	}

	auto FromAliasData(const Core::TypeSerializer::AliasData& a_value) -> json_t
	{
		auto aliasString = TypeSerializer::SerializeAliasData(a_value);
		return { std::move(aliasString) };
	}

	auto ToAliasData(const json_t& a_value) -> std::optional<Core::TypeSerializer::AliasData>
	{
		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto aliasString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeAliasData(aliasString);
	}

	auto FromInventoryRefData(const Core::TypeSerializer::InventoryRefData& a_value) -> json_t
	{
		auto inventoryRefString = TypeSerializer::SerializeInventoryRefData(a_value);
		return { std::move(inventoryRefString) };
	}

	auto ToInventoryRefData(const json_t& a_value) -> std::optional<Core::TypeSerializer::InventoryRefData>
	{
		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto inventoryRefString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeInventoryRefData(inventoryRefString);
	}

	auto FromActiveEffectData(const Core::TypeSerializer::ActiveEffectData& a_value) -> json_t
	{
		auto activeEffectString = TypeSerializer::SerializeActiveEffectData(a_value);
		return { std::move(activeEffectString) };
	}

	auto ToActiveEffectData(const json_t& a_value) -> std::optional<Core::TypeSerializer::ActiveEffectData>
	{
		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto activeEffectString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeActiveEffectData(activeEffectString);
	}

	auto FromInputLayerData(const Core::TypeSerializer::InputLayerData& a_value) -> json_t
	{
		auto inputLayerString = TypeSerializer::SerializeInputLayerData(a_value);
		return { std::move(inputLayerString) };
	}

	auto ToInputLayerData(const json_t& a_value) -> std::optional<Core::TypeSerializer::InputLayerData>
	{
		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto inputLayerString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeInputLayerData(inputLayerString);
	}

	auto FromForm(const Core::TypeSerializer::FormVariant& a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		auto formString = TypeSerializer::SerializeForm(a_value);
		if (!formString.has_value()) {
			return std::nullopt;
		}

		auto jsonForm = json_t();
		jsonForm = std::move(formString).value();
		return jsonForm;
	}

	auto ToForm(const json_t& a_value) -> std::optional<Core::TypeSerializer::FormVariant>
	{
		if (a_value.is_null()) {
			return Core::TypeSerializer::FormVariant(nullptr);
		}

		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto formString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeForm(formString);
	}

	auto FromAlias(const RE::BGSBaseAlias* a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		auto aliasString = TypeSerializer::SerializeAlias(a_value);
		if (!aliasString.has_value()) {
			return std::nullopt;
		}

		auto jsonAlias = json_t();
		jsonAlias = std::move(aliasString).value();
		return jsonAlias;
	}

	auto ToAlias(const json_t& a_value) -> std::optional<RE::BGSBaseAlias*>
	{
		if (a_value.is_null()) {
			return static_cast<RE::BGSBaseAlias*>(nullptr);
		}

		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto aliasString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeAlias(aliasString);
	}

	auto FromInventoryRef(const Core::TypeSerializer::InventoryItemRef& a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		auto inventoryRefString = TypeSerializer::SerializeInventoryRef(a_value);
		if (!inventoryRefString.has_value()) {
			return std::nullopt;
		}

		auto jsonInventoryRef = json_t();
		jsonInventoryRef = std::move(inventoryRefString).value();
		return jsonInventoryRef;
	}

	auto ToInventoryRef(const json_t& a_value) -> std::optional<Core::TypeSerializer::InventoryItemRef>
	{
		if (a_value.is_null()) {
			return Core::TypeSerializer::InventoryItemRef(nullptr);
		}

		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto inventoryRefString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeInventoryRef(inventoryRefString);
	}

	auto FromActiveEffect(const RE::BSTSmartPointer<RE::ActiveEffect>& a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		auto activeEffectString = TypeSerializer::SerializeActiveEffect(a_value);
		if (!activeEffectString.has_value()) {
			return std::nullopt;
		}

		auto jsonActiveEffect = json_t();
		jsonActiveEffect = std::move(activeEffectString).value();
		return jsonActiveEffect;
	}

	auto ToActiveEffect(const json_t& a_value) -> std::optional<RE::BSTSmartPointer<RE::ActiveEffect>>
	{
		if (a_value.is_null()) {
			return RE::BSTSmartPointer<RE::ActiveEffect>(nullptr);
		}

		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto activeEffectString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeActiveEffect(activeEffectString);
	}

	auto FromInputLayer(const RE::BSTSmartPointer<RE::BSInputEnableLayer>& a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		auto inputLayerString = TypeSerializer::SerializeInputLayer(a_value);
		if (!inputLayerString.has_value()) {
			return std::nullopt;
		}

		auto jsonInputLayer = json_t();
		jsonInputLayer = std::move(inputLayerString).value();
		return jsonInputLayer;
	}

	auto ToInputLayer(const json_t& a_value) -> std::optional<RE::BSTSmartPointer<RE::BSInputEnableLayer>>
	{
		if (a_value.is_null()) {
			return RE::BSTSmartPointer<RE::BSInputEnableLayer>(nullptr);
		}

		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto inputLayerString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeInputLayer(inputLayerString);
	}

	auto FromObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		auto objectString = TypeSerializer::SerializeObject(a_value);
		if (!objectString.has_value()) {
			return std::nullopt;
		}

		auto jsonObject = json_t();
		jsonObject = std::move(objectString).value();
		return jsonObject;
	}

	auto ToObject(const json_t& a_value) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Object>>
	{
		if (a_value.is_null()) {
			return RE::BSTSmartPointer<RE::BSScript::Object>(nullptr);
		}

		if (!a_value.is_string()) {
			return std::nullopt;
		}

		const auto objectString = a_value.get<std::string_view>();
		return TypeSerializer::DeserializeObject(objectString);
	}

	auto FromStruct(const RE::BSTSmartPointer<RE::BSScript::Struct>& a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		const auto structTypeInfo = a_value->GetTypeInfo();
		if (!structTypeInfo) {
			return std::nullopt;
		}

		const auto structLock = RE::BSAutoLock(a_value->structLock);

		auto jsonStruct = json_t();

		for (const auto& [scriptKey, varIndex] : structTypeInfo->varNameIndexMap) {
			const auto& scriptElement = a_value->variables[varIndex];

			auto scriptValue = FromValue(scriptElement);
			if (!scriptValue.has_value()) {
				return std::nullopt;
			}

			auto scriptName = std::string{ scriptKey.data(), scriptKey.size() };
			jsonStruct.emplace(std::move(scriptName), std::move(scriptValue).value());
		}

		return jsonStruct;
	}

	auto ToStruct(const json_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Struct>>
	{
		if (a_value.is_null()) {
			return RE::BSTSmartPointer<RE::BSScript::Struct>(nullptr);
		}

		if (!a_value.is_object()) {
			return std::nullopt;
		}

		const auto* structTypeInfo = a_typeInfo.GetStructTypeInfo();
		if (!structTypeInfo) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		auto scriptStruct = RE::BSTSmartPointer<RE::BSScript::Struct>();
		if (!scriptVM->CreateStruct(structTypeInfo->GetName(), scriptStruct) || !scriptStruct) {
			return std::nullopt;
		}

		const auto structLock = RE::BSAutoLock(scriptStruct->structLock);

		for (const auto& [jsonKey, jsonValue] : a_value.items()) {
			auto* scriptElement = scriptStruct->GetVariable(RE::BSFixedString(jsonKey));
			if (!scriptElement) {
				continue;
			}

			auto scriptValue = ToValue(jsonValue, scriptElement->GetTypeInfo());
			if (!scriptValue.has_value()) {
				continue;
			}

			if (!scriptElement->TryAssign(std::move(scriptValue).value())) [[unlikely]] {
				REX::Assert(false);
				continue;
			}
		}

		return scriptStruct;
	}

	auto FromArray(const RE::BSTSmartPointer<RE::BSScript::Array>& a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		const auto arrayLock = RE::BSAutoLock(a_value->elementsLock);
		const auto arraySize = static_cast<std::uint32_t>(a_value->size());

		auto jsonArray = json_t::array();
		jsonArray.get_ref<json_t::array_t&>().reserve(arraySize);

		for (auto i = 0ui32; i < arraySize; i++) {
			const auto& scriptElement = (*a_value)[i];

			auto scriptValue = FromValue(scriptElement);
			if (!scriptValue.has_value()) {
				return std::nullopt;
			}

			jsonArray.emplace_back(std::move(scriptValue).value());
		}

		return jsonArray;
	}

	auto ToArray(const json_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Array>>
	{
		if (a_value.is_null()) {
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

			auto scriptValue = ToValue(a_value[i], scriptElement.GetTypeInfo());
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

	auto FromVar(RE::BSScript::SharedVariant a_value) -> std::optional<json_t>
	{
		if (!a_value) {
			return json_t(nullptr);
		}

		return FromValue(*a_value);
	}

	auto ToVar(const json_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::UniqueVariant>
	{
		if (a_value.is_null()) {
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

	auto FromValue(const RE::BSScript::Variable& a_value) -> std::optional<json_t>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_value.GetRawType()) {
			case RawType::kNone: {
				return json_t(a_value.GetNone());
			}
			case RawType::kBool: {
				return json_t(a_value.GetBool());
			}
			case RawType::kInt: {
				return json_t(a_value.GetInt());
			}
			case RawType::kFloat: {
				return json_t(a_value.GetFloat());
			}
			case RawType::kString: {
				return json_t(static_cast<std::string_view>(a_value.GetString()));
			}
			case RawType::kObject: {
				return FromObject(a_value.GetObject());
			}
			case RawType::kStruct: {
				return FromStruct(a_value.GetStruct());
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

	auto ToValue(const json_t& a_value, const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::Variable>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_typeInfo.GetRawType()) {
			case RawType::kNone: {
				if (!a_value.is_null()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(a_value.get<std::nullptr_t>());
			}
			case RawType::kBool: {
				if (!a_value.is_boolean()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(a_value.get<json_t::boolean_t>());
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
				if (!a_value.is_string()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(RE::BSFixedString(a_value.get<std::string_view>()));
			}
			case RawType::kObject: {
				auto object = ToObject(a_value);
				if (!object.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(std::move(object).value());
			}
			case RawType::kStruct: {
				auto structure = ToStruct(a_value, a_typeInfo);
				if (!structure.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(std::move(structure).value());
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

	auto ToDetectedValue(const json_t& a_value) -> std::optional<RE::BSScript::Variable>
	{
		using value_type = json_t::value_t;

		switch (a_value.type()) {
			case value_type::null:
			case value_type::discarded: {
				return RE::BSScript::Variable(nullptr);
			}
			case value_type::boolean: {
				return RE::BSScript::Variable(a_value.get<json_t::boolean_t>());
			}
			case value_type::number_integer: {
				return RE::BSScript::Variable(static_cast<std::int32_t>(a_value.get<json_t::number_integer_t>()));
			}
			case value_type::number_unsigned: {
				return RE::BSScript::Variable(static_cast<std::uint32_t>(a_value.get<json_t::number_unsigned_t>()));
			}
			case value_type::number_float: {
				return RE::BSScript::Variable(static_cast<REX::Float32>(a_value.get<json_t::number_float_t>()));
			}
			case value_type::string: {
				auto object = ToObject(a_value);
				if (object.has_value()) {
					return RE::BSScript::Variable(std::move(object).value());
				}

				return RE::BSScript::Variable(RE::BSFixedString(a_value.get<std::string_view>()));
			}
			case value_type::array: {
				auto array = ToArray(a_value, RE::BSScript::TypeInfo::RawType::kVar);
				if (!array.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(std::move(array).value());
			}
			case value_type::object:
			case value_type::binary: {
				return RE::BSScript::Variable(nullptr);
			}
			default: {
				std::unreachable();
			}
		}
	}

	auto GetPointer(std::string_view a_key) -> std::expected<json_t::json_pointer, json_t::exception>
	{
		try {
			auto key = std::string(a_key);
			if (!key.empty() && !key.starts_with(KEY_SEPARATOR)) {
				key.insert(0, 1, KEY_SEPARATOR);
			}

			return json_t::json_pointer(key);
		}
		catch (const json_t::exception& error) {
			return std::unexpected(error);
		}
	}
}
