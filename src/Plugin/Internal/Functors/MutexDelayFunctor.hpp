#pragma once

#include "Plugin/Internal/Functors/ISharedDelayFunctor.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

namespace Plugin::Internal::Functors
{
	class MutexDelayFunctor final
		: public ISharedDelayFunctor<MutexDelayFunctor>
	{
	public:
		using Milliseconds = RE::BSTimer::Milliseconds;
		using Seconds = RE::BSTimer::Seconds;

		enum class MutexFlags : std::uint32_t
		{
			None = 0,
			IsLocked = 1 << 0,
			HasTimeout = 1 << 1,
			IsQueuedForUnlock = 1 << 2
		};

		MutexDelayFunctor(
			Script::ScriptFunctionRef a_functionRef,
			RE::BSScript::StackID a_stackId);

		MutexDelayFunctor(
			Script::ScriptFunctionRef a_functionRef,
			RE::BSScript::StackID a_stackId,
			Milliseconds a_timeoutMillis);

		MutexDelayFunctor(
			Script::ScriptFunctionRef a_functionRef,
			RE::BSScript::StackID a_stackId,
			Seconds a_timeoutSeconds);

		explicit MutexDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~MutexDelayFunctor() noexcept override;

		MutexDelayFunctor(const MutexDelayFunctor&) = delete;
		MutexDelayFunctor(MutexDelayFunctor&&) = delete;

		MutexDelayFunctor& operator=(const MutexDelayFunctor&) = delete;
		MutexDelayFunctor& operator=(MutexDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const Script::ScriptFunctionRef& GetFunctionRef() const noexcept;
		[[nodiscard]] Milliseconds GetIntervalMillis() const noexcept;
		[[nodiscard]] Milliseconds GetElapsedMillis() const noexcept;
		[[nodiscard]] Milliseconds GetRemainingMillis() const noexcept;
		[[nodiscard]] Seconds GetInitialSeconds() const noexcept;
		[[nodiscard]] Seconds GetElapsedSeconds() const noexcept;
		[[nodiscard]] Seconds GetRemainingSeconds() const noexcept;
		[[nodiscard]] RE::BSScript::StackID GetStackId() const noexcept;
		[[nodiscard]] REX::EnumSet<MutexFlags> GetFlags() const noexcept;

		[[nodiscard]] bool IsLocked() const noexcept;
		[[nodiscard]] bool HasTimeout() const noexcept;
		[[nodiscard]] bool IsQueueForUnlock() const noexcept;

		void Lock() noexcept;
		void Unlock() noexcept;
		void QueueForUnlock() noexcept;

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;

		bool ShouldReschedule(std::int32_t& a_outDelayMS) override;
		bool ShouldResumeStack(RE::BSScript::StackID& a_outStackId) override;

		bool Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version) override;
		bool Save(const F4SE::SerializationInterface* a_serializer) override;

	private:
		Script::ScriptFunctionRef _functionRef;
		Milliseconds _startMillis{ Milliseconds::zero() };
		Milliseconds _intervalMillis{ Milliseconds::zero() };
		Milliseconds _elapsedMillis{ Milliseconds::zero() };
		RE::BSScript::StackID _stackId{ F4SE::IDelayFunctor::EMPTY_STACK_ID };
		REX::EnumSet<MutexFlags> _flags{ MutexFlags::None };
		REX::EnumSet<MutexFlags> _tempFlags{ MutexFlags::None };
		bool _tempShouldResume{ false };
	};

	using MutexDelayFunctorFactory = F4SE::SharedObjectFactory<MutexDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_MutexDelayFunctorFactory">;
}
