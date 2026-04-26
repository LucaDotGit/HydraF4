#pragma once

namespace Plugin::Internal::Save
{
	class SaveStream;
}

namespace Plugin::Internal::Functors::DelayFunctorUtility
{
	using Milliseconds = RE::BSTimer::Milliseconds;

	template <class T>
	__forceinline void RegisterObjectFactory()
		requires(std::derived_from<T, F4SE::IObjectFactory> &&
				 !std::is_pointer_v<T> &&
				 !std::is_reference_v<T> &&
				 (requires { { T::GetSingleton() } -> std::same_as<const REX::NotNull<std::unique_ptr<T>>&>; }) &&
				 (requires { { T::OBJECT_NAME } -> std::convertible_to<std::string_view>; }));

	[[nodiscard]] bool BeginLoad(const F4SE::SerializationInterface* a_serializer, Save::SaveStream& a_stream,
		std::uint32_t a_expectedVersion, std::uint32_t a_actualVersion);
	[[nodiscard]] bool EndLoad(const F4SE::SerializationInterface* a_serializer, Save::SaveStream& a_stream);

	[[nodiscard]] bool BeginSave(const F4SE::SerializationInterface* a_serializer, Save::SaveStream& a_stream);
	[[nodiscard]] bool EndSave(const F4SE::SerializationInterface* a_serializer, Save::SaveStream& a_stream);

	[[nodiscard]] bool IsInMenuMode();

	[[nodiscard]] REX::NotNull<RE::BSTimer*> GetCurrentTimer();

	[[nodiscard]] Milliseconds GetCurrentDeltaTimeMillis();
	[[nodiscard]] Milliseconds GetCurrentGameTimeMillis();

	[[nodiscard]] REX::Float32 GetCurrentTimeMultiplier();
}

#include "Plugin/Internal/Functors/DelayFunctorUtility.inl"
