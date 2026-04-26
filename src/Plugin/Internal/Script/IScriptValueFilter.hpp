#pragma once

namespace Plugin::Internal::Script
{
	template <class T>
	class IScriptValueFilter
	{
	public:
		using value_type = T;

		IScriptValueFilter() = default;

		virtual ~IScriptValueFilter() //
			noexcept(std::is_nothrow_destructible_v<T>) = default;

		IScriptValueFilter(const IScriptValueFilter&) = default;

		IScriptValueFilter(IScriptValueFilter&&) //
			noexcept(std::is_nothrow_move_constructible_v<T>) = default;

		IScriptValueFilter& operator=(const IScriptValueFilter&) = default;

		IScriptValueFilter& operator=(IScriptValueFilter&&) //
			noexcept(std::is_nothrow_move_assignable_v<T>) = default;

		[[nodiscard]] virtual bool IsMatch(const T& a_value) const = 0;
	};
}
