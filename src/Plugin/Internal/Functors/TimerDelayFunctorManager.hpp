#pragma once

#include "Plugin/Internal/Functors/TimerDelayFunctor.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRefMap.hpp"

namespace Plugin::Internal::Functors
{
	class TimerDelayFunctorManager final
	{
	public:
		using value_type = std::weak_ptr<TimerDelayFunctor>;

		using Seconds = TimerDelayFunctor::Seconds;

		TimerDelayFunctorManager() = default;
		~TimerDelayFunctorManager() noexcept = default;

		TimerDelayFunctorManager(const TimerDelayFunctorManager&) = delete;
		TimerDelayFunctorManager(TimerDelayFunctorManager&&) = delete;

		TimerDelayFunctorManager& operator=(const TimerDelayFunctorManager&) = delete;
		TimerDelayFunctorManager& operator=(TimerDelayFunctorManager&&) = delete;

		void OnGameDataReady();
		void OnSaveRevert();

		[[nodiscard]] bool IsExistent(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsPersistent(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsRepeating(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsActive(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsPaused(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsRunningInRealTime(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsRunningInGameMode(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsRunningInMenuMode(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] bool IsRunningInGameTime(const Script::ScriptFunctionRef& a_functionRef) const;

		[[nodiscard]] Seconds GetIntervalSeconds(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] Seconds GetElapsedSeconds(const Script::ScriptFunctionRef& a_functionRef) const;
		[[nodiscard]] Seconds GetRemainingSeconds(const Script::ScriptFunctionRef& a_functionRef) const;

		bool Pause(const Script::ScriptFunctionRef& a_functionRef);
		bool Resume(const Script::ScriptFunctionRef& a_functionRef);
		bool Stop(const Script::ScriptFunctionRef& a_functionRef);
		bool StopAllGlobal(const RE::BSFixedString& a_namespace);
		bool StopAllLocal(RE::BSScript::ObjectHandle a_objectHandle, const RE::BSFixedString& a_namespace);
		bool StopAllLocal(const RE::BSTSmartPointer<RE::BSScript::Object>& a_object);

		bool StartRealTime(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent = false);
		bool StartGameMode(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent = false);
		bool StartMenuMode(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent = false);
		bool StartGameTime(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent = false);

		bool StartRepeatingRealTime(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent = false);
		bool StartRepeatingGameMode(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent = false);
		bool StartRepeatingMenuMode(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent = false);
		bool StartRepeatingGameTime(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args, bool a_persistent = false);

	protected:
		friend class TimerDelayFunctor;

		[[nodiscard]] bool Contains(const Script::ScriptFunctionRef& a_functionRef) const;
		bool Add(const Script::ScriptFunctionRef& a_functionRef, const std::shared_ptr<TimerDelayFunctor>& a_delayFunctor);
		bool Remove(const Script::ScriptFunctionRef& a_functionRef);
		void Clear() noexcept;

	private:
		bool CreateImpl(const Script::ScriptFunctionRef& a_functionRef,
			const REX::NotNull<std::function<auto(const Script::ScriptFunctionRef&)->REX::NotNull<std::shared_ptr<TimerDelayFunctor>>>>& a_functorFactory);

		bool StartImpl(const Script::ScriptFunctionRef& a_functionRef, Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args,
			REX::EnumSet<TimerDelayFunctor::TimerFlags> a_flags);

		Script::ScriptFunctionRefMap<value_type> _functionRefMap;
	};

	[[nodiscard]] auto GetTimerDelayFunctorManager() -> const REX::NotNull<std::unique_ptr<TimerDelayFunctorManager>>&;
}
