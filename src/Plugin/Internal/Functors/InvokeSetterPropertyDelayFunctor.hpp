#pragma once

#include "Plugin/Internal/Functors/IInvokePropertyDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class InvokeSetterPropertyDelayFunctor final
		: public IInvokePropertyDelayFunctor
	{
	public:
		InvokeSetterPropertyDelayFunctor(
			RE::BSTSmartPointer<RE::BSScript::Object> a_selfObject,
			RE::BSFixedString a_propertyName,
			RE::BSScript::Variable a_newValue,
			RE::BSScript::StackID a_stackId);

		explicit InvokeSetterPropertyDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~InvokeSetterPropertyDelayFunctor() noexcept override;

		InvokeSetterPropertyDelayFunctor(const InvokeSetterPropertyDelayFunctor&) = delete;
		InvokeSetterPropertyDelayFunctor(InvokeSetterPropertyDelayFunctor&&) = delete;

		InvokeSetterPropertyDelayFunctor& operator=(const InvokeSetterPropertyDelayFunctor&) = delete;
		InvokeSetterPropertyDelayFunctor& operator=(InvokeSetterPropertyDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const RE::BSScript::Variable& GetNewValue() const noexcept { return _newValue; }

		[[nodiscard]] std::uint32_t GetObjectVersion() const override;
		[[nodiscard]] const char* GetObjectName() const override;

		bool Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version) override;
		bool Save(const F4SE::SerializationInterface* a_serializer) override;

		bool InvokeFunction(const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>>& a_callbackFunctor) override;
		void ReturnFunction(const RE::BSScript::Variable& a_returnValue) override;

	private:
		RE::BSScript::Variable _newValue;
	};

	using InvokeSetterPropertyDelayFunctorFactory = F4SE::SharedObjectFactory<InvokeSetterPropertyDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_InvokeSetterPropertyDelayFunctorFactory">;
}
