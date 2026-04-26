#pragma once

#include "Plugin/Internal/Functors/MutexDelayFunctor.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRefMap.hpp"

namespace Plugin::Internal::Functors
{
	class MutexDelayFunctorManager final
	{
	public:
		using value_type = std::deque<std::weak_ptr<MutexDelayFunctor>>;

		using Seconds = MutexDelayFunctor::Seconds;

		enum class LockResult : std::uint32_t
		{
			Failure = 0,
			IsOwner,
			IsWaiter
		};

		MutexDelayFunctorManager() = default;
		~MutexDelayFunctorManager() noexcept = default;

		MutexDelayFunctorManager(const MutexDelayFunctorManager&) = delete;
		MutexDelayFunctorManager(MutexDelayFunctorManager&&) = delete;

		MutexDelayFunctorManager& operator=(const MutexDelayFunctorManager&) = delete;
		MutexDelayFunctorManager& operator=(MutexDelayFunctorManager&&) = delete;

		void OnGameDataReady();
		void OnSaveRevert();

		auto Lock(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId) -> LockResult;
		auto LockForSeconds(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId, Seconds a_timeoutSeconds) -> LockResult;

		bool TryLock(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId);

		bool Unlock(const Script::ScriptFunctionRef& a_functionRef);

	protected:
		friend class MutexDelayFunctor;

		[[nodiscard]] bool Contains(const Script::ScriptFunctionRef& a_functionRef) const;
		bool Add(const Script::ScriptFunctionRef& a_functionRef, const std::shared_ptr<MutexDelayFunctor>& a_delayFunctor);
		bool Remove(const Script::ScriptFunctionRef& a_functionRef, const std::shared_ptr<MutexDelayFunctor>& a_delayFunctor);
		void Clear() noexcept;

	private:
		static auto CreateImpl(const Script::ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<auto(const Script::ScriptFunctionRef&)->REX::NotNull<std::shared_ptr<MutexDelayFunctor>>>>& a_functorFactory)
			-> REX::NotNull<std::shared_ptr<MutexDelayFunctor>>;

		static auto CreateImpl(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId)
			-> REX::NotNull<std::shared_ptr<MutexDelayFunctor>>;

		static auto CreateImpl(const Script::ScriptFunctionRef& a_functionRef, RE::BSScript::StackID a_stackId, Seconds a_timeoutSeconds)
			-> REX::NotNull<std::shared_ptr<MutexDelayFunctor>>;

		bool UnlockImpl(const Script::ScriptFunctionRef& a_functionRef, const std::shared_ptr<MutexDelayFunctor>& a_delayFunctor);

		Script::ScriptFunctionRefMap<value_type> _functionRefMap;
	};

	[[nodiscard]] auto GetMutexDelayFunctorManager() -> const REX::NotNull<std::unique_ptr<MutexDelayFunctorManager>>&;
}
