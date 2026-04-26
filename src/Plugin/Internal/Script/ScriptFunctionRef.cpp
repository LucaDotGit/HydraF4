#include "Plugin/Internal/Script/ScriptFunctionRef.hpp"

#include "Plugin/Internal/Save/SaveStream.hpp"

namespace Plugin::Internal::Script
{
	ScriptFunctionRef::ScriptFunctionRef(
		RE::BSScript::ObjectHandle a_objectHandle,
		RE::BSFixedString a_scriptName,
		RE::BSFixedString a_functionName)
		: _objectHandle(a_objectHandle),
		  _scriptName(std::move(a_scriptName)),
		  _functionName(std::move(a_functionName))
	{
	}

	ScriptFunctionRef::ScriptFunctionRef(
		RE::BSFixedString a_scriptName,
		RE::BSFixedString a_functionName)
		: _scriptName(std::move(a_scriptName)),
		  _functionName(std::move(a_functionName))
	{
	}

	ScriptFunctionRef::ScriptFunctionRef(const Structs::FunctionRefs::FunctionRefData& a_functionRef)
		: _objectHandle(Structs::FunctionRefs::FunctionRefData::GetObjectHandle(a_functionRef)),
		  _scriptName(a_functionRef.scriptName),
		  _functionName(a_functionRef.functionName)
	{
	}

	ScriptFunctionRef::ScriptFunctionRef(const RE::BSScript::StackFrame& a_stackFrame)
		: _objectHandle(a_stackFrame.GetSelfObjectHandle()),
		  _scriptName(a_stackFrame.owningObjectType ? a_stackFrame.owningObjectType->GetName() : RE::BSFixedString()),
		  _functionName(a_stackFrame.owningFunction ? a_stackFrame.owningFunction->GetName() : RE::BSFixedString())
	{
	}

	bool ScriptFunctionRef::operator==(const ScriptFunctionRef& a_rhs) const noexcept
	{
		const auto isLeftMember = IsMemberRef();
		const auto isRightMember = a_rhs.IsMemberRef();

		if (isLeftMember != isRightMember) {
			return false;
		}

		if (isLeftMember) {
			return _objectHandle == a_rhs._objectHandle &&
				   _scriptName == a_rhs._scriptName &&
				   _functionName == a_rhs._functionName;
		}

		return _scriptName == a_rhs._scriptName &&
			   _functionName == a_rhs._functionName;
	}

	bool ScriptFunctionRef::operator!=(const ScriptFunctionRef& a_rhs) const noexcept
	{
		return !(*this == a_rhs);
	}

	bool ScriptFunctionRef::IsVMFrozen()
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return scriptVM->IsCompletelyFrozen();
	}

	bool ScriptFunctionRef::ValidateRefMembers() const noexcept
	{
		return !_scriptName.empty() && !_functionName.empty();
	}

	bool ScriptFunctionRef::IsMemberRef() const noexcept
	{
		return _objectHandle != RE::GameScript::HandlePolicy::EMPTY_HANDLE;
	}

	bool ScriptFunctionRef::IsStaticRef() const noexcept
	{
		return !IsMemberRef();
	}

	auto ScriptFunctionRef::GetObject() const -> RE::BSTSmartPointer<RE::BSScript::Object>
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		auto scriptObject = RE::BSTSmartPointer<RE::BSScript::Object>();
		if (!scriptVM->GetScriptObjectExact(_scriptName, _objectHandle, scriptObject)) {
			return nullptr;
		}

		return scriptObject;
	}

	auto ScriptFunctionRef::GetObjectTypeInfo() const -> RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return nullptr;
		}

		auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!scriptVM->GetScriptObjectType(_scriptName, objectTypeInfo) || !objectTypeInfo || !objectTypeInfo->IsLinked()) {
			return nullptr;
		}

		return objectTypeInfo;
	}

	bool ScriptFunctionRef::IsFunctionExistent() const
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (IsMemberRef()) {
			const auto memberFunc = scriptVM->GetMemberFunction(_scriptName, _functionName);
			return memberFunc != nullptr;
		}

		const auto staticFunc = scriptVM->GetStaticFunction(_scriptName, _functionName);
		return staticFunc != nullptr;
	}

	bool ScriptFunctionRef::IsFunctionOfNoParams() const
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (IsMemberRef()) {
			const auto memberFunc = scriptVM->GetMemberFunction(_scriptName, _functionName);
			return memberFunc && memberFunc->HasNoParams();
		}

		const auto staticFunc = scriptVM->GetStaticFunction(_scriptName, _functionName);
		return staticFunc && staticFunc->HasNoParams();
	}

	bool ScriptFunctionRef::IsFunctionOfParamTypes(std::span<const RE::BSScript::TypeInfo> a_args) const
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (IsMemberRef()) {
			const auto memberFunc = scriptVM->GetMemberFunction(_scriptName, _functionName);
			return memberFunc && memberFunc->HasParamsOfTypes(a_args);
		}

		const auto staticFunc = scriptVM->GetStaticFunction(_scriptName, _functionName);
		return staticFunc && staticFunc->HasParamsOfTypes(a_args);
	}

	bool ScriptFunctionRef::IsFunctionOfParamTypes(std::span<const RE::BSScript::Variable> a_args) const
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (IsMemberRef()) {
			const auto memberFunc = scriptVM->GetMemberFunction(_scriptName, _functionName);
			return memberFunc && memberFunc->HasParamsOfTypes(a_args);
		}

		const auto staticFunc = scriptVM->GetStaticFunction(_scriptName, _functionName);
		return staticFunc && staticFunc->HasParamsOfTypes(a_args);
	}

	bool ScriptFunctionRef::IsFunctionOfParamTypes(std::span<const RE::BSScript::SharedVariant> a_args) const
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (IsMemberRef()) {
			const auto memberFunc = scriptVM->GetMemberFunction(_scriptName, _functionName);
			return memberFunc && memberFunc->HasParamsOfVariadicTypes(a_args);
		}

		const auto staticFunc = scriptVM->GetStaticFunction(_scriptName, _functionName);
		return staticFunc && staticFunc->HasParamsOfVariadicTypes(a_args);
	}

	bool ScriptFunctionRef::IsFunctionOfParamTypes(std::span<const RE::BSScript::UniqueVariant> a_args) const
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (IsMemberRef()) {
			const auto memberFunc = scriptVM->GetMemberFunction(_scriptName, _functionName);
			return memberFunc && memberFunc->HasParamsOfVariadicTypes(a_args);
		}

		const auto staticFunc = scriptVM->GetStaticFunction(_scriptName, _functionName);
		return staticFunc && staticFunc->HasParamsOfVariadicTypes(a_args);
	}

	bool ScriptFunctionRef::IsFunctionOfParamTypes(const RE::BSTSmartPointer<RE::BSScript::Array>& a_args) const
	{
		if (!a_args) {
			return IsFunctionOfNoParams();
		}

		const auto arrayLock = RE::BSAutoLock(a_args->elementsLock);
		return IsFunctionOfParamTypes(*a_args);
	}

	bool ScriptFunctionRef::IsFunctionOfParamTypes(const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args) const
	{
		if (!a_args) {
			return IsFunctionOfNoParams();
		}

		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		const auto arrayLock = RE::BSAutoLock(a_args->elementsLock);

		if (IsMemberRef()) {
			const auto memberFunc = scriptVM->GetMemberFunction(_scriptName, _functionName);
			return memberFunc && memberFunc->HasParamsOfVariadicTypes(a_args);
		}

		const auto staticFunc = scriptVM->GetStaticFunction(_scriptName, _functionName);
		return staticFunc && staticFunc->HasParamsOfVariadicTypes(a_args);
	}

	bool ScriptFunctionRef::InvokeAppliedFunction() const
	{
		constexpr auto STACK_CALLBACK = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
		return InvokeAppliedFunctionWithCallback(STACK_CALLBACK);
	}

	bool ScriptFunctionRef::InvokeAppliedFunctionWithCallback(const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor) const
	{
		constexpr auto EMPTY_ARGS = std::span<const RE::BSScript::Variable>();
		return InvokeAppliedFunctionWithCallback(EMPTY_ARGS, a_callbackFunctor);
	}

	bool ScriptFunctionRef::InvokeAppliedFunction(std::span<const RE::BSScript::Variable> a_args) const
	{
		constexpr auto STACK_CALLBACK = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
		return InvokeAppliedFunctionWithCallback(a_args, STACK_CALLBACK);
	}

	bool ScriptFunctionRef::InvokeAppliedFunctionWithCallback(std::span<const RE::BSScript::Variable> a_args,
		const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor) const
	{
		const auto scriptVM = RE::GameVM::GetVMInterface();
		if (!scriptVM) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (IsMemberRef()) {
			return scriptVM->InvokeAppliedMemberFunction(GetObject(), _functionName, a_callbackFunctor, a_args);
		}

		return scriptVM->InvokeAppliedStaticFunction(_scriptName, _functionName, a_callbackFunctor, a_args);
	}

	bool ScriptFunctionRef::InvokeAppliedFunction(const RE::BSTSmartPointer<RE::BSScript::Array>& a_args) const
	{
		constexpr auto STACK_CALLBACK = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
		return InvokeAppliedFunctionWithCallback(a_args, STACK_CALLBACK);
	}

	bool ScriptFunctionRef::InvokeAppliedFunctionWithCallback(const RE::BSTSmartPointer<RE::BSScript::Array>& a_args,
		const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor) const
	{
		if (!a_args) {
			return InvokeAppliedFunctionWithCallback(a_callbackFunctor);
		}

		const auto arrayLock = RE::BSAutoLock(a_args->elementsLock);
		return InvokeAppliedFunctionWithCallback(*a_args, a_callbackFunctor);
	}

	bool ScriptFunctionRef::InvokeAppliedFunction(const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args) const
	{
		constexpr auto STACK_CALLBACK = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
		return InvokeAppliedFunctionWithCallback(a_args, STACK_CALLBACK);
	}

	bool ScriptFunctionRef::InvokeAppliedFunctionWithCallback(const RE::BSScript::ArrayWrapper<RE::BSScript::SharedVariant>& a_args,
		const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callbackFunctor) const
	{
		if (!a_args) {
			return InvokeAppliedFunctionWithCallback(a_callbackFunctor);
		}

		const auto arrayLock = RE::BSAutoLock(a_args->elementsLock);

		auto appliedArgs = std::vector<RE::BSScript::Variable>();
		appliedArgs.reserve(a_args.size());

		for (const auto& arg : a_args) {
			const auto* var = arg.GetVar();
			appliedArgs.push_back(RE::BSScript::UnwrapVariant(var));
		}

		return InvokeAppliedFunctionWithCallback(appliedArgs, a_callbackFunctor);
	}

	auto ScriptFunctionRef::LoadStream(const Save::SaveStream& a_stream) -> std::expected<ScriptFunctionRef, LoadError>
	{
		auto isMemberRef = false;
		if (!a_stream.ReadValue(isMemberRef)) [[unlikely]] {
			REX::Assert(false);
			return std::unexpected(LoadError::Failure);
		}

		if (isMemberRef) {
			auto objectHandle = RE::GameScript::HandlePolicy::EMPTY_HANDLE;
			if (!a_stream.ReadValue(objectHandle)) [[unlikely]] {
				REX::Assert(false);
				return std::unexpected(LoadError::Failure);
			}

			auto scriptName = RE::BSFixedString();
			if (!a_stream.ReadString(scriptName)) [[unlikely]] {
				REX::Assert(false);
				return std::unexpected(LoadError::Failure);
			}

			auto functionName = RE::BSFixedString();
			if (!a_stream.ReadString(functionName)) [[unlikely]] {
				REX::Assert(false);
				return std::unexpected(LoadError::Failure);
			}

			const auto resolvedObjectHandle = a_stream.ResolveObjectHandle(objectHandle);
			if (!resolvedObjectHandle.has_value()) {
				return std::unexpected(LoadError::InvalidHandle);
			}

			return ScriptFunctionRef(
				resolvedObjectHandle.value(),
				std::move(scriptName),
				std::move(functionName));
		}

		auto scriptName = RE::BSFixedString();
		if (!a_stream.ReadString(scriptName)) [[unlikely]] {
			REX::Assert(false);
			return std::unexpected(LoadError::Failure);
		}

		auto functionName = RE::BSFixedString();
		if (!a_stream.ReadString(functionName)) [[unlikely]] {
			REX::Assert(false);
			return std::unexpected(LoadError::Failure);
		}

		return ScriptFunctionRef(
			std::move(scriptName),
			std::move(functionName));
	}

	bool ScriptFunctionRef::SaveStream(Save::SaveStream& a_stream) const
	{
		const auto isMemberRef = IsMemberRef();
		if (!a_stream.WriteValue(isMemberRef)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (isMemberRef) {
			if (!a_stream.WriteValue(_objectHandle)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			if (!a_stream.WriteString(_scriptName)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			if (!a_stream.WriteString(_functionName)) [[unlikely]] {
				REX::Assert(false);
				return false;
			}

			return true;
		}

		if (!a_stream.WriteString(_scriptName)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		if (!a_stream.WriteString(_functionName)) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		return true;
	}

	std::string ScriptFunctionRef::ToString() const
	{
		if (IsMemberRef()) {
			return REX::Format("[{:X}->{}::{}]"sv,
				_objectHandle, _scriptName, _functionName);
		}

		return REX::Format("[{}::{}]"sv,
			_scriptName, _functionName);
	}

	void ScriptFunctionRef::reset() noexcept
	{
		_objectHandle = RE::GameScript::HandlePolicy::EMPTY_HANDLE;
		_scriptName.clear();
		_functionName.clear();
	}
}

namespace std
{
	size_t hash<Plugin::Internal::Script::ScriptFunctionRef>::operator()(const Plugin::Internal::Script::ScriptFunctionRef& a_key) const noexcept
	{
		if (a_key.IsMemberRef()) {
			return REX::HashCombine(a_key.GetObjectHandle(), a_key.GetScriptName(), a_key.GetFunctionName());
		}

		return REX::HashCombine(a_key.GetScriptName(), a_key.GetFunctionName());
	}
}
