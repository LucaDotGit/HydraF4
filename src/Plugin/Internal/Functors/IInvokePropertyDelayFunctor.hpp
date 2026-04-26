#pragma once

#include "Plugin/Internal/Functors/IInvokeFunctionDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class IInvokePropertyDelayFunctor
		: public IInvokeFunctionDelayFunctor
	{
	public:
		IInvokePropertyDelayFunctor(
			RE::BSTSmartPointer<RE::BSScript::Object> a_selfObject,
			RE::BSFixedString a_propertyName,
			RE::BSScript::StackID a_stackId);

		explicit IInvokePropertyDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~IInvokePropertyDelayFunctor() noexcept override;

		IInvokePropertyDelayFunctor(const IInvokePropertyDelayFunctor&) = delete;
		IInvokePropertyDelayFunctor(IInvokePropertyDelayFunctor&&) = delete;

		IInvokePropertyDelayFunctor& operator=(const IInvokePropertyDelayFunctor&) = delete;
		IInvokePropertyDelayFunctor& operator=(IInvokePropertyDelayFunctor&&) = delete;

		[[nodiscard]] const RE::BSTSmartPointer<RE::BSScript::Object>& GetSelfObject() const noexcept { return _selfObject; }
		[[nodiscard]] const RE::BSFixedString& GetPropertyName() const noexcept { return _propertyName; }

	protected:
		RE::BSTSmartPointer<RE::BSScript::Object> _selfObject;
		RE::BSFixedString _propertyName;
	};
}
