#pragma once

#include "Plugin/Internal/Functors/ILoadDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	class LoadSaveDelayFunctor final
		: public ILoadDelayFunctor
	{
	public:
		explicit LoadSaveDelayFunctor(RE::BSFixedString a_saveName, RE::BSScript::StackID a_stackId);
		explicit LoadSaveDelayFunctor(RE::BSFixedString a_saveName, bool a_silent, RE::BSScript::StackID a_stackId);
		explicit LoadSaveDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~LoadSaveDelayFunctor() noexcept override;

		LoadSaveDelayFunctor(const LoadSaveDelayFunctor&) = delete;
		LoadSaveDelayFunctor(LoadSaveDelayFunctor&&) = delete;

		LoadSaveDelayFunctor& operator=(const LoadSaveDelayFunctor&) = delete;
		LoadSaveDelayFunctor& operator=(LoadSaveDelayFunctor&&) = delete;

		static void OnPostLoad();

		[[nodiscard]] const char* GetObjectName() const override;
		[[nodiscard]] std::uint32_t GetObjectVersion() const override;

		bool Run(RE::BSScript::Variable& a_outResult) override;
	};

	using LoadSaveDelayFunctorFactory = F4SE::SharedObjectFactory<LoadSaveDelayFunctor, DELAY_FUNCTOR_FACTORY_PREFIX + "_LoadSaveDelayFunctorFactory">;
}
