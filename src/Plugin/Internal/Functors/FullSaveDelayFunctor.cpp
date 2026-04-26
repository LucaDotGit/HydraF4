#include "Plugin/Internal/Functors/FullSaveDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp" // IWYU pragma: keep

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	FullSaveDelayFunctor::FullSaveDelayFunctor(bool a_isSilent, RE::BSScript::StackID a_stackId)
		: ISaveDelayFunctor(a_isSilent, a_stackId)
	{
	}

	FullSaveDelayFunctor::FullSaveDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ISaveDelayFunctor(a_serializerTag)
	{
	}

	FullSaveDelayFunctor::~FullSaveDelayFunctor() noexcept = default;

	void FullSaveDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<FullSaveDelayFunctorFactory>();
	}

	const char* FullSaveDelayFunctor::GetObjectName() const
	{
		return FullSaveDelayFunctorFactory::OBJECT_NAME.data();
	}

	std::uint32_t FullSaveDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	bool FullSaveDelayFunctor::Run(RE::BSScript::Variable& a_outResult)
	{
		if (_stackId == F4SE::IDelayFunctor::EMPTY_STACK_ID) {
			a_outResult.SetString(RE::BSFixedString());
			return true;
		}

		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Fail("Failed to get the game's save load manager."sv);
		}

		if (!saveLoadManager->CreateCompleteFullSave(_isSilent)) {
			a_outResult.SetString(RE::BSFixedString());
			return true;
		}

		_saveName = RE::BSFixedString(saveLoadManager->mostRecentSaveGame);
		a_outResult.SetString(_saveName);

		return true;
	}
}
