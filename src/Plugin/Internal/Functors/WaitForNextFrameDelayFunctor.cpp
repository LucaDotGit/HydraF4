#include "Plugin/Internal/Functors/WaitForNextFrameDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	WaitForNextFrameDelayFunctor::WaitForNextFrameDelayFunctor(RE::BSScript::StackID a_stackId)
		: _stackId(a_stackId)
	{
		REX::Assert(_internalVM->IsWaitingOnLatent(a_stackId));
	}

	WaitForNextFrameDelayFunctor::WaitForNextFrameDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ISharedDelayFunctor(a_serializerTag)
	{
	}

	WaitForNextFrameDelayFunctor::~WaitForNextFrameDelayFunctor() noexcept = default;

	void WaitForNextFrameDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<WaitForNextFrameDelayFunctorFactory>();
	}

	const char* WaitForNextFrameDelayFunctor::GetObjectName() const
	{
		return WaitForNextFrameDelayFunctorFactory::OBJECT_NAME.data();
	}

	std::uint32_t WaitForNextFrameDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	bool WaitForNextFrameDelayFunctor::Run(RE::BSScript::Variable& a_outResult)
	{
		a_outResult.SetNone();
		return true;
	}

	bool WaitForNextFrameDelayFunctor::ShouldReschedule(std::int32_t& a_outDelayMS)
	{
		a_outDelayMS = F4SE::IDelayFunctor::EMPTY_DELAY;
		return false;
	}

	bool WaitForNextFrameDelayFunctor::ShouldResumeStack(RE::BSScript::StackID& a_outStackId)
	{
		if (!_internalVM->IsWaitingOnLatent(_stackId)) [[unlikely]] {
			REX::Assert(false);
			a_outStackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			return false;
		}

		a_outStackId = _stackId;
		return true;
	}

	bool WaitForNextFrameDelayFunctor::Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version)
	{
		auto stream = Save::SaveStream(a_serializer);
		if (!DelayFunctorUtility::BeginLoad(a_serializer, stream, SAVE_VERSION, a_version)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto stackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
		if (!stream.ReadValue(stackId)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!DelayFunctorUtility::EndLoad(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!_internalVM->IsWaitingOnLatent(stackId)) [[unlikely]] {
			_stackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			return true;
		}

		_stackId = stackId;
		return true;
	}

	bool WaitForNextFrameDelayFunctor::Save(const F4SE::SerializationInterface* a_serializer)
	{
		auto stream = Save::SaveStream(a_serializer);
		if (!DelayFunctorUtility::BeginSave(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_stackId)) [[unlikely]] {
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
