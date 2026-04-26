#pragma once

namespace Plugin::Internal::Script
{
	class ScriptStrictString;

	class ScriptStrictStringPool final
	{
	public:
		template <REX::win32_character T>
		inline static constexpr auto KEY_PREFIX = static_cast<T>('@');

		ScriptStrictStringPool() = default;
		~ScriptStrictStringPool() noexcept = default;

		ScriptStrictStringPool(const ScriptStrictStringPool&) = delete;
		ScriptStrictStringPool(ScriptStrictStringPool&&) = delete;

		ScriptStrictStringPool& operator=(const ScriptStrictStringPool&) = delete;
		ScriptStrictStringPool& operator=(ScriptStrictStringPool&&) = delete;

		[[nodiscard]] bool Contains(const RE::BSFixedString& a_key) const noexcept;

		[[nodiscard]] auto GetValue(const RE::BSFixedString& a_key) const noexcept -> std::optional<RE::BSFixedStringCS>;
		void SetValue(const RE::BSFixedString& a_key, const RE::BSFixedStringCS& a_value);

		bool AddValue(const RE::BSFixedString& a_key, const RE::BSFixedStringCS& a_value);
		bool Remove(const RE::BSFixedString& a_key) noexcept;

		void Clear() noexcept;

		template <REX::win32_character T>
		[[nodiscard]] static constexpr auto IsKeyValid(std::basic_string_view<T> a_key) noexcept
		{
			return a_key.starts_with(KEY_PREFIX<T>);
		}

	private:
		mutable std::shared_mutex _mapMutex;
		std::unordered_map<RE::BSFixedString, RE::BSFixedStringCS> _stringMap;
	};

	[[nodiscard]] auto GetScriptStrictStringPool() noexcept -> const REX::NotNull<std::unique_ptr<ScriptStrictStringPool>>&;
}
