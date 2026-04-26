#include "Plugin/Internal/Functors/CustomSaveDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp" // IWYU pragma: keep

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	CustomSaveDelayFunctor::CustomSaveDelayFunctor(RE::BSFixedString a_saveName, bool a_isSilent, RE::BSScript::StackID a_stackId)
		: ISaveDelayFunctor(std::move(a_saveName), a_isSilent, a_stackId)
	{
	}

	CustomSaveDelayFunctor::CustomSaveDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ISaveDelayFunctor(a_serializerTag)
	{
	}

	CustomSaveDelayFunctor::~CustomSaveDelayFunctor() noexcept = default;

	void CustomSaveDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<CustomSaveDelayFunctorFactory>();
	}

	const char* CustomSaveDelayFunctor::GetObjectName() const
	{
		return CustomSaveDelayFunctorFactory::OBJECT_NAME.data();
	}

	std::uint32_t CustomSaveDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	bool CustomSaveDelayFunctor::Run(RE::BSScript::Variable& a_outResult)
	{
		if (_stackId == F4SE::IDelayFunctor::EMPTY_STACK_ID) {
			a_outResult.SetString(RE::BSFixedString());
			return true;
		}

		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Fail("Failed to get the game's save load manager."sv);
		}

		if (!saveLoadManager->CreateCompleteCustomSave(_saveName.data(), _isSilent)) {
			a_outResult.SetString(RE::BSFixedString());
			return true;
		}

		a_outResult.SetString(_saveName);
		return true;
	}
}
