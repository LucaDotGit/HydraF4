#include "Plugin/Internal/Functors/MutexDelayFunctorManager.hpp"

namespace Plugin::Internal::Functors
{
	void MutexDelayFunctorManager::OnGameDataReady()
	{
		_functionRefMap.RegisterEvents();
	}

	void MutexDelayFunctorManager::OnSaveRevert()
	{
		Clear();
	}

	MutexDelayFunctorManager::LockResult MutexDelayFunctorManager::Lock(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId)
	{
		if (a_stackId == RE::BSScript::Internal::VirtualMachine::EMPTY_STACK_ID) {
			return LockResult::Failure;
		}

		const auto hasInserted = _functionRefMap.AddOrUpdate(a_functionRef,
			[a_stackId](const Script::ScriptFunctionRef& a_functionRef, value_type* a_delayFunctorQueue) {
				auto delayFunctor = CreateImpl(a_functionRef, a_stackId);

				if (a_delayFunctorQueue) {
					auto& updatedQueue = *a_delayFunctorQueue;
					updatedQueue.emplace_back(delayFunctor.get());
					return std::move(updatedQueue);
				}

				auto newQueue = value_type();
				newQueue.emplace_back(delayFunctor.get());
				return newQueue;
			});

		return hasInserted ? LockResult::IsOwner : LockResult::IsWaiter;
	}

	MutexDelayFunctorManager::LockResult MutexDelayFunctorManager::LockForSeconds(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId, Seconds a_timeoutSeconds)
	{
		if (a_stackId == RE::BSScript::Internal::VirtualMachine::EMPTY_STACK_ID) {
			return LockResult::Failure;
		}

		const auto hasInserted = _functionRefMap.AddOrUpdate(a_functionRef,
			[a_stackId, a_timeoutSeconds](const Script::ScriptFunctionRef& a_functionRef, value_type* a_delayFunctorQueue) {
				auto delayFunctor = CreateImpl(a_functionRef, a_stackId, a_timeoutSeconds);

				if (a_delayFunctorQueue) {
					auto& updatedQueue = *a_delayFunctorQueue;
					updatedQueue.emplace_back(delayFunctor.get());
					return std::move(updatedQueue);
				}

				auto newQueue = value_type();
				newQueue.emplace_back(delayFunctor.get());
				return newQueue;
			});

		return hasInserted ? LockResult::IsOwner : LockResult::IsWaiter;
	}

	bool MutexDelayFunctorManager::TryLock(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId)
	{
		if (a_stackId == RE::BSScript::Internal::VirtualMachine::EMPTY_STACK_ID) {
			return false;
		}

		return _functionRefMap.Add(a_functionRef,
			[a_stackId](const Script::ScriptFunctionRef& a_functionRef) {
				auto delayFunctor = CreateImpl(a_functionRef, a_stackId);

				auto newQueue = value_type();
				newQueue.emplace_back(delayFunctor.get());
				return newQueue;
			});
	}

	bool MutexDelayFunctorManager::Unlock(const Script::ScriptFunctionRef& a_functionRef)
	{
		return UnlockImpl(a_functionRef, nullptr);
	}

	bool MutexDelayFunctorManager::Contains(const Script::ScriptFunctionRef& a_functionRef) const
	{
		return _functionRefMap.Contains(a_functionRef);
	}

	bool MutexDelayFunctorManager::Add(const Script::ScriptFunctionRef& a_functionRef,
		const std::shared_ptr<MutexDelayFunctor>& a_delayFunctor)
	{
		if (!a_delayFunctor) {
			return false;
		}

		const auto hasInserted = _functionRefMap.AddOrUpdate(a_functionRef,
			[&a_delayFunctor]([[maybe_unused]] const Script::ScriptFunctionRef& a_functionRef, value_type* a_delayFunctorQueue) {
				if (a_delayFunctorQueue) {
					auto& updatedQueue = *a_delayFunctorQueue;
					updatedQueue.emplace_back(a_delayFunctor);
					return std::move(updatedQueue);
				}

				auto newQueue = value_type();
				newQueue.emplace_back(a_delayFunctor);
				return newQueue;
			});

		return hasInserted;
	}

	bool MutexDelayFunctorManager::Remove(const Script::ScriptFunctionRef& a_functionRef, const std::shared_ptr<MutexDelayFunctor>& a_delayFunctor)
	{
		if (!a_delayFunctor) {
			return false;
		}

		return UnlockImpl(a_functionRef, a_delayFunctor);
	}

	void MutexDelayFunctorManager::Clear() noexcept
	{
		_functionRefMap.Clear();
	}

	auto MutexDelayFunctorManager::CreateImpl(const Script::ScriptFunctionRef& a_functionRef,
		const REX::NotNull<std::function<auto(const Script::ScriptFunctionRef&)->REX::NotNull<std::shared_ptr<MutexDelayFunctor>>>>& a_functorFactory)
		-> REX::NotNull<std::shared_ptr<MutexDelayFunctor>>
	{
		auto mutexFunctor = std::invoke(*a_functorFactory, a_functionRef);
		mutexFunctor->Lock();

		F4SE::GetDelayFunctorManager()->Enqueue(mutexFunctor.get().get(), F4SE::DelayFunctorManager::RUN_DIRECTLY_DELAY);
		return mutexFunctor;
	}

	auto MutexDelayFunctorManager::CreateImpl(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId)
		-> REX::NotNull<std::shared_ptr<MutexDelayFunctor>>
	{
		return CreateImpl(a_functionRef, [a_stackId](const Script::ScriptFunctionRef& a_functionRef) {
			return MutexDelayFunctorFactory::GetSingleton()->CreateObject(a_functionRef, a_stackId);
		});
	}

	auto MutexDelayFunctorManager::CreateImpl(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId, Seconds a_timeoutSeconds)
		-> REX::NotNull<std::shared_ptr<MutexDelayFunctor>>
	{
		return CreateImpl(a_functionRef, [a_stackId, a_timeoutSeconds](const Script::ScriptFunctionRef& a_functionRef) {
			return MutexDelayFunctorFactory::GetSingleton()->CreateObject(a_functionRef, a_stackId, a_timeoutSeconds);
		});
	}

	bool MutexDelayFunctorManager::UnlockImpl(const Script::ScriptFunctionRef& a_functionRef, const std::shared_ptr<MutexDelayFunctor>& a_delayFunctor)
	{
		auto isUnlocked = false;

		_functionRefMap.Remove(a_functionRef,
			[&a_delayFunctor, &isUnlocked]([[maybe_unused]] const Script::ScriptFunctionRef& a_functionRef, value_type& a_delayFunctorQueue) {
				while (!a_delayFunctorQueue.empty()) {
					const auto sharedDelayFunctor = a_delayFunctorQueue.front().lock();
					if (!sharedDelayFunctor) {
						a_delayFunctorQueue.pop_front();
						continue;
					}

					if (a_delayFunctor && a_delayFunctor->GetStackId() != sharedDelayFunctor->GetStackId()) {
						break;
					}

					sharedDelayFunctor->Unlock();
					a_delayFunctorQueue.pop_front();

					isUnlocked = true;

					while (!a_delayFunctorQueue.empty()) {
						const auto nextSharedDelayFunctor = a_delayFunctorQueue.front().lock();
						if (!nextSharedDelayFunctor) {
							a_delayFunctorQueue.pop_front();
							continue;
						}

						nextSharedDelayFunctor->QueueForUnlock();
						break;
					}

					break;
				}

				return a_delayFunctorQueue.empty();
			});

		return isUnlocked;
	}

	auto GetMutexDelayFunctorManager() -> const REX::NotNull<std::unique_ptr<MutexDelayFunctorManager>>&
	{
		static const auto INSTANCE = REX::NotNull(std::make_unique<MutexDelayFunctorManager>());
		return INSTANCE;
	}
}
