#include "Plugin/Internal/Save/SaveScriptFunctionMap.hpp"

#include "Plugin/Internal/Core/JsonUtility.hpp"
#include "Plugin/Internal/Events/EventFunctorMap.hpp"
#include "Plugin/Internal/Events/EventSystem.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Save
{
	SaveScriptFunctionMap::SaveScriptFunctionMap(std::uint32_t a_type, std::uint32_t a_version,
		const REX::NotNull<std::shared_ptr<Events::EventSystem>>& a_eventSystem)
		: ISaveObject(a_type, a_version),
		  _eventSystem(a_eventSystem)
	{
	}

	SaveScriptFunctionMap::~SaveScriptFunctionMap() noexcept = default;

	bool SaveScriptFunctionMap::Load(const F4SE::SerializationInterface& a_serializer, const std::shared_ptr<SaveStringPool>& a_stringPool)
	{
		auto stream = SaveStream(std::addressof(a_serializer), a_stringPool);
		return Load(stream);
	}

	bool SaveScriptFunctionMap::Save(const F4SE::SerializationInterface& a_serializer, const SaveStream& a_stream) const
	{
		return ISaveObject::Save(a_serializer, a_stream);
	}

	bool SaveScriptFunctionMap::Load([[maybe_unused]] const SaveStream& a_stream)
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto mapLock = std::shared_lock(_mapMutex);

		for (const auto& [eventName, eventInfo] : _eventMap) {
			const auto eventFunctor = Events::GetEventFunctorMap()->GetValue(eventName);
			if (!eventFunctor) {
				continue;
			}

			const auto& eventFunctionRef = eventInfo->functionRef;
			if (_eventSystem->IsRegistered(eventFunctionRef, eventName)) {
				continue;
			}

			const auto eventStructName = RE::BSFixedString(std::invoke(*eventFunctor->GetEventArgsNameCallback()));
			const auto eventStructArgs = CreateEventArgsStruct(scriptVM, eventStructName, eventInfo->eventArgs);

			if (!eventStructArgs.has_value()) {
				continue;
			}

			constexpr auto IS_EVENT_HANDLER_PERSISTENT = false;
			const auto eventHandler = std::invoke(*eventFunctor->GetCreateCallback(), eventFunctionRef, eventStructArgs.value(), IS_EVENT_HANDLER_PERSISTENT);

			if (!_eventSystem->Register(eventFunctionRef, eventHandler)) {
				continue;
			}
		}

		return true;
	}

	bool SaveScriptFunctionMap::Save(SaveStream& a_stream) const
	{
		REX::Assert(a_stream.GetMemoryStream().empty());
		return true;
	}

	void SaveScriptFunctionMap::Revert() noexcept
	{
		const auto mapLock = std::scoped_lock(_mapMutex);
		_eventSystem->Revert();
	}

	bool SaveScriptFunctionMap::IsEmpty() const noexcept
	{
		const auto mapLock = std::shared_lock(_mapMutex);
		return _eventMap.empty();
	}

	std::uint32_t SaveScriptFunctionMap::GetSize() const noexcept
	{
		const auto mapLock = std::shared_lock(_mapMutex);
		return static_cast<std::uint32_t>(_eventMap.size());
	}

	void SaveScriptFunctionMap::AddValue(REX::NotNull<std::shared_ptr<EventInfo>> a_eventInfo)
	{
		const auto mapLock = std::scoped_lock(_mapMutex);
		const auto eventName = a_eventInfo->eventName;
		_eventMap.emplace(eventName, std::move(a_eventInfo));
	}

	void SaveScriptFunctionMap::Clear() noexcept
	{
		const auto mapLock = std::scoped_lock(_mapMutex);

		_eventMap.clear();
		_eventMap.rehash(0);
	}

	auto SaveScriptFunctionMap::CreateEventArgsStruct(
		const RE::BSTSmartPointer<RE::BSScript::IVirtualMachine>& a_scriptVM,
		const RE::BSFixedString& a_structName,
		const std::unordered_map<RE::BSFixedString, json_t>& a_eventArgs)
		-> std::optional<RE::BSTSmartPointer<RE::BSScript::Struct>>
	{
		auto argsTypeInfo = RE::BSTSmartPointer<RE::BSScript::StructTypeInfo>();
		if (!a_scriptVM->GetScriptStructType(a_structName, argsTypeInfo) || !argsTypeInfo) {
			return std::nullopt;
		}

		auto argsStruct = RE::BSTSmartPointer<RE::BSScript::Struct>();
		if (!a_scriptVM->CreateStruct(argsTypeInfo->GetName(), argsStruct) || !argsStruct) {
			return std::nullopt;
		}

		for (const auto& [argName, argValue] : a_eventArgs) {
			const auto varIndex = argsTypeInfo->GetVariableIndex(argName);
			if (!varIndex.has_value()) {
				continue;
			}

			const auto& varTypeInfo = argsTypeInfo->variables[varIndex.value()].varType;

			auto scriptValue = Core::JsonUtility::ToValue(argValue, varTypeInfo);
			if (!scriptValue.has_value()) {
				continue;
			}

			argsStruct->variables[varIndex.value()] = std::move(scriptValue).value();
		}

		return argsStruct;
	}
}
