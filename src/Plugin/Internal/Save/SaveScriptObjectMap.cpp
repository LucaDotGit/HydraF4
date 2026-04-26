#include "Plugin/Internal/Save/SaveScriptObjectMap.hpp"

#include "Plugin/Internal/Core/JsonUtility.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	SaveScriptObjectMap::SaveScriptObjectMap(std::uint32_t a_type, std::uint32_t a_version)
		: ISaveObject(a_type, a_version)
	{
	}

	SaveScriptObjectMap::~SaveScriptObjectMap() noexcept = default;

	bool SaveScriptObjectMap::Load(const F4SE::SerializationInterface& a_serializer, const std::shared_ptr<SaveStringPool>& a_stringPool)
	{
		auto stream = SaveStream(std::addressof(a_serializer), a_stringPool);
		return Load(stream);
	}

	bool SaveScriptObjectMap::Save(const F4SE::SerializationInterface& a_serializer, const SaveStream& a_stream) const
	{
		return ISaveObject::Save(a_serializer, a_stream);
	}

	bool SaveScriptObjectMap::Load([[maybe_unused]] const SaveStream& a_stream)
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto mapLock = std::shared_lock(_mapMutex);

		for (const auto& [scriptName, objectInfo] : _scriptObjectMap) {
			auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
			if (!scriptVM->GetScriptObjectType(scriptName, objectTypeInfo) || !objectTypeInfo || !objectTypeInfo->IsLinked()) {
				continue;
			}

			const auto propertiesFactory = REX::NotNull(std::function([objectTypeInfo, objectInfo]() {
				return CreateInitProperties(objectTypeInfo, objectInfo->properties);
			}));

			auto scriptObject = RE::BSTSmartPointer<RE::BSScript::Object>();
			if (!scriptVM->GetOrCreateScriptObjectExact(scriptName, objectInfo->objectHandle, propertiesFactory, scriptObject) || !scriptObject) {
				continue;
			}

			if (!AutoFillProperties(objectTypeInfo, propertiesFactory, scriptObject)) {
				continue;
			}
		}

		return true;
	}

	bool SaveScriptObjectMap::Save(SaveStream& a_stream) const
	{
		REX::Assert(a_stream.GetMemoryStream().empty());
		return true;
	}

	void SaveScriptObjectMap::Revert() noexcept
	{
		return;
	}

	bool SaveScriptObjectMap::IsEmpty() const noexcept
	{
		const auto mapLock = std::shared_lock(_mapMutex);
		return _scriptObjectMap.empty();
	}

	std::uint32_t SaveScriptObjectMap::GetSize() const noexcept
	{
		const auto mapLock = std::shared_lock(_mapMutex);
		return static_cast<std::uint32_t>(_scriptObjectMap.size());
	}

	void SaveScriptObjectMap::AddValue(REX::NotNull<std::shared_ptr<ObjectInfo>> a_objectInfo)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto scriptName = a_objectInfo->scriptName;
		_scriptObjectMap.emplace(scriptName, std::move(a_objectInfo));
	}

	void SaveScriptObjectMap::Clear() noexcept
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		_scriptObjectMap.clear();
		_scriptObjectMap.rehash(0);
	}

	auto SaveScriptObjectMap::CreateInitProperties(
		const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>>& a_objectTypeInfo,
		const std::unordered_map<RE::BSFixedString, json_t>& a_templateProperties)
		-> RE::BSTScrapHashMap<RE::BSFixedString, RE::BSScript::Variable>
	{
		auto initProperties = RE::BSTScrapHashMap<RE::BSFixedString, RE::BSScript::Variable>();
		initProperties.reserve(static_cast<std::uint32_t>(a_templateProperties.size()));

		for (const auto& [propName, propValue] : a_templateProperties) {
			const auto* propInfo = a_objectTypeInfo->GetPropertyInfoByName(propName);
			if (!propInfo) {
				continue;
			}

			const auto& propTypeInfo = propInfo->info.type;

			auto scriptValue = Core::JsonUtility::ToValue(propValue, propTypeInfo);
			if (!scriptValue.has_value()) {
				continue;
			}

			initProperties.emplace(propName, std::move(scriptValue).value());
		}

		return initProperties;
	}

	bool SaveScriptObjectMap::AutoFillProperties(
		const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>>& a_objectTypeInfo,
		const REX::NotNull<std::function<RE::BSTScrapHashMap<RE::BSFixedString, RE::BSScript::Variable>()>>& a_propertiesFactory,
		const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::Object>>& a_scriptObject)
	{
		if (!a_objectTypeInfo->IsLinked()) {
			return false;
		}

		if (!a_scriptObject->IsInitialized()) {
			return false;
		}

		const auto scriptVariableInfos = a_objectTypeInfo->GetThisVariables();
		if (scriptVariableInfos.empty()) {
			return true;
		}

		const auto scriptPropertyInfos = a_objectTypeInfo->GetThisProperties();
		if (scriptPropertyInfos.empty()) {
			return true;
		}

		const auto initProperties = std::invoke(*a_propertiesFactory);
		const auto scriptObjectLock = RE::BSAutoLock(*(a_scriptObject.get()));

		for (const auto& propertyInfo : scriptPropertyInfos) {
			const auto initPropertyIt = initProperties.find(propertyInfo.name);
			if (initPropertyIt == initProperties.end()) {
				continue;
			}

			const auto autoVarIndex = propertyInfo.info.autoVarIndex;
			if (autoVarIndex == std::numeric_limits<decltype(autoVarIndex)>::max()) {
				continue;
			}

			if (autoVarIndex >= scriptVariableInfos.size()) [[unlikely]] {
				REX::Assert(false);
				continue;
			}

			// In the engine, only the backing variable of const auto-properties are auto-filled,
			// so the same behavior is replicated here.

			const auto& variableInfo = scriptVariableInfos[autoVarIndex];
			if (!variableInfo.isConst) {
				continue;
			}

			a_scriptObject->variables[autoVarIndex] = initPropertyIt->second;
		}

		return true;
	}
}
