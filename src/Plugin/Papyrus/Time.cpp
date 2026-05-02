#include "Plugin/Papyrus/Time.hpp"

#include "Plugin/Internal/Structs/DateTimes.hpp"

namespace Plugin::Papyrus::Time::Impl
{
	static constexpr auto NUMERIC_DAYS = []() consteval noexcept {
		constexpr const auto& DAYS = RE::Calendar::DAYS;

		auto result = std::array<std::uint32_t, DAYS.size()>();

		for (auto i = static_cast<std::size_t>(0); i < DAYS.size(); i++) {
			result[i] = DAYS[i].c_encoding();
		}

		return result;
	}();

	static constexpr auto NUMERIC_MONTHS = []() consteval noexcept {
		constexpr const auto& MONTHS = RE::Calendar::MONTHS;

		auto result = std::array<std::uint32_t, MONTHS.size()>();

		for (auto i = static_cast<std::size_t>(0); i < MONTHS.size(); i++) {
			result[i] = static_cast<std::uint32_t>(MONTHS[i]);
		}

		return result;
	}();
}

namespace Plugin::Papyrus::Time
{
	using DateTimeStruct = ::Plugin::Internal::Structs::DateTimes::DateTimeData;

	static REX::Float32 GetDeltaTime(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* timer = RE::BSTimer::GetSingleton();
		if (!timer) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		return std::atomic_ref(timer->delta).load(std::memory_order_acquire);
	}

	static REX::Float32 GetElapsedTime(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* timer = RE::BSTimer::GetSingleton();
		if (!timer) [[unlikely]] {
			REX::Assert(false);
			return 0.0_f32;
		}

		const auto initTime = std::atomic_ref(timer->highPrecisionInitTime).load(std::memory_order_acquire);
		const auto initTimeMillis = RE::BSTimer::Milliseconds(initTime);
		return std::chrono::duration_cast<RE::BSTimer::Seconds>(initTimeMillis).count();
	}

	static REX::Float32 GetTimeMultiplier(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return std::atomic_ref(RE::BSTimer::QGlobalTimeMultiplier()).load(std::memory_order_acquire);
	}

	static REX::Float32 GetDefaultTimeScale(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return RE::Calendar::DEFAULT_TIME_SCALE.count();
	}

	static REX::Float32 GetTimeScale(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return RE::Calendar::DEFAULT_TIME_SCALE.count();
		}

		const auto timeScale = calendar->GetTimeScale();
		return timeScale.count();
	}

	static std::uint32_t GetDayOfWeek(RE::BSScript::StaticTag /*a_staticTag*/,
		std::int32_t a_year,
		std::uint32_t a_month,
		std::uint32_t a_day)
	{
		const auto yearHolder = std::chrono::year(a_year);
		const auto monthHolder = std::chrono::month(a_month);
		const auto dayHolder = std::chrono::day(a_day);
		const auto weekDay = RE::Calendar::GetDayOfWeek(yearHolder, monthHolder, dayHolder);

		return weekDay.c_encoding();
	}

	static std::uint32_t GetDayOfWeek_NoLeapYear(RE::BSScript::StaticTag /*a_staticTag*/,
		std::uint32_t a_month,
		std::uint32_t a_day)
	{
		const auto monthHolder = std::chrono::month(a_month);
		const auto dayHolder = std::chrono::day(a_day);
		const auto weekDay = RE::Calendar::GetDayOfWeek_NoLeapYear(monthHolder, dayHolder);

		return weekDay.c_encoding();
	}

	static DateTimeStruct GetDefaultGameDateTime(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto year = static_cast<std::int32_t>(RE::Calendar::DEFAULT_GAME_YEAR);
		const auto month = static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_MONTH);
		const auto day = static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_DAY);
		const auto hour = static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_HOUR.count());
		const auto minute = static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_MINUTE.count());
		const auto second = static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_SECOND.count());
		const auto millisecond = static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_MILLISECOND.count());

		return {
			.year = year,
			.month = month,
			.day = day,
			.hour = hour,
			.minute = minute,
			.second = second,
			.millisecond = millisecond
		};
	}

	static std::optional<DateTimeStruct> GetGameDateTime(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto year = static_cast<std::int32_t>(calendar->GetYear());
		const auto month = static_cast<std::uint32_t>(calendar->GetMonth());
		const auto day = static_cast<std::uint32_t>(calendar->GetDay());
		const auto hour = static_cast<std::uint32_t>(calendar->GetHour().count());
		const auto minute = static_cast<std::uint32_t>(calendar->GetMinute().count());
		const auto second = static_cast<std::uint32_t>(calendar->GetSecond().count());
		const auto millisecond = static_cast<std::uint32_t>(calendar->GetMillisecond().count());

		return DateTimeStruct{
			.year = static_cast<std::uint32_t>(year),
			.month = month,
			.day = day,
			.hour = hour,
			.minute = minute,
			.second = second,
			.millisecond = millisecond
		};
	}

	static std::uint32_t GetDefaultGameMillisecond(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_MILLISECOND.count());
	}

	static std::uint32_t GetDefaultGameSecond(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_SECOND.count());
	}

	static std::uint32_t GetDefaultGameMinute(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_MINUTE.count());
	}

	static std::uint32_t GetDefaultGameHour(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_HOUR.count());
	}

	static std::uint32_t GetDefaultGameDay(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_DAY);
	}

	static std::uint32_t GetDefaultGameMonth(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_MONTH);
	}

	static std::int32_t GetDefaultGameYear(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return static_cast<std::int32_t>(RE::Calendar::DEFAULT_GAME_YEAR);
	}

	static std::uint32_t GetGameMillisecond(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_SECOND.count());
		}

		const auto millisecond = calendar->GetMillisecond();
		return static_cast<std::uint32_t>(millisecond.count());
	}

	static std::uint32_t GetGameSecond(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_SECOND.count());
		}

		const auto second = calendar->GetSecond();
		return static_cast<std::uint32_t>(second.count());
	}

	static std::uint32_t GetGameMinute(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_MINUTE.count());
		}

		const auto minute = calendar->GetMinute();
		return static_cast<std::uint32_t>(minute.count());
	}

	static std::uint32_t GetGameHour(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_HOUR.count());
		}

		const auto hour = calendar->GetHour();
		return static_cast<std::uint32_t>(hour.count());
	}

	static std::uint32_t GetGameDay(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_DAY);
		}

		const auto day = calendar->GetDay();
		return static_cast<std::uint32_t>(day);
	}

	static std::uint32_t GetGameMonth(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return static_cast<std::uint32_t>(RE::Calendar::DEFAULT_GAME_MONTH);
		}

		const auto month = calendar->GetMonth();
		return static_cast<std::uint32_t>(month);
	}

	static std::int32_t GetGameYear(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return static_cast<std::int32_t>(RE::Calendar::DEFAULT_GAME_YEAR);
		}

		const auto year = calendar->GetYear();
		return static_cast<std::int32_t>(year);
	}

	static std::uint32_t GetGameWeekDay(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return RE::Calendar::DEFAULT_GAME_WEEKDAY.c_encoding();
		}

		const auto weekday = calendar->GetWeekDay();
		return weekday.c_encoding();
	}

	static REX::Float32 GetElapsedGameHours(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return RE::Calendar::DEFAULT_GAME_HOURS_PASSED.count();
		}

		const auto elapsedHours = calendar->GetHoursPassed();
		return elapsedHours.count();
	}

	static REX::Float32 GetElapsedGameDays(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		const auto* calendar = RE::Calendar::GetSingleton();
		if (!calendar) [[unlikely]] {
			REX::Assert(false);
			return RE::Calendar::DEFAULT_GAME_DAYS_PASSED.count();
		}

		const auto elapsedDays = calendar->GetDaysPassed();
		return elapsedDays.count();
	}

	static std::vector<std::uint32_t> GetAllDays(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return { Impl::NUMERIC_DAYS.begin(), Impl::NUMERIC_DAYS.end() };
	}

	static std::vector<std::uint32_t> GetAllMonths(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return { Impl::NUMERIC_MONTHS.begin(), Impl::NUMERIC_MONTHS.end() };
	}

	static std::uint32_t GetDay_Sunday(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return std::chrono::Sunday.c_encoding(); }
	static std::uint32_t GetDay_Monday(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return std::chrono::Monday.c_encoding(); }
	static std::uint32_t GetDay_Tuesday(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return std::chrono::Tuesday.c_encoding(); }
	static std::uint32_t GetDay_Wednesday(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return std::chrono::Wednesday.c_encoding(); }
	static std::uint32_t GetDay_Thursday(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return std::chrono::Thursday.c_encoding(); }
	static std::uint32_t GetDay_Friday(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return std::chrono::Friday.c_encoding(); }
	static std::uint32_t GetDay_Saturday(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return std::chrono::Saturday.c_encoding(); }

	static std::uint32_t GetMonth_January(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::January); }
	static std::uint32_t GetMonth_February(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::February); }
	static std::uint32_t GetMonth_March(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::March); }
	static std::uint32_t GetMonth_April(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::April); }
	static std::uint32_t GetMonth_May(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::May); }
	static std::uint32_t GetMonth_June(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::June); }
	static std::uint32_t GetMonth_July(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::July); }
	static std::uint32_t GetMonth_August(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::August); }
	static std::uint32_t GetMonth_September(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::September); }
	static std::uint32_t GetMonth_October(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::October); }
	static std::uint32_t GetMonth_November(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::November); }
	static std::uint32_t GetMonth_December(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return static_cast<std::uint32_t>(std::chrono::December); }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDeltaTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetElapsedTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetTimeMultiplier);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultTimeScale);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetTimeScale);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDayOfWeek);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDayOfWeek_NoLeapYear);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultGameDateTime);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameDateTime);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultGameMillisecond);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultGameSecond);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultGameMinute);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultGameHour);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultGameDay);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultGameMonth);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDefaultGameYear);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameMillisecond);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameSecond);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameMinute);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameHour);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameDay);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameMonth);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameYear);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetGameWeekDay);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetElapsedGameHours);
		RE_REGISTER_VM_FUNCTION(a_vm, SCRIPT_NAME, GetElapsedGameDays);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAllDays);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAllMonths);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDay_Sunday);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDay_Monday);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDay_Tuesday);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDay_Wednesday);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDay_Thursday);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDay_Friday);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetDay_Saturday);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_January);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_February);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_March);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_April);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_May);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_June);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_July);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_August);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_September);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_October);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_November);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetMonth_December);
	}
}
