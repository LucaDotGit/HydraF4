#include "Plugin/Papyrus/Operator.hpp"

namespace Plugin::Papyrus::Operator
{
	using RawType = RE::BSScript::TypeInfo::RawType;

	static RE::BSScript::UniqueVariant Copy(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		if (!a_var) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_var->Clone(a_vm));
	}

	static RE::BSScript::UniqueVariant DeepCopy(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID /*a_stackId*/, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		if (!a_var) {
			return nullptr;
		}

		return RE::BSScript::CreateVariant(a_var->DeepClone(a_vm));
	}

	static bool IsValueType(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		return a_var && !a_var->IsComplex();
	}

	static bool IsRefType(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		return a_var && a_var->IsComplex();
	}

	static bool IsNone(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		return !a_var || a_var->IsNone();
	}

	static bool IsStruct(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		return a_var && a_var->IsStruct();
	}

	static bool IsArray(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		return a_var && a_var->IsArray();
	}

	static bool IsVar(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		return a_var && a_var->IsVar();
	}

	static std::int32_t Compare(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		const auto comparison = RE::BSScript::UnwrapVariant(a_left).Compare(RE::BSScript::UnwrapVariant(a_right));
		return REX::GetNumericPartialOrdering(comparison);
	}

	static std::int32_t DeepCompare(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		const auto comparison = RE::BSScript::UnwrapVariant(a_left).DeepCompare(RE::BSScript::UnwrapVariant(a_right));
		return REX::GetNumericPartialOrdering(comparison);
	}

	static bool Equals(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		return RE::BSScript::UnwrapVariant(a_left).Equals(RE::BSScript::UnwrapVariant(a_right));
	}

	static bool DeepEquals(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		return RE::BSScript::UnwrapVariant(a_left).DeepEquals(RE::BSScript::UnwrapVariant(a_right));
	}

	static bool RefEquals(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		return RE::BSScript::UnwrapVariant(a_left).RefEquals(RE::BSScript::UnwrapVariant(a_right));
	}

	static bool TypeEquals(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_left,
		RE::BSScript::SharedVariant a_right)
	{
		return RE::BSScript::UnwrapVariant(a_left).GetTypeInfo().Equals(RE::BSScript::UnwrapVariant(a_right).GetTypeInfo());
	}

	static RawType GetRawType(RE::BSScript::StaticTag /*a_staticTag*/,
		RE::BSScript::SharedVariant a_var)
	{
		return RE::BSScript::UnwrapVariant(a_var).GetDeepRawType();
	}

	static RawType GetRawType_None(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kNone; }
	static RawType GetRawType_Bool(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kBool; }
	static RawType GetRawType_Int(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kInt; }
	static RawType GetRawType_Float(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kFloat; }
	static RawType GetRawType_String(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kString; }
	static RawType GetRawType_Object(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kObject; }
	static RawType GetRawType_Struct(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kStruct; }
	static RawType GetRawType_Var(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kVar; }
	static RawType GetRawType_ArrayBool(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kArrayBool; }
	static RawType GetRawType_ArrayInt(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kArrayInt; }
	static RawType GetRawType_ArrayFloat(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kArrayFloat; }
	static RawType GetRawType_ArrayString(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kArrayString; }
	static RawType GetRawType_ArrayObject(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kArrayObject; }
	static RawType GetRawType_ArrayStruct(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kArrayStruct; }
	static RawType GetRawType_ArrayVar(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return RawType::kArrayVar; }

	static RE::BSScript::UniqueVariant Ternary(RE::BSScript::StaticTag /*a_staticTag*/,
		bool a_condition,
		RE::BSScript::SharedVariant a_trueValue,
		RE::BSScript::SharedVariant a_falseValue)
	{
		return a_condition ?
				   RE::BSScript::CreateVariant(RE::BSScript::UnwrapVariant(a_trueValue)) :
				   RE::BSScript::CreateVariant(RE::BSScript::UnwrapVariant(a_falseValue));
	}

	static void DoNothing(RE::BSScript::StaticTag /*a_staticTag*/)
	{
		return;
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Copy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepCopy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsValueType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsRefType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsNone);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsStruct);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsArray);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, IsVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Compare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepCompare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Equals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DeepEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, RefEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, TypeEquals);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_None);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_Bool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_Int);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_Float);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_String);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_Object);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_Struct);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_Var);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_ArrayBool);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_ArrayInt);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_ArrayFloat);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_ArrayString);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_ArrayObject);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_ArrayStruct);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRawType_ArrayVar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, Ternary);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, DoNothing);
	}
}
