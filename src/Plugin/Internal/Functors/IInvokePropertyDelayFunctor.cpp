#include "Plugin/Internal/Functors/IInvokePropertyDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	IInvokePropertyDelayFunctor::IInvokePropertyDelayFunctor(
		RE::BSTSmartPointer<RE::BSScript::Object> a_selfObject,
		RE::BSFixedString a_propertyName,
		RE::BSScript::StackID a_stackId)
		: IInvokeFunctionDelayFunctor(a_stackId),
		  _selfObject(std::move(a_selfObject)),
		  _propertyName(std::move(a_propertyName))
	{
	}

	IInvokePropertyDelayFunctor::IInvokePropertyDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: IInvokeFunctionDelayFunctor(a_serializerTag)
	{
	}

	IInvokePropertyDelayFunctor::~IInvokePropertyDelayFunctor() noexcept = default;
}
