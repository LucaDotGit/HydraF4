#include "Plugin/Internal/Functors/TimerDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp"
#include "Plugin/Internal/Functors/TimerDelayFunctorManager.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	TimerDelayFunctor::TimerDelayFunctor(
		Script::ScriptFunctionRef a_functionRef,
		Milliseconds a_delayMillis,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args,
		TimerFlags a_flags)
		: _functionRef(std::move(a_functionRef)),
		  _startMillis(CalculateStartMillis(a_flags)),
		  _intervalMillis(a_delayMillis),
		  _args(a_args),
		  _flags(a_flags),
		  _tempFlags(a_flags)
	{
	}

	TimerDelayFunctor::TimerDelayFunctor(
		Script::ScriptFunctionRef a_functionRef,
		Seconds a_delaySeconds,
		const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args,
		TimerFlags a_flags)
		: _functionRef(std::move(a_functionRef)),
		  _startMillis(CalculateStartMillis(a_flags)),
		  _intervalMillis(std::chrono::duration_cast<Milliseconds>(a_delaySeconds)),
		  _args(a_args),
		  _flags(a_flags),
		  _tempFlags(a_flags)
	{
	}

	TimerDelayFunctor::TimerDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ISharedDelayFunctor(a_serializerTag)
	{
	}

	TimerDelayFunctor::~TimerDelayFunctor() noexcept = default;

	void TimerDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<TimerDelayFunctorFactory>();
	}

	const Script::ScriptFunctionRef& TimerDelayFunctor::GetFunctionRef() const noexcept
	{
		return _functionRef;
	}

	TimerDelayFunctor::Milliseconds TimerDelayFunctor::GetIntervalMillis() const noexcept
	{
		return std::max(Milliseconds::zero(), _intervalMillis);
	}

	TimerDelayFunctor::Milliseconds TimerDelayFunctor::GetElapsedMillis() const noexcept
	{
		const auto atomicElapsedMillis = std::atomic_ref(_elapsedMillis);
		return std::max(Milliseconds::zero(), atomicElapsedMillis.load(std::memory_order_acquire));
	}

	TimerDelayFunctor::Milliseconds TimerDelayFunctor::GetRemainingMillis() const noexcept
	{
		return std::max(Milliseconds::zero(), GetIntervalMillis() - GetElapsedMillis());
	}

	TimerDelayFunctor::Seconds TimerDelayFunctor::GetIntervalSeconds() const noexcept
	{
		return std::chrono::duration_cast<Seconds>(GetIntervalMillis());
	}

	TimerDelayFunctor::Seconds TimerDelayFunctor::GetElapsedSeconds() const noexcept
	{
		return std::chrono::duration_cast<Seconds>(GetElapsedMillis());
	}

	TimerDelayFunctor::Seconds TimerDelayFunctor::GetRemainingSeconds() const noexcept
	{
		return std::chrono::duration_cast<Seconds>(GetRemainingMillis());
	}

	const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& TimerDelayFunctor::GetArgs() const noexcept
	{
		return _args;
	}

	REX::EnumSet<TimerDelayFunctor::TimerFlags> TimerDelayFunctor::GetFlags() const noexcept
	{
		return _flags;
	}

	bool TimerDelayFunctor::IsPersistent() const noexcept
	{
		return _flags.any_atomic(TimerFlags::IsPersistent);
	}

	bool TimerDelayFunctor::IsRepeating() const noexcept
	{
		return _flags.any_atomic(TimerFlags::IsRepeating);
	}

	bool TimerDelayFunctor::IsActive() const noexcept
	{
		return _flags.none_atomic(TimerFlags::IsPaused, TimerFlags::IsStopped);
	}

	bool TimerDelayFunctor::IsPaused() const noexcept
	{
		return _flags.any_atomic(TimerFlags::IsPaused);
	}

	bool TimerDelayFunctor::IsStopped() const noexcept
	{
		return _flags.any_atomic(TimerFlags::IsStopped);
	}

	bool TimerDelayFunctor::IsRunningInRealTime() const noexcept
	{
		return _flags.none_atomic(TimerFlags::RunInGameMode, TimerFlags::RunInMenuMode, TimerFlags::RunInGameTime);
	}

	bool TimerDelayFunctor::IsRunningInGameMode() const noexcept
	{
		return _flags.any_atomic(TimerFlags::RunInGameMode);
	}

	bool TimerDelayFunctor::IsRunningInMenuMode() const noexcept
	{
		return _flags.any_atomic(TimerFlags::RunInMenuMode);
	}

	bool TimerDelayFunctor::IsRunningInGameTime() const noexcept
	{
		return _flags.any_atomic(TimerFlags::RunInGameTime);
	}

	void TimerDelayFunctor::Pause() noexcept
	{
		_flags.set_atomic(TimerFlags::IsPaused);
	}

	void TimerDelayFunctor::Resume() noexcept
	{
		_flags.reset_atomic(TimerFlags::IsPaused);
	}

	void TimerDelayFunctor::Stop() noexcept
	{
		_flags.set_atomic(TimerFlags::IsStopped);
	}

	const char* TimerDelayFunctor::GetObjectName() const
	{
		return TimerDelayFunctorFactory::OBJECT_NAME.data();
	}

	std::uint32_t TimerDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	bool TimerDelayFunctor::Run(RE::BSScript::Variable& a_outResult)
	{
		a_outResult.SetNone();

		_tempFlags = _flags.get_atomic();
		_tempShouldResume = false;

		if (_tempFlags.any(TimerFlags::IsPaused)) {
			return true;
		}

		if (_tempFlags.any(TimerFlags::IsStopped)) {
			if (!Functors::GetTimerDelayFunctorManager()->Remove(_functionRef)) [[unlikely]] {
				REX::Assert(false);
			}

			_tempShouldResume = true;
			return true;
		}

		if (!IsInCorrectGameMode(_tempFlags.get())) {
			return true;
		}

		const auto atomicElapsedMillis = std::atomic_ref(_elapsedMillis);
		auto tempElapsedMillis = atomicElapsedMillis.load(std::memory_order_acquire);

		const auto deltaMillis = CalculateDeltaMillis(_tempFlags.get(), _startMillis, tempElapsedMillis);
		if (deltaMillis <= Milliseconds::zero()) [[unlikely]] {
			REX::Assert(false);
			return true;
		}

		tempElapsedMillis += deltaMillis;
		if (tempElapsedMillis < _intervalMillis) {
			atomicElapsedMillis.store(tempElapsedMillis, std::memory_order_release);
			return true;
		}

		if (_tempFlags.any(TimerFlags::IsRepeating)) {
			tempElapsedMillis = Milliseconds::zero();
		}

		atomicElapsedMillis.store(tempElapsedMillis, std::memory_order_release);

		_functionRef.InvokeAppliedFunction(_args);

		if (_tempFlags.none(TimerFlags::IsRepeating)) {
			if (!Functors::GetTimerDelayFunctorManager()->Remove(_functionRef)) [[unlikely]] {
				REX::Assert(false);
			}

			_tempShouldResume = true;
		}

		return true;
	}

	bool TimerDelayFunctor::ShouldReschedule(std::int32_t& a_outDelayMS)
	{
		if (_tempShouldResume) {
			a_outDelayMS = F4SE::IDelayFunctor::EMPTY_DELAY;
			return false;
		}

		a_outDelayMS = F4SE::IDelayFunctor::RUN_ON_NEXT_FRAME_DELAY;
		return true;
	}

	bool TimerDelayFunctor::ShouldResumeStack(RE::BSScript::StackID& a_outStackId)
	{
		a_outStackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
		return false;
	}

	bool TimerDelayFunctor::Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version)
	{
		auto stream = Save::SaveStream(a_serializer);
		if (!DelayFunctorUtility::BeginLoad(a_serializer, stream, SAVE_VERSION, a_version)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto functionRef = Script::ScriptFunctionRef::LoadStream(stream);
		if (!functionRef.has_value() && functionRef.error() == Script::ScriptFunctionRef::LoadError::Failure) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto startMillis = static_cast<Milliseconds::rep>(0);
		if (!stream.ReadValue(startMillis)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto intervalMillis = static_cast<Milliseconds::rep>(0);
		if (!stream.ReadValue(intervalMillis)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto elapsedMillis = static_cast<Milliseconds::rep>(0);
		if (!stream.ReadValue(elapsedMillis)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto args = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>(nullptr);
		if (!stream.ReadArray(args.get())) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto flags = TimerFlags::None;
		if (!stream.ReadValue(flags)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!DelayFunctorUtility::EndLoad(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!functionRef.has_value() || !functionRef->IsFunctionOfParamTypes(args)) [[unlikely]] {
			_functionRef.reset();
			_startMillis = Milliseconds::zero();
			_intervalMillis = Milliseconds::zero();
			_elapsedMillis = Milliseconds::zero();
			_args.reset();
			_flags = TimerFlags::None;
			_tempFlags = TimerFlags::None;

			Stop();
			return true;
		}

		_functionRef = std::move(functionRef).value();
		_startMillis = static_cast<Milliseconds>(startMillis);
		_intervalMillis = static_cast<Milliseconds>(intervalMillis);
		_elapsedMillis = static_cast<Milliseconds>(elapsedMillis);
		_args = std::move(args);
		_flags = flags;
		_tempFlags = flags;

		if (_tempFlags.none(TimerFlags::IsStopped) && !Functors::GetTimerDelayFunctorManager()->Add(_functionRef, shared_from_this())) [[unlikely]] {
			REX::Assert(false);
			Stop();
		}

		return true;
	}

	bool TimerDelayFunctor::Save(const F4SE::SerializationInterface* a_serializer)
	{
		if (!IsPersistent()) {
			return true;
		}

		auto stream = Save::SaveStream(a_serializer);
		if (!DelayFunctorUtility::BeginSave(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!_functionRef.SaveStream(stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_startMillis.count())) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_intervalMillis.count())) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_elapsedMillis.count())) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteArray(_args.get())) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_flags.underlying())) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!DelayFunctorUtility::EndSave(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	TimerDelayFunctor::Milliseconds TimerDelayFunctor::CalculateStartMillis(TimerFlags a_flags)
	{
		const auto flagSet = REX::EnumSet(a_flags);
		if (flagSet.any(TimerFlags::RunInGameTime)) {
			return DelayFunctorUtility::GetCurrentGameTimeMillis();
		}

		return Milliseconds::zero();
	}

	TimerDelayFunctor::Milliseconds TimerDelayFunctor::CalculateDeltaMillis(TimerFlags a_flags, Milliseconds a_startMillis, Milliseconds a_elapsedMillis)
	{
		const auto flagSet = REX::EnumSet(a_flags);
		if (flagSet.any(TimerFlags::RunInGameMode, TimerFlags::RunInMenuMode)) {
			const auto deltaTimeMillis = DelayFunctorUtility::GetCurrentDeltaTimeMillis();
			const auto timeMultiplier = DelayFunctorUtility::GetCurrentTimeMultiplier();
			return std::chrono::duration_cast<Milliseconds>(deltaTimeMillis * timeMultiplier);
		}

		if (flagSet.any(TimerFlags::RunInGameTime)) {
			const auto gameTimeMillis = DelayFunctorUtility::GetCurrentGameTimeMillis();
			return gameTimeMillis - a_startMillis - a_elapsedMillis;
		}

		return DelayFunctorUtility::GetCurrentDeltaTimeMillis();
	}

	bool TimerDelayFunctor::IsInCorrectGameMode(TimerFlags a_flags)
	{
		const auto flagSet = REX::EnumSet(a_flags);
		const auto isInMenuMode = DelayFunctorUtility::IsInMenuMode();

		if (flagSet.any(TimerFlags::RunInGameMode, TimerFlags::RunInGameTime)) {
			return !isInMenuMode;
		}

		if (flagSet.any(TimerFlags::RunInMenuMode)) {
			return isInMenuMode;
		}

		return true;
	}
}