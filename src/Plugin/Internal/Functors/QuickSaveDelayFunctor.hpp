#pragma once

#include "Plugin/Internal/Functors/ISaveDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class QuickSaveDelayFunctor final
		: public ISaveDelayFunctor
	{
	public:
		explicit QuickSaveDelayFunctor(bool a_isSilent, RE::BSScript::StackID a_stackId);
		explicit QuickSaveDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~QuickSaveDelayFunctor() noexcept override;

		QuickSaveDelayFunctor(const QuickSaveDelayFunctor&) = delete;
		QuickSaveDelayFunctor(QuickSaveDelayFunctor&&) = delete;

		QuickSaveDelayFunctor& operator=(const QuickSaveDelayFunctor&) = delete;
		QuickSaveDelayFunctor& operator=(QuickSaveDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;
	};

	using QuickSaveDelayFunctorFactory = F4SE::SharedObjectFactory<QuickSaveDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_QuickSaveDelayFunctorFactory">;
}
