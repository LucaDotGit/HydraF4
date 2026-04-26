#include "Plugin/Internal/Functors/ISaveDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	ISaveDelayFunctor::ISaveDelayFunctor(bool a_isSilent, RE::BSScript::StackID a_stackId)
		: _isSilent(a_isSilent),
		  _stackId(a_stackId)
	{
		REX::Assert(_internalVM->IsWaitingOnLatent(a_stackId));
	}

	ISaveDelayFunctor::ISaveDelayFunctor(RE::BSFixedString a_saveName, bool a_isSilent, RE::BSScript::StackID a_stackId)
		: _saveName(std::move(a_saveName)),
		  _isSilent(a_isSilent),
		  _stackId(a_stackId)
	{
		REX::Assert(_internalVM->IsWaitingOnLatent(a_stackId));
	}

	ISaveDelayFunctor::ISaveDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ISharedDelayFunctor(a_serializerTag)
	{
	}

	ISaveDelayFunctor::~ISaveDelayFunctor() noexcept = default;

	bool ISaveDelayFunctor::ShouldReschedule(std::int32_t& a_outDelayMS)
	{
		a_outDelayMS = F4SE::IDelayFunctor::EMPTY_DELAY;
		return false;
	}

	bool ISaveDelayFunctor::ShouldResumeStack(RE::BSScript::StackID& a_outStackId)
	{
		if (!_internalVM->IsWaitingOnLatent(_stackId)) [[unlikely]] {
			REX::Assert(false);
			a_outStackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			return false;
		}

		a_outStackId = _stackId;
		return true;
	}

	bool ISaveDelayFunctor::Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version)
	{
		auto stream = Save::SaveStream(a_serializer);
		if (!DelayFunctorUtility::BeginLoad(a_serializer, stream, SAVE_VERSION, a_version)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto saveName = RE::BSFixedString();
		if (!stream.ReadString(saveName)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto isSilent = false;
		if (!stream.ReadValue(isSilent)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto stackId = static_cast<RE::BSScript::StackID>(0);
		if (!stream.ReadValue(stackId)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!DelayFunctorUtility::EndLoad(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!_internalVM->IsWaitingOnLatent(stackId)) [[unlikely]] {
			_saveName.clear();
			_isSilent = false;
			_stackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;

			return true;
		}

		_saveName = saveName;
		_isSilent = isSilent;
		_stackId = stackId;

		return true;
	}

	bool ISaveDelayFunctor::Save(const F4SE::SerializationInterface* a_serializer)
	{
		auto stream = Save::SaveStream(a_serializer);
		if (!DelayFunctorUtility::BeginSave(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteString(_saveName)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_isSilent)) [[unlikely]] {
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
