#include "Plugin/Internal/Functors/LoadSaveDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp" // IWYU pragma: keep

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	LoadSaveDelayFunctor::LoadSaveDelayFunctor(RE::BSFixedString a_saveName, RE::BSScript::StackID a_stackId)
		: ILoadDelayFunctor(std::move(a_saveName), false, a_stackId)
	{
	}

	LoadSaveDelayFunctor::LoadSaveDelayFunctor(RE::BSFixedString a_saveName, bool a_silent, RE::BSScript::StackID a_stackId)
		: ILoadDelayFunctor(std::move(a_saveName), a_silent, a_stackId)
	{
	}

	LoadSaveDelayFunctor::LoadSaveDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ILoadDelayFunctor(a_serializerTag)
	{
	}

	LoadSaveDelayFunctor::~LoadSaveDelayFunctor() noexcept = default;

	void LoadSaveDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<LoadSaveDelayFunctorFactory>();
	}

	const char* LoadSaveDelayFunctor::GetObjectName() const
	{
		return LoadSaveDelayFunctorFactory::OBJECT_NAME.data();
	}

	std::uint32_t LoadSaveDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	bool LoadSaveDelayFunctor::Run(RE::BSScript::Variable& a_outResult)
	{
		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Fail("Failed to get the game's save load manager."sv);
		}

		constexpr auto CHECK_FOR_MODS = true;

		if (!saveLoadManager->LoadGame(_saveName.data(), CHECK_FOR_MODS, _isSilent)) {
			a_outResult.SetBool(false);
			return true;
		}

		a_outResult.SetBool(true);
		return true;
	}
}
