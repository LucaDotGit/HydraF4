#include "Plugin/Internal/Functors/IInvokeFunctionDelayFunctor.hpp"

namespace Plugin::Internal::Functors::Impl
{
	class StackCallbackFunctor final
		: public RE::BSScript::IStackCallbackFunctor
	{
	public:
		explicit StackCallbackFunctor(std::weak_ptr<IInvokeFunctionDelayFunctor> a_delayFunctor)
			: _delayFunctor(std::move(a_delayFunctor))
		{
		}

		~StackCallbackFunctor() noexcept override = default;

		StackCallbackFunctor(const StackCallbackFunctor&) = delete;
		StackCallbackFunctor(StackCallbackFunctor&&) = delete;

		StackCallbackFunctor& operator=(const StackCallbackFunctor&) = delete;
		StackCallbackFunctor& operator=(StackCallbackFunctor&&) = delete;

		void CallQueued() override
		{
			return;
		}

		void CallCanceled() override
		{
			auto sharedDelayFunctor = _delayFunctor.lock();
			if (!sharedDelayFunctor) {
				return;
			}

			sharedDelayFunctor->ReturnFunction(RE::BSScript::NONE_VARIABLE);
		}

		void StartMultiDispatch() override
		{
			return;
		}

		void EndMultiDispatch() override
		{
			return;
		}

		void operator()(RE::BSScript::Variable a_returnValue) override
		{
			auto sharedDelayFunctor = _delayFunctor.lock();
			if (!sharedDelayFunctor) {
				return;
			}

			sharedDelayFunctor->ReturnFunction(a_returnValue);
		}

		bool CanSave() override
		{
			return false;
		}

	private:
		std::weak_ptr<IInvokeFunctionDelayFunctor> _delayFunctor;
	};
}

namespace Plugin::Internal::Functors
{
	IInvokeFunctionDelayFunctor::IInvokeFunctionDelayFunctor(RE::BSScript::StackID a_stackId)
		: _stackId(a_stackId)
	{
		REX::Assert(_internalVM->IsWaitingOnLatent(a_stackId));
	}

	IInvokeFunctionDelayFunctor::IInvokeFunctionDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ISharedDelayFunctor(a_serializerTag)
	{
	}

	IInvokeFunctionDelayFunctor::~IInvokeFunctionDelayFunctor() noexcept = default;

	bool IInvokeFunctionDelayFunctor::GetHasReturned() const noexcept
	{
		return _hasReturned.load(std::memory_order_acquire);
	}

	bool IInvokeFunctionDelayFunctor::Run(RE::BSScript::Variable& a_outResult)
	{
		a_outResult.SetNone();

		_tempShouldResume = false;

		const auto hasReturned = _hasReturned.load(std::memory_order_acquire);
		if (hasReturned) {
			_tempShouldResume = hasReturned;
			return true;
		}

		return DoRunFunction();
	}

	bool IInvokeFunctionDelayFunctor::ShouldReschedule(std::int32_t& a_outDelayMS)
	{
		if (!_tempShouldResume) {
			a_outDelayMS = F4SE::IDelayFunctor::RUN_ON_NEXT_FRAME_DELAY;
			return true;
		}

		a_outDelayMS = F4SE::IDelayFunctor::EMPTY_DELAY;
		return false;
	}

	bool IInvokeFunctionDelayFunctor::ShouldResumeStack(RE::BSScript::StackID& a_outStackId)
	{
		a_outStackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
		return false;
	}

	void IInvokeFunctionDelayFunctor::ReturnFunction(const RE::BSScript::Variable& a_returnValue)
	{
		if (_internalVM->IsWaitingOnLatent(_stackId)) {
			_internalVM->ReturnFromLatent(_stackId, a_returnValue);
		}

		_hasReturned.store(true, std::memory_order_release);
	}

	bool IInvokeFunctionDelayFunctor::DoRunFunction()
	{
		if (_hasInvoked) {
			return true;
		}

		const auto stackCallbackFunctor = REX::NotNull(RE::make_shared<Impl::StackCallbackFunctor>(shared_from_this()));

		const auto stackLock = RE::BSAutoLock(_internalVM->runningStacksLock);
		const auto nextStackId = _internalVM->nextStackID;

		if (!InvokeFunction(stackCallbackFunctor)) {
			_tempShouldResume = true;
			return true;
		}

		_hasInvoked = true;

		const auto& stackMap = _internalVM->allRunningStacks;

		const auto nextStackPair = stackMap.find(nextStackId);
		if (nextStackPair == stackMap.end()) [[unlikely]] {
			REX::Assert(false);
			_tempShouldResume = true;
			return true;
		}

		const auto nextStack = nextStackPair->second;
		if (!nextStack) [[unlikely]] {
			REX::Assert(false);
			_tempShouldResume = true;
			return true;
		}

		if (nextStack->callback != RE::static_pointer_cast<RE::BSScript::IStackCallbackFunctor>(stackCallbackFunctor.get())) [[unlikely]] {
			REX::Assert(false);
			_tempShouldResume = true;
			return true;
		}

		_callbackStackId = nextStackId;
		return true;
	}

	bool IInvokeFunctionDelayFunctor::RestoreStackCallback(RE::BSScript::StackID a_stackId)
	{
		if (a_stackId == F4SE::IDelayFunctor::EMPTY_STACK_ID) {
			return false;
		}

		const auto stackLock = RE::BSAutoLock(_internalVM->runningStacksLock);
		const auto& stackMap = _internalVM->allRunningStacks;

		const auto currentStackPair = stackMap.find(a_stackId);
		if (currentStackPair == stackMap.end()) {
			return false;
		}

		const auto currentStack = currentStackPair->second;
		if (!currentStack) {
			return false;
		}

		currentStack->callback = RE::make_shared<Impl::StackCallbackFunctor>(shared_from_this());
		return true;
	}
}
