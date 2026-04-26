#include "Plugin/Internal/Functors/DelayFunctorUtility.hpp"

#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Functors::DelayFunctorUtility
{
	bool BeginLoad(const F4SE::SerializationInterface* a_serializer, Save::SaveStream& a_stream,
		std::uint32_t a_expectedVersion, std::uint32_t a_actualVersion)
	{
		if (!a_serializer) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (a_expectedVersion != a_actualVersion) {
			return true;
		}

		auto streamSize = static_cast<std::uint32_t>(0);
		if (!a_serializer->ReadRecordData(streamSize)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		a_stream = Save::SaveStream(a_serializer, streamSize);
		if (!a_serializer->ReadRecordData(a_stream.data(), static_cast<std::uint32_t>(a_stream.size()))) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool EndLoad(const F4SE::SerializationInterface* a_serializer, [[maybe_unused]] Save::SaveStream& a_stream)
	{
		if (!a_serializer) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool BeginSave(const F4SE::SerializationInterface* a_serializer, Save::SaveStream& a_stream)
	{
		if (!a_serializer) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!a_stream.WriteValue(static_cast<std::uint32_t>(0))) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool EndSave(const F4SE::SerializationInterface* a_serializer, Save::SaveStream& a_stream)
	{
		if (!a_serializer) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		a_stream.GetMemoryStream().rewind();

		if (!a_stream.WriteValue(static_cast<std::uint32_t>(a_stream.size() - sizeof(std::uint32_t)))) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!a_serializer->WriteRecordData(a_stream.data(), static_cast<std::uint32_t>(a_stream.size()))) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	bool IsInMenuMode()
	{
		const auto* main = RE::Main::GetSingleton();
		if (!main) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return main->inMenuMode;
	}

	REX::NotNull<RE::BSTimer*> GetCurrentTimer()
	{
		const auto* main = RE::Main::GetSingleton();
		if (!main) [[unlikely]] {
			REX::Fail("Failed to get the game's main singleton."sv);
		}

		if (!main->inMenuMode) {
			return RE::BSTimer::GetSingleton();
		}

		auto* ui = RE::UI::GetSingleton();
		if (!ui) [[unlikely]] {
			REX::Fail("Failed to get the game's UI singleton."sv);
		}

		return std::addressof(ui->timer);
	}

	DelayFunctorUtility::Milliseconds GetCurrentDeltaTimeMillis()
	{
		const auto currentTimer = GetCurrentTimer();
		const auto deltaTime = currentTimer->GetDeltaTime();
		return std::chrono::duration_cast<Milliseconds>(deltaTime);
	}

	DelayFunctorUtility::Milliseconds GetCurrentGameTimeMillis()
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return Milliseconds::zero();
		}

		const auto passedGameDays = calendar->GetDaysPassed();
		return std::chrono::duration_cast<Milliseconds>(passedGameDays);
	}

	REX::Float32 GetCurrentTimeMultiplier()
	{
		const auto currentTimer = GetCurrentTimer();
		if (!currentTimer->GetRuntimeData().useGlobalTimeMultiplierTarget) {
			return 1.0_f32;
		}

		return RE::BSTimer::QGlobalTimeMultiplierTarget();
	}
}
