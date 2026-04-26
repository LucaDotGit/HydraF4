#include "Plugin/Internal/Functors/InvokeSetterPropertyDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	InvokeSetterPropertyDelayFunctor::InvokeSetterPropertyDelayFunctor(
		RE::BSTSmartPointer<RE::BSScript::Object> a_selfObject,
		RE::BSFixedString a_propertyName,
		RE::BSScript::Variable a_newValue,
		RE::BSScript::StackID a_stackId)
		: IInvokePropertyDelayFunctor(std::move(a_selfObject), a_propertyName, a_stackId),
		  _newValue(std::move(a_newValue))
	{
	}

	InvokeSetterPropertyDelayFunctor::InvokeSetterPropertyDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: IInvokePropertyDelayFunctor(a_serializerTag)
	{
	}

	InvokeSetterPropertyDelayFunctor::~InvokeSetterPropertyDelayFunctor() noexcept = default;

	void InvokeSetterPropertyDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<InvokeSetterPropertyDelayFunctorFactory>();
	}

	std::uint32_t InvokeSetterPropertyDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	const char* InvokeSetterPropertyDelayFunctor::GetObjectName() const
	{
		return InvokeSetterPropertyDelayFunctorFactory::OBJECT_NAME.data();
	}

	bool InvokeSetterPropertyDelayFunctor::Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version)
	{
		auto stream = Save::SaveStream(a_serializer);
		if (!DelayFunctorUtility::BeginLoad(a_serializer, stream, SAVE_VERSION, a_version)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto selfObject = RE::BSTSmartPointer<RE::BSScript::Object>();
		if (!stream.ReadObject(selfObject)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto propertyName = RE::BSFixedString();
		if (!stream.ReadString(propertyName)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		auto newValue = RE::BSScript::Variable();
		if (!stream.ReadValue(newValue)) [[unlikely]] {
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

		if (selfObject == nullptr || !_internalVM->IsWaitingOnLatent(stackId) || !RestoreStackCallback(callbackStackId)) [[unlikely]] {
			_selfObject.reset();
			_propertyName.clear();
			_newValue.reset();
			_stackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			_callbackStackId = F4SE::IDelayFunctor::EMPTY_STACK_ID;
			_hasInvoked = true;
			_hasReturned.store(true, std::memory_order_release);

			return true;
		}

		_selfObject = std::move(selfObject);
		_propertyName = std::move(propertyName);
		_newValue = std::move(newValue);
		_stackId = stackId;
		_callbackStackId = callbackStackId;
		_hasInvoked = hasInvoked;
		_hasReturned.store(hasReturned, std::memory_order_release);

		return true;
	}

	bool InvokeSetterPropertyDelayFunctor::Save(const F4SE::SerializationInterface* a_serializer)
	{
		auto stream = Save::SaveStream(a_serializer);
		if (!DelayFunctorUtility::BeginSave(a_serializer, stream)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteObject(_selfObject)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteString(_propertyName)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!stream.WriteValue(_newValue)) [[unlikely]] {
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

	bool InvokeSetterPropertyDelayFunctor::InvokeFunction(const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>>& a_callbackFunctor)
	{
		return _internalVM->SetPropertyValue(_selfObject, _propertyName.data(), _newValue, a_callbackFunctor.get());
	}

	void InvokeSetterPropertyDelayFunctor::ReturnFunction([[maybe_unused]] const RE::BSScript::Variable& a_returnValue)
	{
		const auto realReturnValue = RE::BSScript::Variable(true);
		IInvokeFunctionDelayFunctor::ReturnFunction(realReturnValue);
	}
}
