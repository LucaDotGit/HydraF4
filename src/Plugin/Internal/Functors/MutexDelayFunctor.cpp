#include "Plugin/Internal/Functors/MutexDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp"
#include "Plugin/Internal/Functors/MutexDelayFunctorManager.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	MutexDelayFunctor::MutexDelayFunctor(
		Script::ScriptFunctionRef a_functionRef,
		RE::BSScript::StackID a_stackId)
		: _functionRef(std::move(a_functionRef)),
		  _stackId(a_stackId)
	{
		REX::Assert(_internalVM->IsWaitingOnLatent(a_stackId));
	}

	MutexDelayFunctor::MutexDelayFunctor(
		Script::ScriptFunctionRef a_functionRef,
		RE::BSScript::StackID a_stackId,
		Milliseconds a_timeoutMillis)
		: _functionRef(std::move(a_functionRef)),
		  _intervalMillis(a_timeoutMillis),
		  _stackId(a_stackId),
		  _flags(MutexFlags::HasTimeout),
		  _tempFlags(MutexFlags::HasTimeout)
	{
		REX::Assert(_internalVM->IsWaitingOnLatent(a_stackId));
	}

	MutexDelayFunctor::MutexDelayFunctor(
		Script::ScriptFunctionRef a_functionRef,
		RE::BSScript::StackID a_stackId,
		Seconds a_timeoutSeconds)
		: _functionRef(std::move(a_functionRef)),
		  _intervalMillis(std::chrono::duration_cast<Milliseconds>(a_timeoutSeconds)),
		  _stackId(a_stackId),
		  _flags(MutexFlags::HasTimeout),
		  _tempFlags(MutexFlags::HasTimeout)
	{
		REX::Assert(_internalVM->IsWaitingOnLatent(a_stackId));
	}

	MutexDelayFunctor::MutexDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ISharedDelayFunctor(a_serializerTag)
	{
	}

	MutexDelayFunctor::~MutexDelayFunctor() noexcept = default;

	void MutexDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<MutexDelayFunctorFactory>();
	}

	const Script::ScriptFunctionRef& MutexDelayFunctor::GetFunctionRef() const noexcept
	{
		return _functionRef;
	}

	MutexDelayFunctor::Milliseconds MutexDelayFunctor::GetIntervalMillis() const noexcept
	{
		return std::max(Milliseconds::zero(), _intervalMillis);
	}

	MutexDelayFunctor::Milliseconds MutexDelayFunctor::GetElapsedMillis() const noexcept
	{
		return std::max(Milliseconds::zero(), _elapsedMillis);
	}

	MutexDelayFunctor::Milliseconds MutexDelayFunctor::GetRemainingMillis() const noexcept
	{
		return std::max(Milliseconds::zero(), GetIntervalMillis() - GetElapsedMillis());
	}

	MutexDelayFunctor::Seconds MutexDelayFunctor::GetInitialSeconds() const noexcept
	{
		return std::chrono::duration_cast<Seconds>(GetIntervalMillis());
	}

	MutexDelayFunctor::Seconds MutexDelayFunctor::GetElapsedSeconds() const noexcept
	{
		return std::chrono::duration_cast<Seconds>(GetElapsedMillis());
	}

	MutexDelayFunctor::Seconds MutexDelayFunctor::GetRemainingSeconds() const noexcept
	{
		return std::chrono::duration_cast<Seconds>(GetRemainingMillis());
	}

	RE::BSScript::StackID MutexDelayFunctor::GetStackId() const noexcept
	{
		return _stackId;
	}

	REX::EnumSet<MutexDelayFunctor::MutexFlags> MutexDelayFunctor::GetFlags() const noexcept
	{
		return _flags;
	}

	bool MutexDelayFunctor::IsLocked() const noexcept
	{
		return _flags.any_atomic(MutexFlags::IsLocked);
	}

	bool MutexDelayFunctor::HasTimeout() const noexcept
	{
		return _flags.any_atomic(MutexFlags::HasTimeout);
	}

	bool MutexDelayFunctor::IsQueueForUnlock() const noexcept
	{
		return _flags.any_atomic(MutexFlags::IsQueuedForUnlock);
	}

	void MutexDelayFunctor::Lock() noexcept
	{
		_flags.set_atomic(MutexFlags::IsLocked);
	}

	void MutexDelayFunctor::Unlock() noexcept
	{
		_flags.reset_atomic(MutexFlags::IsLocked, MutexFlags::IsQueuedForUnlock);

		if (_internalVM->IsWaitingOnLatent(_stackId)) {
			_internalVM->ReturnLatentResult(_stackId);
		}
	}

	void MutexDelayFunctor::QueueForUnlock() noexcept
	{
		_flags.set_atomic(MutexFlags::IsQueuedForUnlock);
	}

	const char* MutexDelayFunctor::GetObjectName() const
	{
		return MutexDelayFunctorFactory::OBJECT_NAME.data();
	}

	std::uint32_t MutexDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	bool MutexDelayFunctor::Run(RE::BSScript::Variable& a_outResult)
	{
		a_outResult.SetNone();

		_tempFlags = _flags.get_atomic();
		_tempShouldResume = false;

		if (_tempFlags.any(MutexFlags::IsQueuedForUnlock)) {
			const auto isUnlocked = Functors::GetMutexDelayFunctorManager()->Remove(_functionRef, shared_from_this());

			_tempShouldResume = isUnlocked;
			return true;
		}

		if (_tempFlags.none(MutexFlags::IsLocked)) {
			_tempShouldResume = true;
			return true;
		}

		if (_tempFlags.none(MutexFlags::HasTimeout)) {
			return true;
		}

		const auto deltaMillis = DelayFunctorUtility::GetCurrentDeltaTimeMillis();
		if (deltaMillis <= Milliseconds::zero()) [[unlikely]] {
			REX::Assert(false);
			return true;
		}

		_elapsedMillis += deltaMillis;

		if (_elapsedMillis >= _intervalMillis) {
			const auto isUnlocked = Functors::GetMutexDelayFunctorManager()->Remove(_functionRef, shared_from_this());
			_tempShouldResume = isUnlocked;
		}

		return true;
	}

	bool MutexDelayFunctor::ShouldReschedule(std::int32_t& a_outDelayMS)
	{
		if (_tempShouldResume) {
			a_outDelayMS = F4SE::IDelayFunctor::EMPTY_DELAY;
			return false;
		}

		a_outDelayMS = F4SE::IDelayFunctor::RUN_ON_NEXT_FRAME_DELAY;
		return true;
	}

	bool MutexDelayFunctor::ShouldResumeStack(RE::BSScript::StackID& a_outStackId)
	{
		if (!_internalVM->IsWaitingOnLatent(_stackId)) {
			a_outStackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			return false;
		}

		a_outStackId = _stackId;
		return true;
	}

	bool MutexDelayFunctor::Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version)
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

		auto stackId = static_cast<RE::BSScript::StackID>(0);
		if (!stream.ReadValue(stackId)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto flags = MutexFlags::None;
		if (!stream.ReadValue(flags)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!DelayFunctorUtility::EndLoad(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!functionRef.has_value() || !_internalVM->IsWaitingOnLatent(stackId)) [[unlikely]] {
			_functionRef.reset();
			_startMillis = Milliseconds::zero();
			_intervalMillis = Milliseconds::zero();
			_elapsedMillis = Milliseconds::zero();
			_stackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			_flags = MutexFlags::None;
			_tempFlags = MutexFlags::None;

			Unlock();
			return true;
		}

		_functionRef = std::move(functionRef).value();
		_startMillis = static_cast<Milliseconds>(startMillis);
		_intervalMillis = static_cast<Milliseconds>(intervalMillis);
		_elapsedMillis = static_cast<Milliseconds>(elapsedMillis);
		_stackId = stackId;
		_flags = flags;
		_tempFlags = flags;

		if (_tempFlags.any(MutexFlags::IsLocked) && !Functors::GetMutexDelayFunctorManager()->Add(_functionRef, shared_from_this())) [[unlikely]] {
			REX::Assert(false);
			Unlock();
		}

		return true;
	}

	bool MutexDelayFunctor::Save(const F4SE::SerializationInterface* a_serializer)
	{
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

		if (!stream.WriteValue(_stackId)) [[unlikely]] {
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
}
