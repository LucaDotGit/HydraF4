#include "Plugin/Internal/FunctorsManager.hpp"

#include "Plugin/Internal/Functors/MutexDelayFunctorManager.hpp"
#include "Plugin/Internal/Functors/TimerDelayFunctorManager.hpp"

#include "Plugin/Internal/Functors/AutoSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/CustomSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/FullSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/InvokeFunctionDelayFunctor.hpp"
#include "Plugin/Internal/Functors/InvokeGetterPropertyDelayFunctor.hpp"
#include "Plugin/Internal/Functors/InvokeSetterPropertyDelayFunctor.hpp"
#include "Plugin/Internal/Functors/LoadSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/MutexDelayFunctor.hpp"
#include "Plugin/Internal/Functors/QuickLoadDelayFunctor.hpp"
#include "Plugin/Internal/Functors/QuickSaveDelayFunctor.hpp"
#include "Plugin/Internal/Functors/TimerDelayFunctor.hpp"
#include "Plugin/Internal/Functors/WaitForNextFrameDelayFunctor.hpp"

namespace Plugin::Internal
{
	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void FunctorsManager::OnPostLoad()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		Functors::AutoSaveDelayFunctor::OnPostLoad();
		Functors::CustomSaveDelayFunctor::OnPostLoad();
		Functors::FullSaveDelayFunctor::OnPostLoad();
		Functors::InvokeFunctionDelayFunctor::OnPostLoad();
		Functors::InvokeGetterPropertyDelayFunctor::OnPostLoad();
		Functors::InvokeSetterPropertyDelayFunctor::OnPostLoad();
		Functors::LoadSaveDelayFunctor::OnPostLoad();
		Functors::MutexDelayFunctor::OnPostLoad();
		Functors::QuickLoadDelayFunctor::OnPostLoad();
		Functors::QuickSaveDelayFunctor::OnPostLoad();
		Functors::TimerDelayFunctor::OnPostLoad();
		Functors::WaitForNextFrameDelayFunctor::OnPostLoad();
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void FunctorsManager::OnGameDataReady()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		Functors::GetMutexDelayFunctorManager()->OnGameDataReady();
		Functors::GetTimerDelayFunctorManager()->OnGameDataReady();
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void FunctorsManager::OnSaveRevert()
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		Functors::GetMutexDelayFunctorManager()->OnSaveRevert();
		Functors::GetTimerDelayFunctorManager()->OnSaveRevert();
	}
}
