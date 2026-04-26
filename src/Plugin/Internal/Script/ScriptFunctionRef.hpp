#pragma once

#include "Plugin/Internal/Structs/FunctionRefs.hpp"

namespace Plugin::Internal::Save
{
	class SaveStream;
}

namespace Plugin::Internal::Script
{
	class ScriptFunctionRef final
	{
	public:
		enum class LoadError : std::int32_t
		{
			Failure = 0,
			InvalidHandle
		};

		ScriptFunctionRef() = default;

		explicit ScriptFunctionRef(
			RE::BSScript::ObjectHandle a_objectHandle,
			RE::BSFixedString a_scriptName,
			RE::BSFixedString a_functionName);

		explicit ScriptFunctionRef(
			RE::BSFixedString a_scriptName,
			RE::BSFixedString a_functionName);

		explicit ScriptFunctionRef(const Structs::FunctionRefs::FunctionRefData& a_functionRef);

		explicit ScriptFunctionRef(const RE::BSScript::StackFrame& a_stackFrame);

		~ScriptFunctionRef() noexcept = default;

		ScriptFunctionRef(const ScriptFunctionRef&) = default;
		ScriptFunctionRef(ScriptFunctionRef&&) noexcept = default;

		ScriptFunctionRef& operator=(const ScriptFunctionRef&) = default;
		ScriptFunctionRef& operator=(ScriptFunctionRef&&) noexcept = default;

		[[nodiscard]] bool operator==(const ScriptFunctionRef& a_rhs) const noexcept;
		[[nodiscard]] bool operator!=(const ScriptFunctionRef& a_rhs) const noexcept;

		[[nodiscard]] static bool IsVMFrozen();

		[[nodiscard]] bool ValidateRefMembers() const noexcept;
		[[nodiscard]] bool IsMemberRef() const noexcept;
		[[nodiscard]] bool IsStaticRef() const noexcept;

		[[nodiscard]] bool IsFunctionExistent() const;
		[[nodiscard]] bool IsFunctionOfNoParams() const;
		[[nodiscard]] bool IsFunctionOfParamTypes(std::span<const RE::BSScript::TypeInfo> a_args) const;
		[[nodiscard]] bool IsFunctionOfParamTypes(std::span<const RE::BSScript::Variable> a_args) const;
		[[nodiscard]] bool IsFunctionOfParamTypes(std::span<const RE::BSScript::SharedVariant> a_args) const;
		[[nodiscard]] bool IsFunctionOfParamTypes(std::span<const RE::BSScript::UniqueVariant> a_args) const;
		[[nodiscard]] bool IsFunctionOfParamTypes(const RE::BSTSmartPointer<RE::BSScript::Array>& a_args) const;
		[[nodiscard]] bool IsFunctionOfParamTypes(const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args) const;

		[[nodiscard]] auto GetObjectHandle() const noexcept { return _objectHandle; }
		[[nodiscard]] const auto& GetScriptName() const noexcept { return _scriptName; }
		[[nodiscard]] const auto& GetFunctionName() const noexcept { return _functionName; }

		[[nodiscard]] auto GetObject() const -> RE::BSTSmartPointer<RE::BSScript::Object>;
		[[nodiscard]] auto GetObjectTypeInfo() const -> RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>;

		template <class... Args>
		bool InvokeFunction(Args&&... a_args) const
			requires((RE::BSScript::ValidateParameter<std::remove_reference_t<Args>>() && ...));

		template <class... Args>
		bool InvokeFunctionWithCallback(const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor, Args&&... a_args) const
			requires((RE::BSScript::ValidateParameter<std::remove_reference_t<Args>>() && ...));

		bool InvokeAppliedFunction() const;
		bool InvokeAppliedFunctionWithCallback(const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor) const;

		bool InvokeAppliedFunction(std::span<const RE::BSScript::Variable> a_args) const;
		bool InvokeAppliedFunctionWithCallback(std::span<const RE::BSScript::Variable> a_args,
			const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor) const;

		bool InvokeAppliedFunction(const RE::BSTSmartPointer<RE::BSScript::Array>& a_args) const;
		bool InvokeAppliedFunctionWithCallback(const RE::BSTSmartPointer<RE::BSScript::Array>& a_args,
			const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor) const;

		bool InvokeAppliedFunction(const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args) const;
		bool InvokeAppliedFunctionWithCallback(const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args,
			const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor) const;

		static auto LoadStream(const Save::SaveStream& a_stream) -> std::expected<ScriptFunctionRef, LoadError>;
		bool SaveStream(Save::SaveStream& a_stream) const;

		[[nodiscard]] std::string ToString() const;

		void reset() noexcept;

	private:
		RE::BSScript::ObjectHandle _objectHandle{ RE::GameScript::HandlePolicy::EMPTY_HANDLE };
		RE::BSFixedString _scriptName;
		RE::BSFixedString _functionName;
	};
}

#include "Plugin/Internal/Script/ScriptFunctionRef.inl"

namespace std
{
	template <>
	struct hash<Plugin::Internal::Script::ScriptFunctionRef>
	{
	public:
		[[nodiscard]] std::size_t operator()(const Plugin::Internal::Script::ScriptFunctionRef& a_key) const noexcept;
	};
}

#if __cpp_lib_format > 0l
namespace std
{
	template <>
	struct formatter<Plugin::Internal::Script::ScriptFunctionRef>
		: public formatter<std::string>
	{
	public:
		template <class ParseContext>
		[[nodiscard]] constexpr auto parse(ParseContext& a_ctx) const
		{
			return a_ctx.begin();
		}

		template <class FormatContext>
		[[nodiscard]] constexpr auto format(const Plugin::Internal::Script::ScriptFunctionRef& a_value, FormatContext& a_ctx) const
		{
			using namespace std::string_view_literals;

			return format_to(a_ctx.out(), "{}"sv, a_value.ToString());
		}
	};
}
#endif

#if FMT_VERSION > 0l
namespace fmt
{
	template <>
	struct formatter<Plugin::Internal::Script::ScriptFunctionRef>
		: public formatter<std::string>
	{
	public:
		template <class ParseContext>
		[[nodiscard]] constexpr auto parse(ParseContext& a_ctx) const
		{
			return a_ctx.begin();
		}

		template <class FormatContext>
		[[nodiscard]] constexpr auto format(const Plugin::Internal::Script::ScriptFunctionRef& a_value, FormatContext& a_ctx) const
		{
			using namespace std::string_view_literals;

			return format_to(a_ctx.out(), "{}"sv, a_value.ToString());
		}
	};
}
#endif
