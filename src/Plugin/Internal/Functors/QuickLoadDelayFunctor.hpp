#pragma once

#include "Plugin/Internal/Functors/ILoadDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class QuickLoadDelayFunctor final
		: public ILoadDelayFunctor
	{
	public:
		explicit QuickLoadDelayFunctor(RE::BSFixedString a_saveName, RE::BSScript::StackID a_stackId);
		explicit QuickLoadDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~QuickLoadDelayFunctor() noexcept override;

		QuickLoadDelayFunctor(const QuickLoadDelayFunctor&) = delete;
		QuickLoadDelayFunctor(QuickLoadDelayFunctor&&) = delete;

		QuickLoadDelayFunctor& operator=(const QuickLoadDelayFunctor&) = delete;
		QuickLoadDelayFunctor& operator=(QuickLoadDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;
	};

	using QuickLoadDelayFunctorFactory = F4SE::SharedObjectFactory<QuickLoadDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_QuickLoadDelayFunctorFactory">;
}
