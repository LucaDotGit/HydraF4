#pragma once

#include "Plugin/Internal/Functors/ISharedDelayFunctor.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

namespace Plugin::Internal::Functors
{
	class TimerDelayFunctor final
		: public ISharedDelayFunctor<TimerDelayFunctor>
	{
	public:
		using Milliseconds = RE::BSTimer::Milliseconds;
		using Seconds = RE::BSTimer::Seconds;

		enum class TimerFlags : std::uint32_t
		{
			None = 0,
			IsPersistent = 1 << 0,
			IsRepeating = 1 << 1,
			IsPaused = 1 << 2,
			IsStopped = 1 << 3,
			RunInGameMode = 1 << 4,
			RunInMenuMode = 1 << 5,
			RunInGameTime = 1 << 6
		};

		TimerDelayFunctor(
			Script::ScriptFunctionRef a_functionRef,
			Milliseconds a_delayMillis,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args = nullptr,
			TimerFlags a_flags = TimerFlags::None);

		TimerDelayFunctor(
			Script::ScriptFunctionRef a_functionRef,
			Seconds a_delaySeconds,
			const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args = nullptr,
			TimerFlags a_flags = TimerFlags::None);

		explicit TimerDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~TimerDelayFunctor() noexcept override;

		TimerDelayFunctor(const TimerDelayFunctor&) = delete;
		TimerDelayFunctor(TimerDelayFunctor&&) = delete;

		TimerDelayFunctor& operator=(const TimerDelayFunctor&) = delete;
		TimerDelayFunctor& operator=(TimerDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const Script::ScriptFunctionRef& GetFunctionRef() const noexcept;
		[[nodiscard]] Milliseconds GetIntervalMillis() const noexcept;
		[[nodiscard]] Milliseconds GetElapsedMillis() const noexcept;
		[[nodiscard]] Milliseconds GetRemainingMillis() const noexcept;
		[[nodiscard]] Seconds GetIntervalSeconds() const noexcept;
		[[nodiscard]] Seconds GetElapsedSeconds() const noexcept;
		[[nodiscard]] Seconds GetRemainingSeconds() const noexcept;
		[[nodiscard]] const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& GetArgs() const noexcept;
		[[nodiscard]] REX::EnumSet<TimerFlags> GetFlags() const noexcept;

		[[nodiscard]] bool IsPersistent() const noexcept;
		[[nodiscard]] bool IsRepeating() const noexcept;
		[[nodiscard]] bool IsActive() const noexcept;
		[[nodiscard]] bool IsPaused() const noexcept;
		[[nodiscard]] bool IsStopped() const noexcept;
		[[nodiscard]] bool IsRunningInRealTime() const noexcept;
		[[nodiscard]] bool IsRunningInGameMode() const noexcept;
		[[nodiscard]] bool IsRunningInMenuMode() const noexcept;
		[[nodiscard]] bool IsRunningInGameTime() const noexcept;

		void Pause() noexcept;
		void Resume() noexcept;
		void Stop() noexcept;

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;

		bool ShouldReschedule(std::int32_t& a_outDelayMS) override;
		bool ShouldResumeStack(RE::BSScript::StackID& a_outStackId) override;

		bool Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version) override;
		bool Save(const F4SE::SerializationInterface* a_serializer) override;

	private:
		[[nodiscard]] static Milliseconds CalculateStartMillis(TimerFlags a_flags);
		[[nodiscard]] static Milliseconds CalculateDeltaMillis(TimerFlags a_flags, Milliseconds a_startMillis, Milliseconds a_elapsedMillis);
		[[nodiscard]] static bool IsInCorrectGameMode(TimerFlags a_flags);

		Script::ScriptFunctionRef _functionRef;
		Milliseconds _startMillis{ Milliseconds::zero() };
		Milliseconds _intervalMillis{ Milliseconds::zero() };
		Milliseconds _elapsedMillis{ Milliseconds::zero() };
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> _args{ nullptr };
		REX::EnumSet<TimerFlags> _flags{ TimerFlags::None };
		REX::EnumSet<TimerFlags> _tempFlags{ TimerFlags::None };
		bool _tempShouldResume{ false };
	};

	using TimerDelayFunctorFactory = F4SE::SharedObjectFactory<TimerDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_TimerDelayFunctorFactory">;
}
