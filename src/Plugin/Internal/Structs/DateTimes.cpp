#include "Plugin/Internal/Structs/DateTimes.hpp"

namespace RE::BSScript
{
	static constexpr auto YEAR_KEY = "iYear"sv;
	static constexpr auto MONTH_KEY = "iMonth"sv;
	static constexpr auto DAY_KEY = "iDay"sv;
	static constexpr auto HOUR_KEY = "iHour"sv;
	static constexpr auto MINUTE_KEY = "iMinute"sv;
	static constexpr auto SECOND_KEY = "iSecond"sv;
	static constexpr auto MILLISECOND_KEY = "iMillisecond"sv;
	static constexpr auto MICROSECOND_KEY = "iMicrosecond"sv;
	static constexpr auto NANOSECOND_KEY = "iNanosecond"sv;

	auto variable_packer<::Plugin::Internal::Structs::DateTimes::DateTimeData>::GetTypeInfo() -> std::optional<TypeInfo>
	{
		return RE::BSScript::GetTypeInfo<::Plugin::Internal::Structs::DateTimes::DateTime>();
	}

	void variable_packer<::Plugin::Internal::Structs::DateTimes::DateTimeData>::PackVariable(Variable& a_var, const ::Plugin::Internal::Structs::DateTimes::DateTimeData& a_val)
	{
		auto dateTimeStruct = ::Plugin::Internal::Structs::DateTimes::DateTime::Create();
		REX::Assert(static_cast<bool>(dateTimeStruct));

		const auto structLock = dateTimeStruct.Lock();
		dateTimeStruct.Insert_NoLock(YEAR_KEY, a_val.year);
		dateTimeStruct.Insert_NoLock(MONTH_KEY, a_val.month);
		dateTimeStruct.Insert_NoLock(DAY_KEY, a_val.day);
		dateTimeStruct.Insert_NoLock(HOUR_KEY, a_val.hour);
		dateTimeStruct.Insert_NoLock(MINUTE_KEY, a_val.minute);
		dateTimeStruct.Insert_NoLock(SECOND_KEY, a_val.second);
		dateTimeStruct.Insert_NoLock(MILLISECOND_KEY, a_val.millisecond);
		dateTimeStruct.Insert_NoLock(MICROSECOND_KEY, a_val.microsecond);
		dateTimeStruct.Insert_NoLock(NANOSECOND_KEY, a_val.nanosecond);

		RE::BSScript::PackVariable(a_var, std::move(dateTimeStruct));
	}

	auto variable_packer<::Plugin::Internal::Structs::DateTimes::DateTimeData>::UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Structs::DateTimes::DateTimeData
	{
		auto dateTimeStruct = RE::BSScript::Impl::UnpackVariable<::Plugin::Internal::Structs::DateTimes::DateTime>(a_var);
		if (!dateTimeStruct) {
			return {};
		}

		const auto structLock = dateTimeStruct.Lock();

		const auto year = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(YEAR_KEY);
		const auto month = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(MONTH_KEY);
		const auto day = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(DAY_KEY);
		const auto hour = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(HOUR_KEY);
		const auto minute = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(MINUTE_KEY);
		const auto second = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(SECOND_KEY);
		const auto millisecond = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(MILLISECOND_KEY);
		const auto microsecond = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(MICROSECOND_KEY);
		const auto nanosecond = dateTimeStruct.FindOrDefault_NoLock<std::uint32_t>(NANOSECOND_KEY);

		return {
			.year = year,
			.month = month,
			.day = day,
			.hour = hour,
			.minute = minute,
			.second = second,
			.millisecond = millisecond,
			.microsecond = microsecond,
			.nanosecond = nanosecond
		};
	}
}
