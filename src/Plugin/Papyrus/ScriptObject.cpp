#include "Plugin/Papyrus/ScriptObject.hpp"

#include "Plugin/Internal/Functors/InvokeGetterPropertyDelayFunctor.hpp"
#include "Plugin/Internal/Functors/InvokeSetterPropertyDelayFunctor.hpp"
#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Int64.hpp"

namespace Plugin::Papyrus::ScriptObject
{
	using LongStruct = ::Plugin::Internal::Structs::Int64::LongData;

	static bool IsRegisteredForRemoteEvent(RE::BSScript::Internal::VirtualMachine& a_internalVM, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSTSmartPointer<RE::BSScript::Object> a_sender,
		RE::BSFixedString a_eventName)
	{
		if (!a_object || !a_sender) {
			a_internalVM.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto eventLock = RE::BSAutoLock(a_internalVM.eventRelayLock);

		const auto scriptIt = a_internalVM.eventRelays.find(a_sender->GetHandle());
		if (scriptIt == a_internalVM.eventRelays.end()) {
			return false;
		}

		const auto& eventRelay = scriptIt->second;
		const auto eventIt = eventRelay->events.find(a_eventName);

		if (eventIt == eventRelay->events.end()) {
			return false;
		}

		const auto& targetList = eventIt->second;
		return targetList->targets.contains(a_object);
	}

	static bool IsRegisteredForExternalEvent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_eventName,
		RE::BSFixedString a_functionName)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto objectTypeInfo = a_object->GetTypeInfo();
		if (!objectTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		auto externalEventInfo = F4SE::Events::ExternalEventInfo{
			.objectHandle = a_object->GetHandle(),
			.scriptName = objectTypeInfo->GetName(),
			.functionName = a_functionName
		};

		return F4SE::Events::IsRegisteredForExternalEvent(a_eventName, externalEventInfo);
	}

	static bool IsTimerActive(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		std::uint32_t a_timerId)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto* timers = RE::GameScript::TimerEventHandler::GetSingleton();
		if (!timers) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto timerLock = RE::BSAutoLock(timers->timerLock);

		return std::ranges::any_of(timers->timerQueue, [&a_object, a_timerId](const RE::BSTSmartPointer<RE::GameScript::Internal::ScriptTimer>& a_timer) {
			return a_timer && a_timer->object == a_object && a_timer->timerID == a_timerId;
		});
	}

	static bool IsTimerGameTimeActive(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		std::uint32_t a_timerId)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto* timers = RE::GameScript::TimerEventHandler::GetSingleton();
		if (!timers) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto timerLock = RE::BSAutoLock(timers->timerLock);

		return std::ranges::any_of(timers->timerGameTimeQueue, [&a_object, a_timerId](const RE::BSTSmartPointer<RE::GameScript::Internal::ScriptTimer>& a_timer) {
			return a_timer && a_timer->object == a_object && a_timer->timerID == a_timerId;
		});
	}

	static REX::Float32 GetTimerRemainingTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		std::uint32_t a_timerId)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return 0.0_f32;
		}

		const auto* timers = RE::GameScript::TimerEventHandler::GetSingleton();
		if (!timers) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		const auto timerLock = RE::BSAutoLock(timers->timerLock);

		for (const auto& timer : timers->timerQueue) {
			if (!timer || timer->object != a_object || timer->timerID != a_timerId) {
				continue;
			}

			const auto* gameVM = RE::GameVM::GetSingleton();
			if (!gameVM) [[unlikely]] {
				REX::Assert(false);
				return 0.0_f32;
			}

			const auto timeLock = RE::BSAutoLock(gameVM->GetRuntimeData().vmTimeLock);
			return timer->GetRemainingSeconds(gameVM->GetRuntimeData().currentTime);
		}

		return 0.0_f32;
	}

	static REX::Float32 GetTimerGameTimeRemainingTime(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		std::uint32_t a_timerId)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return 0.0_f32;
		}

		const auto* timers = RE::GameScript::TimerEventHandler::GetSingleton();
		if (!timers) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		const auto timerLock = RE::BSAutoLock(timers->timerLock);

		for (const auto& timer : timers->timerGameTimeQueue) {
			if (!timer || timer->object != a_object || timer->timerID != a_timerId) {
				continue;
			}

			const auto* gameVM = RE::GameVM::GetSingleton();
			if (!gameVM) [[unlikely]] {
				REX::Assert(false);
				return 0.0_f32;
			}

			const auto timeLock = RE::BSAutoLock(gameVM->GetRuntimeData().vmTimeLock);
			return timer->GetRemainingGameTimeSeconds(gameVM->GetRuntimeData().currentGameTime);
		}

		return 0.0_f32;
	}

	static std::optional<std::vector<std::uint32_t>> GetActiveTimerIds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* timers = RE::GameScript::TimerEventHandler::GetSingleton();
		if (!timers) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto timerLock = RE::BSAutoLock(timers->timerLock);

		auto activeTimerIds = std::vector<std::uint32_t>();

		for (const auto& timer : timers->timerQueue) {
			if (timer && timer->object == a_object) {
				activeTimerIds.push_back(timer->timerID);
			}
		}

		return activeTimerIds;
	}

	static std::optional<std::vector<std::uint32_t>> GetActiveTimerGameTimeIds(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return std::nullopt;
		}

		const auto* timers = RE::GameScript::TimerEventHandler::GetSingleton();
		if (!timers) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto timerLock = RE::BSAutoLock(timers->timerLock);

		auto activeTimerIds = std::vector<std::uint32_t>();

		for (const auto& timer : timers->timerGameTimeQueue) {
			if (timer && timer->object == a_object) {
				activeTimerIds.push_back(timer->timerID);
			}
		}

		return activeTimerIds;
	}

	static void CancelAllTimers(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return;
		}

		auto* timers = RE::GameScript::TimerEventHandler::GetSingleton();
		if (!timers) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		const auto timerLock = RE::BSAutoLock(timers->timerLock);

		auto& timerQueue = timers->timerQueue;

		const auto removeIt = std::ranges::remove_if(timerQueue, [&a_object](const RE::BSTSmartPointer<RE::GameScript::Internal::ScriptTimer>& a_timer) {
			return a_timer && a_timer->object == a_object;
		});

		timerQueue.erase(removeIt.begin(), removeIt.end());
	}

	static void CancelAllGameTimeTimers(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return;
		}

		auto* timers = RE::GameScript::TimerEventHandler::GetSingleton();
		if (!timers) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		const auto timerLock = RE::BSAutoLock(timers->timerLock);

		auto& timerQueue = timers->timerGameTimeQueue;

		const auto removeIt = std::ranges::remove_if(timerQueue, [&a_object](const RE::BSTSmartPointer<RE::GameScript::Internal::ScriptTimer>& a_timer) {
			return a_timer && a_timer->object == a_object;
		});

		timerQueue.erase(removeIt.begin(), removeIt.end());
	}

	static bool IsInstanceOf(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_scriptName)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto objectTypeInfo = a_object->GetTypeInfo();
		if (!objectTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		return a_vm.IsInstanceOf(objectTypeInfo, a_scriptName);
	}

	static RE::TESForm* GetFormById(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFormID a_formId)
	{
		return RE::TESForm::FindFormByNumericID(a_formId);
	}

	static RE::TESForm* GetFormByEditorId(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSFixedString a_editorId)
	{
		return RE::TESForm::FindFormByEditorID(a_editorId);
	}

	static RE::TESForm* GetFormFromPlugin(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESFormID a_localFormId,
		std::string_view a_pluginName)
	{
		const auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return dataHandler->FindForm(a_localFormId, a_pluginName);
	}

	static RE::BGSBaseAlias* GetAliasById(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint16_t a_aliasId,
		RE::TESQuest* a_quest)
	{
		if (!a_quest) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::QUEST_NULL, a_stackId);
			return nullptr;
		}

		return a_quest->GetAliasByID(a_aliasId);
	}

	static RE::NiPointer<RE::TESObjectREFR> GetInventoryRefById(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint16_t a_itemId,
		RE::NiPointer<RE::TESObjectREFR> a_containerRef)
	{
		if (!a_containerRef) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::OBJECT_REFERENCE_NULL, a_stackId);
			return nullptr;
		}

		return RE::GameScript::GetRefInventoryObjectRef(a_containerRef, a_itemId);
	}

	static RE::BSTSmartPointer<RE::ActiveEffect> GetActiveEffectById(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint16_t a_activeEffectId,
		RE::NiPointer<RE::Actor> a_actor)
	{
		if (!a_actor) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::ACTOR_NULL, a_stackId);
			return nullptr;
		}

		const auto* activeEffectList = a_actor->GetActiveEffectList();
		if (!activeEffectList) {
			return nullptr;
		}

		return activeEffectList->GetActiveEffectByID(a_activeEffectId);
	}

	static RE::BSTSmartPointer<RE::BSInputEnableLayer> GetInputLayerById(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_inputLayerId)
	{
		const auto* inputEnableManager = RE::BSInputEnableManager::GetSingleton();
		if (!inputEnableManager) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		return inputEnableManager->GetLayerByID(a_inputLayerId);
	}

	static LongStruct GetHandle(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return LongStruct::FromUInt64(RE::GameScript::HandlePolicy::EMPTY_HANDLE);
		}

		return LongStruct::FromUInt64(a_object->GetHandle());
	}

	static RE::BSFixedString GetScriptName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return {};
		}

		const auto objectTypeInfo = a_object->GetTypeInfo();
		if (!objectTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return {};
		}

		return objectTypeInfo->GetName();
	}

	static RE::BSFixedString GetParentScriptName(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return {};
		}

		const auto objectTypeInfo = a_object->GetTypeInfo();
		if (!objectTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return {};
		}

		const auto parentTypeInfo = objectTypeInfo->parentTypeInfo;
		if (!parentTypeInfo) {
			return {};
		}

		return parentTypeInfo->GetName();
	}

	static bool ContainsVariable(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto objectLock = RE::BSAutoLock(*a_object);

		const auto* variable = a_object->GetVariable(a_name);
		return variable != nullptr;
	}

	static RE::BSScript::UniqueVariant GetVariableValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return nullptr;
		}

		const auto objectLock = RE::BSAutoLock(*a_object);

		const auto* variable = a_object->GetVariable(a_name);
		if (!variable) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::VARIABLE_NOT_FOUND, a_stackId, a_name);
			return nullptr;
		}

		return RE::BSScript::CreateVariant(variable);
	}

	static bool SetVariableValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto objectLock = RE::BSAutoLock(*a_object);

		auto* variable = a_object->GetVariable(a_name);
		if (!variable) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::VARIABLE_NOT_FOUND, a_stackId, a_name);
			return false;
		}

		return variable->TryAssign(RE::BSScript::UnwrapVariant(a_value));
	}

	static bool ContainsAutoProperty(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto objectLock = RE::BSAutoLock(*a_object);

		const auto* property = a_object->GetProperty(a_name);
		return property != nullptr;
	}

	static RE::BSScript::UniqueVariant GetAutoPropertyValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return nullptr;
		}

		const auto objectLock = RE::BSAutoLock(*a_object);

		const auto* property = a_object->GetProperty(a_name);
		if (!property) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROPERTY_NOT_FOUND, a_stackId, a_name);
			return nullptr;
		}

		return RE::BSScript::CreateVariant(property);
	}

	static bool SetAutoPropertyValue(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto objectLock = RE::BSAutoLock(*a_object);

		auto* property = a_object->GetProperty(a_name);
		if (!property) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROPERTY_NOT_FOUND, a_stackId, a_name);
			return false;
		}

		return property->TryAssign(RE::BSScript::UnwrapVariant(a_value));
	}

	static bool ContainsGetterProperty(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto objectTypeInfo = a_object->GetTypeInfo();
		if (!objectTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto* propertyInfo = objectTypeInfo->GetPropertyInfoByName(a_name);
		if (!propertyInfo) {
			return false;
		}

		return propertyInfo->info.getFunction != nullptr;
	}

	static bool ContainsSetterProperty(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto objectTypeInfo = a_object->GetTypeInfo();
		if (!objectTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		const auto* propertyInfo = objectTypeInfo->GetPropertyInfoByName(a_name);
		if (!propertyInfo) {
			return false;
		}

		return propertyInfo->info.setFunction != nullptr;
	}

	static RE::BSScript::LatentResult<RE::BSScript::UniqueVariant> InvokeGetterProperty(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto objectTypeInfo = a_object->GetTypeInfo();
		if (!objectTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto* propertyInfo = objectTypeInfo->GetPropertyInfoByName(a_name);
		if (!propertyInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROPERTY_NOT_FOUND, a_stackId, a_name);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto getterFunction = propertyInfo->info.getFunction;
		if (!getterFunction || !getterFunction->HasNoParams()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROPERTY_NOT_FOUND, a_stackId, a_name);
			return RE::BSScript::LatentStatus::kFailed;
		}

		auto invokeFunctor = ::Plugin::Internal::Functors::InvokeGetterPropertyDelayFunctorFactory::GetSingleton()->CreateObject(a_object, a_name, a_stackId);
		F4SE::GetDelayFunctorManager()->Enqueue(invokeFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);

		return RE::BSScript::LatentStatus::kStarted;
	}

	static RE::BSScript::LatentResult<bool> InvokeSetterProperty(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto objectTypeInfo = a_object->GetTypeInfo();
		if (!objectTypeInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto* propertyInfo = objectTypeInfo->GetPropertyInfoByName(a_name);
		if (!propertyInfo) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROPERTY_NOT_FOUND, a_stackId, a_name);
			return RE::BSScript::LatentStatus::kFailed;
		}

		const auto setterFunction = propertyInfo->info.setFunction;
		if (!setterFunction || !setterFunction->HasParamsOfVariadicTypes(std::array{ a_value })) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::PROPERTY_NOT_FOUND, a_stackId, a_name);
			return RE::BSScript::LatentStatus::kFailed;
		}

		auto invokeFunctor = ::Plugin::Internal::Functors::InvokeSetterPropertyDelayFunctorFactory::GetSingleton()->CreateObject(a_object, a_name, RE::BSScript::UnwrapVariant(a_value), a_stackId);
		F4SE::GetDelayFunctorManager()->Enqueue(invokeFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);

		return RE::BSScript::LatentStatus::kStarted;
	}

	static bool InvokeSetterPropertyAsync(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSTSmartPointer<RE::BSScript::Object> a_object,
		RE::BSFixedString a_name,
		RE::BSScript::SharedVariant a_value)
	{
		if (!a_object) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::SCRIPT_OBJECT_NULL, a_stackId);
			return false;
		}

		constexpr auto STACK_CALLBACK = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
		return a_vm.SetPropertyValue(a_object, a_name.data(), RE::BSScript::UnwrapVariant(a_value), STACK_CALLBACK);
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsInstanceOf);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForRemoteEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRegisteredForExternalEvent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsTimerActive);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsTimerGameTimeActive);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTimerRemainingTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTimerGameTimeRemainingTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetActiveTimerIds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetActiveTimerGameTimeIds);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CancelAllTimers);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, CancelAllGameTimeTimers);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFormById);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFormByEditorId);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFormFromPlugin);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAliasById);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInventoryRefById);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetActiveEffectById);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetInputLayerById);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetHandle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetScriptName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetParentScriptName);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsVariable);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetVariableValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetVariableValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsAutoProperty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAutoPropertyValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAutoPropertyValue);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsGetterProperty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, ContainsSetterProperty);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, InvokeGetterProperty);
		RE_REGISTER_VM_FUNCTION_LATENT_ASYNC(a_vm, SCRIPT_NAME, InvokeSetterProperty);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, InvokeSetterPropertyAsync);
	}
}
