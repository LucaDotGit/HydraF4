#include "Plugin/Internal/Functors/TimerDelayFunctorManager.hpp"

namespace Plugin::Internal::Functors
{
	void TimerDelayFunctorManager::OnGameDataReady()
	{
		_functionRefMap.RegisterEvents();
	}

	void TimerDelayFunctorManager::OnSaveRevert()
	{
		Clear();
	}

	bool TimerDelayFunctorManager::IsExistent(const Script::ScriptFunctionRef& a_functionRef) const
	{
		return _functionRefMap.Contains(a_functionRef);
	}

	bool TimerDelayFunctorManager::IsPersistent(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		return sharedDelayFunctor->IsPersistent();
	}

	bool TimerDelayFunctorManager::IsRepeating(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		return sharedDelayFunctor->IsRepeating();
	}

	bool TimerDelayFunctorManager::IsActive(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		return sharedDelayFunctor->IsActive();
	}

	bool TimerDelayFunctorManager::IsPaused(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		return sharedDelayFunctor->IsPaused();
	}

	bool TimerDelayFunctorManager::IsRunningInRealTime(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		return sharedDelayFunctor->IsRunningInRealTime();
	}

	bool TimerDelayFunctorManager::IsRunningInGameMode(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		return sharedDelayFunctor->IsRunningInGameMode();
	}

	bool TimerDelayFunctorManager::IsRunningInMenuMode(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		return sharedDelayFunctor->IsRunningInMenuMode();
	}

	bool TimerDelayFunctorManager::IsRunningInGameTime(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		return sharedDelayFunctor->IsRunningInGameTime();
	}

	TimerDelayFunctorManager::Seconds TimerDelayFunctorManager::GetIntervalSeconds(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return Seconds::zero();
		}

		return sharedDelayFunctor->GetIntervalSeconds();
	}

	TimerDelayFunctorManager::Seconds TimerDelayFunctorManager::GetElapsedSeconds(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return Seconds::zero();
		}

		return sharedDelayFunctor->GetElapsedSeconds();
	}

	TimerDelayFunctorManager::Seconds TimerDelayFunctorManager::GetRemainingSeconds(const Script::ScriptFunctionRef& a_functionRef) const
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return Seconds::zero();
		}

		return sharedDelayFunctor->GetRemainingSeconds();
	}

	bool TimerDelayFunctorManager::Pause(const Script::ScriptFunctionRef& a_functionRef)
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		sharedDelayFunctor->Pause();
		return true;
	}

	bool TimerDelayFunctorManager::Resume(const Script::ScriptFunctionRef& a_functionRef)
	{
		const auto delayFunctor = _functionRefMap.GetValueOrDefault(a_functionRef);

		const auto sharedDelayFunctor = delayFunctor.lock();
		if (!sharedDelayFunctor) {
			return false;
		}

		sharedDelayFunctor->Resume();
		return true;
	}

	bool TimerDelayFunctorManager::Stop(const Script::ScriptFunctionRef& a_functionRef)
	{
		return _functionRefMap.Remove(a_functionRef,
			[]([[maybe_unused]] const Script::ScriptFunctionRef& a_functionRef, const value_type& a_delayFunctor) {
				const auto sharedDelayFunctor = a_delayFunctor.lock();
				if (!sharedDelayFunctor) {
					return true;
				}

				sharedDelayFunctor->Stop();
				return true;
			});
	}

	bool TimerDelayFunctorManager::StopAllGlobal(const RE::BSFixedString& a_namespace)
	{
		return _functionRefMap.RemoveAllInScript(a_namespace,
			[]([[maybe_unused]] const Script::ScriptFunctionRef& a_functionRef, const value_type& a_delayFunctor) {
				const auto sharedDelayFunctor = a_delayFunctor.lock();
				if (!sharedDelayFunctor) {
					return true;
				}

				sharedDelayFunctor->Stop();
				return true;
			});
	}

	bool TimerDelayFunctorManager::StopAllLocal(RE::BSScript::ObjectHandle a_objectHandle, const RE::BSFixedString& a_namespace)
	{
		return _functionRefMap.RemoveAllInScript(a_objectHandle, a_namespace,
			[]([[maybe_unused]] const Script::ScriptFunctionRef& a_functionRef, const value_type& a_delayFunctor) {
				const auto sharedDelayFunctor = a_delayFunctor.lock();
				if (!sharedDelayFunctor) {
					return true;
				}

				sharedDelayFunctor->Stop();
				return true;
			});
	}

	bool TimerDelayFunctorManager::StopAllLocal(const RE::BSTSmartPointer<RE::BSScript::Object>& a_object)
	{
		if (!a_object) {
			return false;
		}

		const auto objectHandle = a_object->GetHandle();
		if (objectHandle == RE::GameScript::HandlePolicy::EMPTY_HANDLE) {
			return false;
		}

		const auto scriptName = a_object->GetTypeInfoName();
		if (!scriptName.has_value()) {
			return false;
		}

		return StopAllLocal(objectHandle, scriptName.value());
	}

	bool TimerDelayFunctorManager::StartRealTime(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent)
	{
		auto timerFlags = REX::EnumSet(TimerDelayFunctor::TimerFlags::None);
		timerFlags.set(a_persistent, TimerDelayFunctor::TimerFlags::IsPersistent);

		return StartImpl(a_functionRef, a_delaySeconds, a_args, timerFlags);
	}

	bool TimerDelayFunctorManager::StartGameMode(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent)
	{
		auto timerFlags = REX::EnumSet(TimerDelayFunctor::TimerFlags::RunInGameMode);
		timerFlags.set(a_persistent, TimerDelayFunctor::TimerFlags::IsPersistent);

		return StartImpl(a_functionRef, a_delaySeconds, a_args, timerFlags);
	}

	bool TimerDelayFunctorManager::StartMenuMode(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent)
	{
		auto timerFlags = REX::EnumSet(TimerDelayFunctor::TimerFlags::RunInMenuMode);
		timerFlags.set(a_persistent, TimerDelayFunctor::TimerFlags::IsPersistent);

		return StartImpl(a_functionRef, a_delaySeconds, a_args, timerFlags);
	}

	bool TimerDelayFunctorManager::StartGameTime(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent)
	{
		auto timerFlags = REX::EnumSet(TimerDelayFunctor::TimerFlags::RunInGameTime);
		timerFlags.set(a_persistent, TimerDelayFunctor::TimerFlags::IsPersistent);

		return StartImpl(a_functionRef, a_delaySeconds, a_args, timerFlags);
	}

	bool TimerDelayFunctorManager::StartRepeatingRealTime(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent)
	{
		auto timerFlags = REX::EnumSet(TimerDelayFunctor::TimerFlags::IsRepeating);
		timerFlags.set(a_persistent, TimerDelayFunctor::TimerFlags::IsPersistent);

		return StartImpl(a_functionRef, a_delaySeconds, a_args, timerFlags);
	}

	bool TimerDelayFunctorManager::StartRepeatingGameMode(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent)
	{
		auto timerFlags = REX::EnumSet(TimerDelayFunctor::TimerFlags::IsRepeating, TimerDelayFunctor::TimerFlags::RunInGameMode);
		timerFlags.set(a_persistent, TimerDelayFunctor::TimerFlags::IsPersistent);

		return StartImpl(a_functionRef, a_delaySeconds, a_args, timerFlags);
	}

	bool TimerDelayFunctorManager::StartRepeatingMenuMode(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent)
	{
		auto timerFlags = REX::EnumSet(TimerDelayFunctor::TimerFlags::IsRepeating, TimerDelayFunctor::TimerFlags::RunInMenuMode);
		timerFlags.set(a_persistent, TimerDelayFunctor::TimerFlags::IsPersistent);

		return StartImpl(a_functionRef, a_delaySeconds, a_args, timerFlags);
	}

	bool TimerDelayFunctorManager::StartRepeatingGameTime(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent)
	{
		auto timerFlags = REX::EnumSet(TimerDelayFunctor::TimerFlags::IsRepeating, TimerDelayFunctor::TimerFlags::RunInGameTime);
		timerFlags.set(a_persistent, TimerDelayFunctor::TimerFlags::IsPersistent);

		return StartImpl(a_functionRef, a_delaySeconds, a_args, timerFlags);
	}

	bool TimerDelayFunctorManager::Contains(const Script::ScriptFunctionRef& a_functionRef) const
	{
		return IsExistent(a_functionRef);
	}

	bool TimerDelayFunctorManager::Add(const Script::ScriptFunctionRef& a_functionRef, const std::shared_ptr<TimerDelayFunctor>& a_delayFunctor)
	{
		if (!a_delayFunctor) {
			return false;
		}

		return _functionRefMap.Add(a_functionRef, a_delayFunctor);
	}

	bool TimerDelayFunctorManager::Remove(const Script::ScriptFunctionRef& a_functionRef)
	{
		return Stop(a_functionRef);
	}

	void TimerDelayFunctorManager::Clear() noexcept
	{
		_functionRefMap.Clear();
	}

	bool TimerDelayFunctorManager::CreateImpl(const Script::ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<auto(const Script::ScriptFunctionRef&)->REX::NotNull<std::shared_ptr<TimerDelayFunctor>>>>& a_functorFactory)
	{
		const auto hasInserted = _functionRefMap.AddOrUpdate(a_functionRef,
			[&a_functorFactory]([[maybe_unused]] const Script::ScriptFunctionRef& a_functionRef, const value_type* a_existingDelayFunctor) {
				if (a_existingDelayFunctor) {
					const auto sharedExistingFunctor = a_existingDelayFunctor->lock();
					if (sharedExistingFunctor) {
						sharedExistingFunctor->Stop();
					}
				}

				const auto delayFunctor = std::invoke(*a_functorFactory, a_functionRef);
				F4SE::GetDelayFunctorManager()->Enqueue(delayFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
				return delayFunctor.get();
			});

		return hasInserted;
	}

	bool TimerDelayFunctorManager::StartImpl(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args,
		REX::EnumSet<TimerDelayFunctor::TimerFlags> a_flags)
	{
		if (!a_functionRef.ValidateRefMembers()) {
			return false;
		}

		if (a_delaySeconds <= Seconds::zero()) {
			return a_functionRef.InvokeAppliedFunction(a_args);
		}

		return CreateImpl(a_functionRef, [a_delaySeconds, &a_args, a_flags](const Script::ScriptFunctionRef& a_functionRef) {
			return TimerDelayFunctorFactory::GetSingleton()->CreateObject(a_functionRef, a_delaySeconds, a_args, a_flags.get());
		});
	}

	auto GetTimerDelayFunctorManager() -> const REX::NotNull<std::unique_ptr<TimerDelayFunctorManager>>&
	{
		static const auto INSTANCE = REX::NotNull(std::make_unique<TimerDelayFunctorManager>());
		return INSTANCE;
	}
}
