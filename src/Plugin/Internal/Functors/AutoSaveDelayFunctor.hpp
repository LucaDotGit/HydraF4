#pragma once

#include "Plugin/Internal/Functors/ISaveDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class AutoSaveDelayFunctor final
		: public ISaveDelayFunctor
	{
	public:
		explicit AutoSaveDelayFunctor(bool a_isSilent, RE::BSScript::StackID a_stackId);
		explicit AutoSaveDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~AutoSaveDelayFunctor() noexcept override;

		AutoSaveDelayFunctor(const AutoSaveDelayFunctor&) = delete;
		AutoSaveDelayFunctor(AutoSaveDelayFunctor&&) = delete;

		AutoSaveDelayFunctor& operator=(const AutoSaveDelayFunctor&) = delete;
		AutoSaveDelayFunctor& operator=(AutoSaveDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;
	};

	using AutoSaveDelayFunctorFactory = F4SE::SharedObjectFactory<AutoSaveDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_AutoSaveDelayFunctorFactory">;
}
