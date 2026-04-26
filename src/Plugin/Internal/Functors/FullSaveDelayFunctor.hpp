#pragma once

#include "Plugin/Internal/Functors/ISaveDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class FullSaveDelayFunctor final
		: public ISaveDelayFunctor
	{
	public:
		explicit FullSaveDelayFunctor(bool a_isSilent, RE::BSScript::StackID a_stackId);
		explicit FullSaveDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~FullSaveDelayFunctor() noexcept override;

		FullSaveDelayFunctor(const FullSaveDelayFunctor&) = delete;
		FullSaveDelayFunctor(FullSaveDelayFunctor&&) = delete;

		FullSaveDelayFunctor& operator=(const FullSaveDelayFunctor&) = delete;
		FullSaveDelayFunctor& operator=(FullSaveDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;
	};

	using FullSaveDelayFunctorFactory = F4SE::SharedObjectFactory<FullSaveDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_FullSaveDelayFunctorFactory">;
}
