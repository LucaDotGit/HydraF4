#pragma once

#include "Plugin/Internal/Functors/IInvokePropertyDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class InvokeGetterPropertyDelayFunctor final
		: public IInvokePropertyDelayFunctor
	{
	public:
		InvokeGetterPropertyDelayFunctor(
			RE::BSTSmartPointer<RE::BSScript::Object> a_selfObject,
			RE::BSFixedString a_propertyName,
			RE::BSScript::StackID a_stackId);

		explicit InvokeGetterPropertyDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~InvokeGetterPropertyDelayFunctor() noexcept override;

		InvokeGetterPropertyDelayFunctor(const InvokeGetterPropertyDelayFunctor&) = delete;
		InvokeGetterPropertyDelayFunctor(InvokeGetterPropertyDelayFunctor&&) = delete;

		InvokeGetterPropertyDelayFunctor& operator=(const InvokeGetterPropertyDelayFunctor&) = delete;
		InvokeGetterPropertyDelayFunctor& operator=(InvokeGetterPropertyDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] std::uint32_t GetObjectVersion() const override;
		[[nodiscard]] const char* GetObjectName() const override;

		bool Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version) override;
		bool Save(const F4SE::SerializationInterface* a_serializer) override;

		bool InvokeFunction(const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>>& a_callbackFunctor) override;
	};

	using InvokeGetterPropertyDelayFunctorFactory = F4SE::SharedObjectFactory<InvokeGetterPropertyDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_InvokeGetterPropertyDelayFunctorFactory">;
}
