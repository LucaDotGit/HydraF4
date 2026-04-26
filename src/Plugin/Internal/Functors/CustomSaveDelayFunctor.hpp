#pragma once

#include "Plugin/Internal/Functors/ISaveDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class CustomSaveDelayFunctor final
		: public ISaveDelayFunctor
	{
	public:
		explicit CustomSaveDelayFunctor(RE::BSFixedString a_saveName, bool a_isSilent, RE::BSScript::StackID a_stackId);
		explicit CustomSaveDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~CustomSaveDelayFunctor() noexcept override;

		CustomSaveDelayFunctor(const CustomSaveDelayFunctor&) = delete;
		CustomSaveDelayFunctor(CustomSaveDelayFunctor&&) = delete;

		CustomSaveDelayFunctor& operator=(const CustomSaveDelayFunctor&) = delete;
		CustomSaveDelayFunctor& operator=(CustomSaveDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;
	};

	using CustomSaveDelayFunctorFactory = F4SE::SharedObjectFactory<CustomSaveDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_CustomSaveDelayFunctorFactory">;
}
