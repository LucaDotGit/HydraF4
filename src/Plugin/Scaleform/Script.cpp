#include "Plugin/Scaleform/Script.hpp"

#include "Plugin/Internal/Core/ScaleformUtility.hpp"

namespace Plugin::Scaleform::Script::Impl
{
	class ScriptFunctionCallback final
		: public RE::BSScript::IStackCallbackFunctor
	{
	public:
		explicit ScriptFunctionCallback(::Scaleform::GFx::Value a_function)
			: _function(std::move(a_function))
		{
			REX::Assert(_function.IsFunction());
		}

		~ScriptFunctionCallback() override = default;

		ScriptFunctionCallback(const ScriptFunctionCallback&) = delete;
		ScriptFunctionCallback(ScriptFunctionCallback&&) = delete;

		ScriptFunctionCallback& operator=(const ScriptFunctionCallback&) = delete;
		ScriptFunctionCallback& operator=(ScriptFunctionCallback&&) = delete;

		void CallQueued() override
		{
			return;
		}

		void CallCanceled() override
		{
			if (!_function.IsFunction()) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			auto gfxArgs = std::array{
				::Scaleform::GFx::NULL_VALUE
			};

			if (!_function.InvokeFunction(std::addressof(_function), gfxArgs)) [[unlikely]] {
				REX::Assert(false);
				return;
			}
		}

		void StartMultiDispatch() override
		{
			return;
		}

		void EndMultiDispatch() override
		{
			return;
		}

		void operator()(RE::BSScript::Variable a_returnValue) override
		{
			if (!_function.IsFunction()) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			auto* movie = _function.GetMovie();
			if (!movie) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			auto movieRoot = movie->asMovieRoot;
			if (!movieRoot) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			auto gfxFuncResult = ::Plugin::Internal::Core::ScaleformUtility::FromValue(a_returnValue, movieRoot);
			if (!gfxFuncResult.has_value()) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			auto gfxArgs = std::array{
				std::move(gfxFuncResult).value()
			};

			if (!_function.InvokeFunction(std::addressof(_function), gfxArgs)) [[unlikely]] {
				REX::Assert(false);
				return;
			}
		}

		bool CanSave() override
		{
			return false;
		}

	private:
		::Scaleform::GFx::Value _function;
	};

	[[nodiscard]] static auto ToMemberFunctionArgs(
		const RE::BSTSmartPointer<RE::BSScript::Object>& a_scriptObject,
		const RE::BSFixedString& a_functionName,
		const ::Scaleform::GFx::Value& a_gfxArgs) -> std::optional<RE::BSScrapArray<RE::BSScript::Variable>>
	{
		if (!a_scriptObject) {
			return std::nullopt;
		}

		if (!a_gfxArgs.IsArray()) {
			return std::nullopt;
		}

		const auto function = RE::BSScript::IVirtualMachine::GetMemberFunction(a_scriptObject, a_functionName);
		if (!function) {
			return std::nullopt;
		}

		const auto paramCount = function->GetParamCount();
		if (paramCount != a_gfxArgs.GetArraySize()) {
			return std::nullopt;
		}

		auto scriptArgs = RE::BSScrapArray<RE::BSScript::Variable>();

		function->ForEachParam([&a_gfxArgs, &scriptArgs](std::uint32_t a_index, [[maybe_unused]] const RE::BSFixedString& a_paramName, const RE::BSScript::TypeInfo& a_paramTypeInfo) {
			auto gfxElement = ::Scaleform::GFx::Value();
			if (!a_gfxArgs.GetElement(a_index, std::addressof(gfxElement))) [[unlikely]] {
				REX::Assert(false);
				return RE::BSContainer::ForEachResult::kStop;
			}

			auto scriptValue = ::Plugin::Internal::Core::ScaleformUtility::ToValue(gfxElement, a_paramTypeInfo);
			if (!scriptValue.has_value()) {
				return RE::BSContainer::ForEachResult::kStop;
			}

			scriptArgs.push_back(std::move(scriptValue).value());
			return RE::BSContainer::ForEachResult::kContinue;
		});

		return scriptArgs;
	}

	[[nodiscard]] static auto ToStaticFunctionArgs(
		const RE::BSFixedString& a_scriptName,
		const RE::BSFixedString& a_functionName,
		const ::Scaleform::GFx::Value& a_gfxArgs) -> std::optional<RE::BSScrapArray<RE::BSScript::Variable>>
	{
		if (!a_gfxArgs.IsArray()) {
			return std::nullopt;
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return std::nullopt;
		}

		const auto function = scriptVM->GetStaticFunction(a_scriptName, a_functionName);
		if (!function) {
			return std::nullopt;
		}

		const auto paramCount = function->GetParamCount();
		if (paramCount != a_gfxArgs.GetArraySize()) {
			return std::nullopt;
		}

		auto scriptArgs = RE::BSScrapArray<RE::BSScript::Variable>();

		function->ForEachParam([&a_gfxArgs, &scriptArgs](std::uint32_t a_index, [[maybe_unused]] const RE::BSFixedString& a_paramName, const RE::BSScript::TypeInfo& a_paramTypeInfo) {
			auto gfxElement = ::Scaleform::GFx::Value();
			if (!a_gfxArgs.GetElement(a_index, std::addressof(gfxElement))) [[unlikely]] {
				REX::Assert(false);
				return RE::BSContainer::ForEachResult::kStop;
			}

			auto scriptValue = ::Plugin::Internal::Core::ScaleformUtility::ToValue(gfxElement, a_paramTypeInfo);
			if (!scriptValue.has_value()) {
				return RE::BSContainer::ForEachResult::kStop;
			}

			scriptArgs.push_back(std::move(scriptValue).value());
			return RE::BSContainer::ForEachResult::kContinue;
		});

		return scriptArgs;
	}

	[[nodiscard]] static auto ToScriptCallback(
		const ::Scaleform::GFx::Value& a_function) -> std::optional<RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>>
	{
		if (a_function.IsNullOrUndefined()) {
			return nullptr;
		}

		if (!a_function.IsFunction()) {
			return std::nullopt;
		}

		return RE::make_shared<ScriptFunctionCallback>(a_function);
	}
}

namespace Plugin::Scaleform::Script
{
	class getScriptVariable final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getScriptVariable() = default;
		~getScriptVariable() override = default;

		getScriptVariable(const getScriptVariable&) = delete;
		getScriptVariable(getScriptVariable&&) = delete;

		getScriptVariable& operator=(const getScriptVariable&) = delete;
		getScriptVariable& operator=(getScriptVariable&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 2) {
				paramResult->SetNull();
				return;
			}

			auto scriptObject = ::Plugin::Internal::Core::ScaleformUtility::ToObject(a_params.args[0]);
			if (!scriptObject.has_value() || !scriptObject.value()) {
				paramResult->SetNull();
				return;
			}

			auto variableName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[1]);
			if (!variableName.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto objectLock = RE::BSAutoLock(*(scriptObject.value()));

			const auto* scriptVariable = (*scriptObject)->GetVariable(variableName.value());
			if (!scriptVariable) {
				paramResult->SetNull();
				return;
			}

			auto gfxValue = ::Plugin::Internal::Core::ScaleformUtility::FromValue(*scriptVariable, a_params.GetMovieRoot());
			if (!gfxValue.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxValue).value());
		}
	};

	class setScriptVariable final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		setScriptVariable() = default;
		~setScriptVariable() override = default;

		setScriptVariable(const setScriptVariable&) = delete;
		setScriptVariable(setScriptVariable&&) = delete;

		setScriptVariable& operator=(const setScriptVariable&) = delete;
		setScriptVariable& operator=(setScriptVariable&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 3) {
				paramResult->SetBoolean(false);
				return;
			}

			auto scriptObject = ::Plugin::Internal::Core::ScaleformUtility::ToObject(a_params.args[0]);
			if (!scriptObject.has_value() || !scriptObject.value()) {
				paramResult->SetBoolean(false);
				return;
			}

			auto variableName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[1]);
			if (!variableName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto objectLock = RE::BSAutoLock(*(scriptObject.value()));

			auto* scriptVariable = (*scriptObject)->GetVariable(variableName.value());
			if (!scriptVariable) {
				paramResult->SetBoolean(false);
				return;
			}

			auto scriptValue = ::Plugin::Internal::Core::ScaleformUtility::ToValue(a_params.args[2], scriptVariable->GetTypeInfo());
			if (!scriptValue.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			if (!scriptVariable->TryAssign(std::move(scriptValue).value())) {
				paramResult->SetBoolean(false);
				return;
			}

			paramResult->SetBoolean(true);
		}
	};

	class getScriptAutoProperty final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getScriptAutoProperty() = default;
		~getScriptAutoProperty() override = default;

		getScriptAutoProperty(const getScriptAutoProperty&) = delete;
		getScriptAutoProperty(getScriptAutoProperty&&) = delete;

		getScriptAutoProperty& operator=(const getScriptAutoProperty&) = delete;
		getScriptAutoProperty& operator=(getScriptAutoProperty&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 2) {
				paramResult->SetNull();
				return;
			}

			auto scriptObject = ::Plugin::Internal::Core::ScaleformUtility::ToObject(a_params.args[0]);
			if (!scriptObject.has_value() || !scriptObject.value()) {
				paramResult->SetNull();
				return;
			}

			auto variableName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[1]);
			if (!variableName.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto objectLock = RE::BSAutoLock(*(scriptObject.value()));

			const auto* scriptProperty = (*scriptObject)->GetProperty(variableName.value());
			if (!scriptProperty) {
				paramResult->SetNull();
				return;
			}

			auto gfxValue = ::Plugin::Internal::Core::ScaleformUtility::FromValue(*scriptProperty, a_params.GetMovieRoot());
			if (!gfxValue.has_value()) {
				paramResult->SetNull();
				return;
			}

			paramResult->operator=(std::move(gfxValue).value());
		}
	};

	class setScriptAutoProperty final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		setScriptAutoProperty() = default;
		~setScriptAutoProperty() override = default;

		setScriptAutoProperty(const setScriptAutoProperty&) = delete;
		setScriptAutoProperty(setScriptAutoProperty&&) = delete;

		setScriptAutoProperty& operator=(const setScriptAutoProperty&) = delete;
		setScriptAutoProperty& operator=(setScriptAutoProperty&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 3) {
				paramResult->SetBoolean(false);
				return;
			}

			auto scriptObject = ::Plugin::Internal::Core::ScaleformUtility::ToObject(a_params.args[0]);
			if (!scriptObject.has_value() || !scriptObject.value()) {
				paramResult->SetBoolean(false);
				return;
			}

			auto variableName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[1]);
			if (!variableName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto objectLock = RE::BSAutoLock(*(scriptObject.value()));

			auto* scriptProperty = (*scriptObject)->GetProperty(variableName.value());
			if (!scriptProperty) {
				paramResult->SetBoolean(false);
				return;
			}

			auto scriptValue = ::Plugin::Internal::Core::ScaleformUtility::ToValue(a_params.args[2], scriptProperty->GetTypeInfo());
			if (!scriptValue.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			if (!scriptProperty->TryAssign(std::move(scriptValue).value())) {
				paramResult->SetBoolean(false);
				return;
			}

			paramResult->SetBoolean(true);
		}
	};

	class callScriptMethodAsync final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		callScriptMethodAsync() = default;
		~callScriptMethodAsync() override = default;

		callScriptMethodAsync(const callScriptMethodAsync&) = delete;
		callScriptMethodAsync(callScriptMethodAsync&&) = delete;

		callScriptMethodAsync& operator=(const callScriptMethodAsync&) = delete;
		callScriptMethodAsync& operator=(callScriptMethodAsync&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount < 2 || a_params.argCount > 4) {
				paramResult->SetBoolean(false);
				return;
			}

			auto scriptObject = ::Plugin::Internal::Core::ScaleformUtility::ToObject(a_params.args[0]);
			if (!scriptObject.has_value() || !scriptObject.value()) {
				paramResult->SetBoolean(false);
				return;
			}

			auto functionName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[1]);
			if (!functionName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			auto scriptArgs = Impl::ToMemberFunctionArgs(scriptObject.value(), functionName.value(), a_params.GetNthArgOrNull(2));
			if (!scriptArgs.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto scriptVM = RE::GameVM::GetVMInterface();
			if (!scriptVM) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			const auto scriptCallback = Impl::ToScriptCallback(a_params.GetNthArgOrNull(3));
			if (!scriptCallback.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto isDispatched = scriptVM->InvokeAppliedMemberFunction(
				scriptObject.value(), functionName.value(), scriptCallback.value(), std::move(scriptArgs).value());

			paramResult->SetBoolean(isDispatched);
		}
	};

	class callScriptFunctionAsync final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		callScriptFunctionAsync() = default;
		~callScriptFunctionAsync() override = default;

		callScriptFunctionAsync(const callScriptFunctionAsync&) = delete;
		callScriptFunctionAsync(callScriptFunctionAsync&&) = delete;

		callScriptFunctionAsync& operator=(const callScriptFunctionAsync&) = delete;
		callScriptFunctionAsync& operator=(callScriptFunctionAsync&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount < 2 || a_params.argCount > 4) {
				paramResult->SetBoolean(false);
				return;
			}

			auto scriptName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[0]);
			if (!scriptName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			auto functionName = ::Plugin::Internal::Core::ScaleformUtility::ToFixedString(a_params.args[1]);
			if (!functionName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			auto scriptArgs = Impl::ToStaticFunctionArgs(scriptName.value(), functionName.value(), a_params.GetNthArgOrNull(2));
			if (!scriptArgs.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto scriptVM = RE::GameVM::GetVMInterface();
			if (!scriptVM) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			const auto scriptCallback = Impl::ToScriptCallback(a_params.GetNthArgOrNull(3));
			if (!scriptCallback.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto isDispatched = scriptVM->InvokeAppliedStaticFunction(
				scriptName.value(), functionName.value(), scriptCallback.value(), std::move(scriptArgs).value());

			paramResult->SetBoolean(isDispatched);
		}
	};

	void RegisterFunctions(::Scaleform::GFx::Movie* a_movie, ::Scaleform::GFx::Value* a_value)
	{
		if (!a_movie) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		if (!a_value) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getScriptVariable);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, setScriptVariable);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getScriptAutoProperty);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, setScriptAutoProperty);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, callScriptMethodAsync);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, callScriptFunctionAsync);
	}
}
