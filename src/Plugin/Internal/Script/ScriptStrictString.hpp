#pragma once

namespace Plugin::Internal::Script
{
	class ScriptStrictString final
	{
	public:
		using key_type = RE::BSFixedString;
		using value_type = RE::BSFixedStringCS;
		using traits_type = value_type::traits_type;
		using size_type = value_type::size_type;
		using difference_type = value_type::difference_type;
		using pointer = value_type::pointer;
		using const_pointer = value_type::const_pointer;
		using reference = value_type::reference;
		using const_reference = value_type::const_reference;
		using iterator = value_type::iterator;
		using const_iterator = value_type::const_iterator;
		using reverse_iterator = value_type::reverse_iterator;
		using const_reverse_iterator = value_type::const_reverse_iterator;

		ScriptStrictString() = default;
		ScriptStrictString(key_type a_key, value_type a_value);

		~ScriptStrictString() noexcept = default;

		ScriptStrictString(const ScriptStrictString&) = default;
		ScriptStrictString(ScriptStrictString&&) noexcept = default;

		ScriptStrictString& operator=(const ScriptStrictString&) = default;
		ScriptStrictString& operator=(ScriptStrictString&&) noexcept = default;

		[[nodiscard]] bool operator==(const ScriptStrictString&) const noexcept = default;
		[[nodiscard]] bool operator!=(const ScriptStrictString&) const noexcept = default;

		[[nodiscard]] explicit operator std::string_view() const noexcept { return _value; }

		[[nodiscard]] const_reference operator[](size_type a_index) const noexcept { return _value[a_index]; }

		[[nodiscard]] const_reference at(size_type a_index) const noexcept { return _value.at(a_index); }

		[[nodiscard]] const_reference front() const noexcept { return _value.front(); }
		[[nodiscard]] const_reference back() const noexcept { return _value.back(); }

		[[nodiscard]] const_pointer data() const noexcept { return _value.data(); }
		[[nodiscard]] const_pointer c_str() const noexcept { return _value.c_str(); }

		[[nodiscard]] const_iterator begin() const noexcept { return _value.begin(); }
		[[nodiscard]] const_iterator cbegin() const noexcept { return _value.cbegin(); }
		[[nodiscard]] const_iterator end() const noexcept { return _value.end(); }
		[[nodiscard]] const_iterator cend() const noexcept { return _value.cend(); }

		[[nodiscard]] const_reverse_iterator rbegin() const noexcept { return _value.rbegin(); }
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept { return _value.crbegin(); }
		[[nodiscard]] const_reverse_iterator rend() const noexcept { return _value.rend(); }
		[[nodiscard]] const_reverse_iterator crend() const noexcept { return _value.crend(); }

		[[nodiscard]] size_type size() const noexcept { return _value.size(); }
		[[nodiscard]] size_type length() const noexcept { return _value.length(); }

		[[nodiscard]] bool empty() const noexcept { return _value.empty(); }

		[[nodiscard]] auto GetKey() const noexcept -> const key_type& { return _key; }
		[[nodiscard]] auto GetValue() const noexcept -> const value_type& { return _value; }

		void SetValue(const value_type& a_value) noexcept { _value = a_value; }

	private:
		key_type _key;
		value_type _value;
	};
}

namespace RE::BSScript
{
	template <class>
	struct variable_packer;

	template <>
	struct variable_packer<::Plugin::Internal::Script::ScriptStrictString>
	{
	public:
		[[nodiscard]] static auto GetTypeInfo() -> std::optional<TypeInfo>;

		static void PackVariable(Variable& a_var, const ::Plugin::Internal::Script::ScriptStrictString& a_val);

		[[nodiscard]] static auto UnpackVariable(const Variable& a_var) -> ::Plugin::Internal::Script::ScriptStrictString;
	};

	static_assert(RE::BSScript::ValidateParameter<::Plugin::Internal::Script::ScriptStrictString>());
}
