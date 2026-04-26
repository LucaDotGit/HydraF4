#pragma once

#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp"

namespace Plugin::Internal::Functors::DelayFunctorUtility
{
	template <class T>
	__forceinline void RegisterObjectFactory()
		requires(std::derived_from<T, F4SE::IObjectFactory> &&
				 !std::is_pointer_v<T> &&
				 !std::is_reference_v<T> &&
				 (requires { { T::GetSingleton() } -> std::same_as<const REX::NotNull<std::unique_ptr<T>>&>; }) &&
				 (requires { { T::OBJECT_NAME } -> std::convertible_to<std::string_view>; }))
	{
		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, []() {
			if (!F4SE::GetObjectRegistry()->RegisterFactory(T::GetSingleton().get().get())) [[unlikely]] {
				REX::Fail(R"(Failed to register the object factory with name "{}".)"sv,
					static_cast<std::string_view>(T::OBJECT_NAME));
			}
		});
	}
}
