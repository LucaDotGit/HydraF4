#include "Plugin/Internal/Core/ScaleformUtility.hpp"

namespace Plugin::Internal::Core::ScaleformUtility::Impl
{
	static auto ToObject(std::string_view a_value) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Object>>
	{
		return TypeSerializer::DeserializeObject(a_value);
	}
}

namespace Plugin::Internal::Core::ScaleformUtility
{
	auto ToNull(const ::Scaleform::GFx::Value& a_value) -> std::optional<std::nullptr_t>
	{
		using ValueType = ::Scaleform::GFx::Value::ValueType;

		switch (a_value.GetType()) {
			case ValueType::kNull: {
				return a_value.GetNull();
			}
			case ValueType::kUndefined: {
				return a_value.GetUndefined();
			}
			default: {
				return std::nullopt;
			}
		}
	}

	// Integers may be interpreted as `Number` in AS3, even if no decimal point is specified.
	// So it will be allowed here to implicitly convert numbers to (unsigned) integers.

	auto ToInt(const ::Scaleform::GFx::Value& a_value) -> std::optional<std::int32_t>
	{
		using ValueType = ::Scaleform::GFx::Value::ValueType;

		switch (a_value.GetType()) {
			case ValueType::kInt: {
				return a_value.GetInt();
			}
			case ValueType::kUInt: {
				return static_cast<std::int32_t>(a_value.GetUInt());
			}
			case ValueType::kNumber: {
				return static_cast<std::int32_t>(a_value.GetNumber());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToUInt(const ::Scaleform::GFx::Value& a_value) -> std::optional<std::uint32_t>
	{
		using ValueType = ::Scaleform::GFx::Value::ValueType;

		switch (a_value.GetType()) {
			case ValueType::kInt: {
				return static_cast<std::uint32_t>(a_value.GetInt());
			}
			case ValueType::kUInt: {
				return a_value.GetUInt();
			}
			case ValueType::kNumber: {
				return static_cast<std::uint32_t>(a_value.GetNumber());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToFloat(const ::Scaleform::GFx::Value& a_value) -> std::optional<REX::Float32>
	{
		using ValueType = ::Scaleform::GFx::Value::ValueType;

		switch (a_value.GetType()) {
			case ValueType::kInt: {
				return static_cast<REX::Float32>(a_value.GetInt());
			}
			case ValueType::kUInt: {
				return static_cast<REX::Float32>(a_value.GetUInt());
			}
			case ValueType::kNumber: {
				return static_cast<REX::Float32>(a_value.GetNumber());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToDouble(const ::Scaleform::GFx::Value& a_value) -> std::optional<REX::Float64>
	{
		using ValueType = ::Scaleform::GFx::Value::ValueType;

		switch (a_value.GetType()) {
			case ValueType::kInt: {
				return static_cast<REX::Float64>(a_value.GetInt());
			}
			case ValueType::kUInt: {
				return static_cast<REX::Float64>(a_value.GetUInt());
			}
			case ValueType::kNumber: {
				return a_value.GetNumber();
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto FromString(std::string_view a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_movieRoot) {
			return std::nullopt;
		}

		auto result = ::Scaleform::GFx::Value();
		a_movieRoot->CreateString(std::addressof(result), a_value.data());
		return result;
	}

	auto ToString(const ::Scaleform::GFx::Value& a_value) -> std::optional<std::string>
	{
		switch (a_value.GetType()) {
			case ::Scaleform::GFx::Value::ValueType::kString: {
				return std::string(a_value.GetStringView());
			}
			case ::Scaleform::GFx::Value::ValueType::kStringW: {
				auto u8string = REX::Utf16ToUtf8(a_value.GetWStringView());
				if (!u8string.has_value()) {
					return std::nullopt;
				}

				return std::move(u8string).value();
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto ToFixedString(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSFixedString>
	{
		switch (a_value.GetType()) {
			case ::Scaleform::GFx::Value::ValueType::kString: {
				return RE::BSFixedString(a_value.GetStringView());
			}
			case ::Scaleform::GFx::Value::ValueType::kStringW: {
				auto u8string = REX::Utf16ToUtf8(a_value.GetWStringView());
				if (!u8string.has_value()) {
					return std::nullopt;
				}

				return RE::BSFixedString(std::move(u8string).value());
			}
			default: {
				return std::nullopt;
			}
		}
	}

	auto FromFormData(const Core::TypeSerializer::FormData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_movieRoot) {
			return std::nullopt;
		}

		const auto formString = TypeSerializer::SerializeFormData(a_value);

		auto result = ::Scaleform::GFx::Value();
		a_movieRoot->CreateString(std::addressof(result), formString.data());
		return result;
	}

	auto ToFormData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::FormData>
	{
		if (!a_value.IsString()) {
			return std::nullopt;
		}

		const auto formString = a_value.GetStringView();
		return TypeSerializer::DeserializeFormData(formString);
	}

	auto FromAliasData(const Core::TypeSerializer::AliasData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_movieRoot) {
			return std::nullopt;
		}

		const auto aliasString = TypeSerializer::SerializeAliasData(a_value);

		auto result = ::Scaleform::GFx::Value();
		a_movieRoot->CreateString(std::addressof(result), aliasString.data());
		return result;
	}

	auto ToAliasData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::AliasData>
	{
		if (!a_value.IsString()) {
			return std::nullopt;
		}

		const auto aliasString = a_value.GetStringView();
		return TypeSerializer::DeserializeAliasData(aliasString);
	}

	auto FromInventoryRefData(const Core::TypeSerializer::InventoryRefData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_movieRoot) {
			return std::nullopt;
		}

		const auto inventoryRefString = TypeSerializer::SerializeInventoryRefData(a_value);

		auto result = ::Scaleform::GFx::Value();
		a_movieRoot->CreateString(std::addressof(result), inventoryRefString.data());
		return result;
	}

	auto ToInventoryRefData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::InventoryRefData>
	{
		if (!a_value.IsString()) {
			return std::nullopt;
		}

		const auto inventoryRefString = a_value.GetStringView();
		return TypeSerializer::DeserializeInventoryRefData(inventoryRefString);
	}

	auto FromActiveEffectData(const Core::TypeSerializer::ActiveEffectData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_movieRoot) {
			return std::nullopt;
		}

		const auto activeEffectString = TypeSerializer::SerializeActiveEffectData(a_value);

		auto result = ::Scaleform::GFx::Value();
		a_movieRoot->CreateString(std::addressof(result), activeEffectString.data());
		return result;
	}

	auto ToActiveEffectData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::ActiveEffectData>
	{
		if (!a_value.IsString()) {
			return std::nullopt;
		}

		const auto activeEffectString = a_value.GetStringView();
		return TypeSerializer::DeserializeActiveEffectData(activeEffectString);
	}

	auto FromInputLayerData(const Core::TypeSerializer::InputLayerData& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_movieRoot) {
			return std::nullopt;
		}

		const auto inputLayerString = TypeSerializer::SerializeInputLayerData(a_value);

		auto result = ::Scaleform::GFx::Value();
		a_movieRoot->CreateString(std::addressof(result), inputLayerString.data());
		return result;
	}

	auto ToInputLayerData(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::InputLayerData>
	{
		if (!a_value.IsString()) {
			return std::nullopt;
		}

		const auto inputLayerString = a_value.GetStringView();
		return TypeSerializer::DeserializeInputLayerData(inputLayerString);
	}

	auto FromForm(const Core::TypeSerializer::FormVariant& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		auto formString = TypeSerializer::SerializeForm(a_value);
		if (!formString.has_value()) {
			return std::nullopt;
		}

		return FromString(formString.value(), a_movieRoot);
	}

	auto ToForm(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::FormVariant>
	{
		auto stringValue = ToString(a_value);
		if (!stringValue.has_value()) {
			return std::nullopt;
		}

		return TypeSerializer::DeserializeForm(stringValue.value());
	}

	auto FromAlias(const RE::BGSBaseAlias* a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		auto aliasString = TypeSerializer::SerializeAlias(a_value);
		if (!aliasString.has_value()) {
			return std::nullopt;
		}

		return FromString(aliasString.value(), a_movieRoot);
	}

	auto ToAlias(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BGSBaseAlias*>
	{
		auto stringValue = ToString(a_value);
		if (!stringValue.has_value()) {
			return std::nullopt;
		}

		return TypeSerializer::DeserializeAlias(stringValue.value());
	}

	auto FromInventoryRef(const Core::TypeSerializer::InventoryItemRef& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		auto inventoryRefString = TypeSerializer::SerializeInventoryRef(a_value);
		if (!inventoryRefString.has_value()) {
			return std::nullopt;
		}

		return FromString(inventoryRefString.value(), a_movieRoot);
	}

	auto ToInventoryRef(const ::Scaleform::GFx::Value& a_value) -> std::optional<Core::TypeSerializer::InventoryItemRef>
	{
		auto stringValue = ToString(a_value);
		if (!stringValue.has_value()) {
			return std::nullopt;
		}

		return TypeSerializer::DeserializeInventoryRef(stringValue.value());
	}

	auto FromActiveEffect(const RE::BSTSmartPointer<RE::ActiveEffect>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		auto activeEffectString = TypeSerializer::SerializeActiveEffect(a_value);
		if (!activeEffectString.has_value()) {
			return std::nullopt;
		}

		return FromString(activeEffectString.value(), a_movieRoot);
	}

	auto ToActiveEffect(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSTSmartPointer<RE::ActiveEffect>>
	{
		auto stringValue = ToString(a_value);
		if (!stringValue.has_value()) {
			return std::nullopt;
		}

		return TypeSerializer::DeserializeActiveEffect(stringValue.value());
	}

	auto FromInputLayer(const RE::BSTSmartPointer<RE::BSInputEnableLayer>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		auto inputLayerString = TypeSerializer::SerializeInputLayer(a_value);
		if (!inputLayerString.has_value()) {
			return std::nullopt;
		}

		return FromString(inputLayerString.value(), a_movieRoot);
	}

	auto ToInputLayer(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSTSmartPointer<RE::BSInputEnableLayer>>
	{
		auto stringValue = ToString(a_value);
		if (!stringValue.has_value()) {
			return std::nullopt;
		}

		return TypeSerializer::DeserializeInputLayer(stringValue.value());
	}

	auto FromObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		auto objectString = TypeSerializer::SerializeObject(a_value);
		if (!objectString.has_value()) {
			return std::nullopt;
		}

		return FromString(objectString.value(), a_movieRoot);
	}

	auto ToObject(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Object>>
	{
		auto stringValue = ToString(a_value);
		if (!stringValue.has_value()) {
			return std::nullopt;
		}

		return Impl::ToObject(stringValue.value());
	}

	auto FromStruct(const RE::BSTSmartPointer<RE::BSScript::Struct>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		const auto structTypeInfo = a_value->GetTypeInfo();
		if (!structTypeInfo) {
			return std::nullopt;
		}

		const auto structLock = RE::BSAutoLock(a_value->structLock);

		auto gfxStruct = ::Scaleform::GFx::Value();
		a_movieRoot->CreateObject(std::addressof(gfxStruct));

		for (const auto& [scriptKey, varIndex] : structTypeInfo->varNameIndexMap) {
			const auto& scriptElement = a_value->variables[varIndex];

			auto scriptValue = FromValue(scriptElement, a_movieRoot);
			if (!scriptValue.has_value()) {
				return std::nullopt;
			}

			if (!gfxStruct.SetMember(scriptKey.data(), scriptValue.value())) [[unlikely]] {
				REX::Assert(false);
				return std::nullopt;
			}
		}

		return gfxStruct;
	}

	auto ToStruct(const ::Scaleform::GFx::Value& a_value,
		const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Struct>>
	{
		if (a_value.IsNullOrUndefined()) {
			return RE::BSTSmartPointer<RE::BSScript::Struct>();
		}

		if (!a_value.IsObject()) {
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

		for (const auto& [scriptKey, varIndex] : structTypeInfo->varNameIndexMap) {
			auto* scriptElement = scriptStruct->GetVariable(scriptKey);
			if (!scriptElement) {
				continue;
			}

			auto gfxElement = ::Scaleform::GFx::Value();
			if (!a_value.GetMember(scriptKey.data(), std::addressof(gfxElement))) {
				continue;
			}

			auto scriptValue = ToValue(gfxElement, scriptElement->GetTypeInfo());
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

	auto FromArray(const RE::BSTSmartPointer<RE::BSScript::Array>& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		const auto arrayLock = RE::BSAutoLock(a_value->elementsLock);
		const auto arraySize = static_cast<std::uint32_t>(a_value->size());

		auto gfxArray = ::Scaleform::GFx::Value();
		a_movieRoot->CreateArray(std::addressof(gfxArray));

		gfxArray.SetArraySize(arraySize);

		for (auto i = 0ui32; i < arraySize; i++) {
			const auto& scriptElement = (*a_value)[i];

			auto scriptValue = FromValue(scriptElement, a_movieRoot);
			if (!scriptValue.has_value()) {
				return std::nullopt;
			}

			if (!gfxArray.SetElement(i, scriptValue.value())) [[unlikely]] {
				REX::Assert(false);
				return std::nullopt;
			}
		}

		return gfxArray;
	}

	auto ToArray(const ::Scaleform::GFx::Value& a_value,
		const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSTSmartPointer<RE::BSScript::Array>>
	{
		if (a_value.IsNullOrUndefined()) {
			return RE::BSTSmartPointer<RE::BSScript::Array>();
		}

		if (!a_value.IsArray()) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto arraySize = static_cast<std::uint32_t>(a_value.GetArraySize());

		auto scriptArray = RE::BSTSmartPointer<RE::BSScript::Array>();
		if (!scriptVM->CreateArray(a_typeInfo, arraySize, scriptArray) || !scriptArray) {
			return std::nullopt;
		}

		const auto arrayLock = RE::BSAutoLock(scriptArray->elementsLock);

		for (auto i = 0ui32; i < arraySize; i++) {
			auto& scriptElement = (*scriptArray)[i];

			auto gfxElement = ::Scaleform::GFx::Value();
			if (!a_value.GetElement(i, std::addressof(gfxElement))) [[unlikely]] {
				REX::Assert(false);
				return std::nullopt;
			}

			auto scriptValue = ToValue(gfxElement, scriptElement.GetTypeInfo());
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

	auto FromVar(RE::BSScript::SharedVariant a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		if (!a_value) {
			return ::Scaleform::GFx::Value(nullptr);
		}

		return FromValue(*a_value, a_movieRoot);
	}

	auto ToVar(const ::Scaleform::GFx::Value& a_value,
		const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::UniqueVariant>
	{
		if (a_value.IsNullOrUndefined()) {
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

	auto FromValue(const RE::BSScript::Variable& a_value,
		const ::Scaleform::Ptr<::Scaleform::GFx::ASMovieRootBase>& a_movieRoot) -> std::optional<::Scaleform::GFx::Value>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_value.GetRawType()) {
			case RawType::kNone: {
				return ::Scaleform::GFx::Value(a_value.GetNone());
			}
			case RawType::kBool: {
				return ::Scaleform::GFx::Value(a_value.GetBool());
			}
			case RawType::kInt: {
				return ::Scaleform::GFx::Value(a_value.GetInt());
			}
			case RawType::kFloat: {
				return ::Scaleform::GFx::Value(a_value.GetFloat());
			}
			case RawType::kString: {
				return ::Scaleform::GFx::Value(static_cast<std::string_view>(a_value.GetString()));
			}
			case RawType::kObject: {
				return FromObject(a_value.GetObject(), a_movieRoot);
			}
			case RawType::kStruct: {
				return FromStruct(a_value.GetStruct(), a_movieRoot);
			}
			case RawType::kVar: {
				return FromVar(a_value.GetVar(), a_movieRoot);
			}
			case RawType::kArrayBool:
			case RawType::kArrayInt:
			case RawType::kArrayFloat:
			case RawType::kArrayString:
			case RawType::kArrayObject:
			case RawType::kArrayStruct:
			case RawType::kArrayVar: {
				return FromArray(a_value.GetArray(), a_movieRoot);
			}
			default: {
				std::unreachable();
			}
		}
	}

	auto ToValue(const ::Scaleform::GFx::Value& a_value,
		const RE::BSScript::TypeInfo& a_typeInfo) -> std::optional<RE::BSScript::Variable>
	{
		using RawType = RE::BSScript::TypeInfo::RawType;

		switch (a_typeInfo.GetRawType()) {
			case RawType::kNone: {
				auto null = ToNull(a_value);
				if (!null.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(null.value());
			}
			case RawType::kBool: {
				if (!a_value.IsBoolean()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(a_value.GetBoolean());
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
			case RawType::kObject: {
				auto object = ToObject(a_value);
				if (!object.has_value()) {
					return std::nullopt;
				}
			}
			case RawType::kStruct: {
				auto structure = ToStruct(a_value, a_typeInfo);
				if (!structure.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(structure.value());
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

				return RE::BSScript::Variable(array.value());
			}
			default: {
				std::unreachable();
			}
		}
	}

	auto ToDetectedValue(const ::Scaleform::GFx::Value& a_value) -> std::optional<RE::BSScript::Variable>
	{
		using ValueType = ::Scaleform::GFx::Value::ValueType;

		switch (a_value.GetType()) {
			case ValueType::kNull:
			case ValueType::kUndefined: {
				return RE::BSScript::Variable(nullptr);
			}
			case ValueType::kBoolean: {
				return RE::BSScript::Variable(a_value.GetBoolean());
			}
			case ValueType::kInt: {
				return RE::BSScript::Variable(a_value.GetInt());
			}
			case ValueType::kUInt: {
				return RE::BSScript::Variable(a_value.GetUInt());
			}
			case ValueType::kNumber: {
				return RE::BSScript::Variable(static_cast<REX::Float32>(a_value.GetNumber()));
			}
			case ValueType::kString:
			case ValueType::kStringW: {
				auto stringValue = ToString(a_value);
				if (!stringValue.has_value()) {
					return std::nullopt;
				}

				auto object = Impl::ToObject(stringValue.value());
				if (object.has_value()) {
					return RE::BSScript::Variable(std::move(object).value());
				}

				return RE::BSScript::Variable(RE::BSFixedString(std::move(stringValue).value()));
			}
			case ValueType::kArray: {
				auto array = ToArray(a_value, RE::BSScript::TypeInfo::RawType::kVar);
				if (!array.has_value()) {
					return std::nullopt;
				}

				return RE::BSScript::Variable(std::move(array).value());
			}
			case ValueType::kObject:
			case ValueType::kDisplayObject:
			case ValueType::kClosure: {
				return RE::BSScript::Variable(nullptr);
			}
			default: {
				std::unreachable();
			}
		}
	}
}
