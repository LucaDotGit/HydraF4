#include "Plugin/Internal/Functors/QuickLoadDelayFunctor.hpp"

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp" // IWYU pragma: keep

namespace Plugin::Internal::Functors
{
	static constexpr auto SAVE_VERSION = 1ui32;

	QuickLoadDelayFunctor::QuickLoadDelayFunctor(RE::BSFixedString a_saveName, RE::BSScript::StackID a_stackId)
		: ILoadDelayFunctor(std::move(a_saveName), false, a_stackId)
	{
	}

	QuickLoadDelayFunctor::QuickLoadDelayFunctor(F4SE::SerializationTag a_serializerTag)
		: ILoadDelayFunctor(a_serializerTag)
	{
	}

	QuickLoadDelayFunctor::~QuickLoadDelayFunctor() noexcept = default;

	void QuickLoadDelayFunctor::OnPostLoad()
	{
		DelayFunctorUtility::RegisterObjectFactory<QuickLoadDelayFunctorFactory>();
	}

	const char* QuickLoadDelayFunctor::GetObjectName() const
	{
		return QuickLoadDelayFunctorFactory::OBJECT_NAME.data();
	}

	std::uint32_t QuickLoadDelayFunctor::GetObjectVersion() const
	{
		return SAVE_VERSION;
	}

	bool QuickLoadDelayFunctor::Run(RE::BSScript::Variable& a_outResult)
	{
		auto* saveLoadManager = RE::BGSSaveLoadManager::GetSingleton();
		if (!saveLoadManager) [[unlikely]] {
			REX::Fail("Failed to get the game's save load manager."sv);
		}

		if (saveLoadManager->quicksaveFileName != _saveName) {
			a_outResult.SetBool(false);
			return true;
		}

		if (!saveLoadManager->Quickload()) {
			a_outResult.SetBool(false);
			return true;
		}

		a_outResult.SetBool(true);
		return true;
	}
}
