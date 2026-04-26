#include "Plugin/Internal/Functors/InvokeFunctionDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	InvokeFunctionDelayFunctor::InvokeFunctionDelayFunctor(
		Script::ScriptFunctionRef a_functionRef,
		RE::BSScript::StackID a_stackId,
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args)
		: IInvokeFunctionDelayFunctor(a_stackId),
		  _functionRef(std::move(a_functionRef)),
		  _args(std::move(a_args))
	{
	}

	InvokeFunctionDelayFunctor::InvokeFunctionDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: IInvokeFunctionDelayFunctor(a_serializerTag)
	{
	}

	InvokeFunctionDelayFunctor::~InvokeFunctionDelayFunctor() noexcept = default;

	void InvokeFunctionDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<InvokeFunctionDelayFunctorFactory>();
	}

	const char* InvokeFunctionDelayFunctor::GetObjectName() const
	{
		return InvokeFunctionDelayFunctorFactory::OBJECT_NAME.data();
	}

	std::uint32_t InvokeFunctionDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	bool InvokeFunctionDelayFunctor::Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version)
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

		auto args = RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>(nullptr);
		if (!stream.ReadArray(args.get())) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto stackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
		if (!stream.ReadValue(stackId)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto callbackStackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
		if (!stream.ReadValue(callbackStackId)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto hasInvoked = false;
		if (!stream.ReadValue(hasInvoked)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto hasReturned = false;
		if (!stream.ReadValue(hasReturned)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!DelayFunctorUtility::EndLoad(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!functionRef.has_value() || !_internalVM->IsWaitingOnLatent(stackId) || !RestoreStackCallback(callbackStackId)) [[unlikely]] {
			_functionRef.reset();
			_args.reset();
			_stackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			_callbackStackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			_hasInvoked = true;
			_hasReturned.store(true, std::memory_order_release);

			return true;
		}

		_functionRef = std::move(functionRef).value();
		_args = std::move(args);
		_stackId = stackId;
		_callbackStackId = callbackStackId;
		_hasInvoked = hasInvoked;
		_hasReturned.store(hasReturned, std::memory_order_release);

		return true;
	}

	bool InvokeFunctionDelayFunctor::Save(const F4SE::SerializationInterface* a_serializer)
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

		if (!stream.WriteArray(_args.get())) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_stackId)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_callbackStackId)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_hasInvoked)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_hasReturned.load(std::memory_order_acquire))) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!DelayFunctorUtility::EndSave(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool InvokeFunctionDelayFunctor::InvokeFunction(const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>>& a_callbackFunctor)
	{
		return _functionRef.InvokeAppliedFunctionWithCallback(_args, a_callbackFunctor.get());
	}
}
