#include "Plugin/ScaleformManager.hpp"

#include "Plugin/Scaleform/Form.hpp"
#include "Plugin/Scaleform/Plugins.hpp"
#include "Plugin/Scaleform/Script.hpp"

namespace Plugin::ScaleformManager
{
	void Setup(REX::NotNull<const F4SE::ScaleformInterface*> a_scaleform)
	{
		static constexpr auto FUNCTION_NAME = REX::GetCurrentFunctionName();

		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, [a_scaleform]() {
			REX::LogDebug("{}"sv,
				FUNCTION_NAME);

			a_scaleform->Register(OBJECT_NAME.data(), Callback);
		});
	}

	bool Callback(::Scaleform::GFx::Movie* a_movie, ::Scaleform::GFx::Value* a_value)
	{
		if (!a_movie) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!a_value) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		Scaleform::Form::RegisterFunctions(a_movie, a_value);
		Scaleform::Script::RegisterFunctions(a_movie, a_value);
		Scaleform::Plugins::RegisterFunctions(a_movie, a_value);

		return true;
	}
}
