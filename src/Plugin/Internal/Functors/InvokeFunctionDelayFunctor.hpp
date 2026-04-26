#pragma once

#include "Plugin/Internal/Functors/IInvokeFunctionDelayFunctor.hpp"
#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

namespace Plugin::Internal::Functors
{
	class InvokeFunctionDelayFunctor final
		: public IInvokeFunctionDelayFunctor
	{
	public:
		InvokeFunctionDelayFunctor(
			Script::ScriptFunctionRef a_functionRef,
			RE::BSScript::StackID a_stackId,
			RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> a_args = nullptr);

		explicit InvokeFunctionDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~InvokeFunctionDelayFunctor() noexcept override;

		InvokeFunctionDelayFunctor(const InvokeFunctionDelayFunctor&) = delete;
		InvokeFunctionDelayFunctor(InvokeFunctionDelayFunctor&&) = delete;

		InvokeFunctionDelayFunctor& operator=(const InvokeFunctionDelayFunctor&) = delete;
		InvokeFunctionDelayFunctor& operator=(InvokeFunctionDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const Script::ScriptFunctionRef& GetFunctionRef() const noexcept { return _functionRef; }
		[[nodiscard]] const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& GetArgs() const noexcept { return _args; }

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Load(const F4SE::SerializationInterface* a_serializer, std::uint32_t a_version) override;
		bool Save(const F4SE::SerializationInterface* a_serializer) override;

		bool InvokeFunction(const REX::NotNull<RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>>& a_callbackFunctor) override;

	private:
		Script::ScriptFunctionRef _functionRef;
		RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant> _args{ nullptr };
	};

	using InvokeFunctionDelayFunctorFactory = F4SE::SharedObjectFactory<InvokeFunctionDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_InvokeFunctionDelayFunctorFactory">;
}
