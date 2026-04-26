#include "Plugin/Papyrus/Forms/Light.hpp"

#include "Plugin/Internal/Script/ScriptErrors.hpp"
#include "Plugin/Internal/Structs/Colors.hpp"

namespace Plugin::Papyrus::Forms::Light
{
	using ColorStruct = ::Plugin::Internal::Structs::Colors::ColorData;

	using LightFlags = RE::OBJ_LIGH::Flags;
	using LightRecordFlags = RE::TESObjectLIGH::RecordFlags;

	static RE::BGSSoundDescriptorForm* GetSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_light->sound).load(std::memory_order_acquire);
	}

	static void SetSound(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		RE::BGSSoundDescriptorForm* a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->sound).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetFov(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.fov).load(std::memory_order_acquire);
	}

	static void SetFov(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.fov).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetFalloffExponent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.falloffExponent).load(std::memory_order_acquire);
	}

	static void SetFalloffExponent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.falloffExponent).store(a_value, std::memory_order_release);
	}

	static std::uint32_t GetRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0;
		}

		return std::atomic_ref(a_light->data.radius).load(std::memory_order_acquire);
	}

	static void SetRadius(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		std::uint32_t a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.radius).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetNearDistance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.nearDistance).load(std::memory_order_acquire);
	}

	static void SetNearDistance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.nearDistance).store(a_value, std::memory_order_release);
	}

	static std::optional<ColorStruct> GetColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return std::nullopt;
		}

		return ColorStruct::FromHex(std::atomic_ref(a_light->data.color).load(std::memory_order_acquire));
	}

	static void SetColor(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		std::optional<ColorStruct> a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		if (!a_value.has_value()) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::STRUCT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.color).store(a_value->ToHex(), std::memory_order_release);
	}

	static REX::Float32 GetColorFade(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->fade).load(std::memory_order_acquire);
	}

	static void SetColorFade(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->fade).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetFlickerPeriod(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.flickerPeriodRecip).load(std::memory_order_acquire);
	}

	static void SetFlickerPeriod(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.flickerPeriodRecip).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetFlickerIntensityAmplitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.flickerIntensityAmplitude).load(std::memory_order_acquire);
	}

	static void SetFlickerIntensityAmplitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.flickerIntensityAmplitude).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetFlickerMovementAmplitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.flickerMovementAmplitude).load(std::memory_order_acquire);
	}

	static void SetFlickerMovementAmplitude(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.flickerMovementAmplitude).store(a_value, std::memory_order_release);
	}

	static RE::BGSGodRays* GetGodRays(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_light->godRays).load(std::memory_order_acquire);
	}

	static void SetGodRays(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		RE::BGSGodRays* a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->godRays).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetGodRaysNearDistance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.godraysNearDistance).load(std::memory_order_acquire);
	}

	static void SetGodRaysNearDistance(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.godraysNearDistance).store(a_value, std::memory_order_release);
	}

	static RE::BGSLensFlare* GetLensFlare(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return nullptr;
		}

		return std::atomic_ref(a_light->lensFlare).load(std::memory_order_acquire);
	}

	static void SetLensFlare(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		RE::BGSLensFlare* a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->lensFlare).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetAttenuationConstant(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.attenuationConstant).load(std::memory_order_acquire);
	}

	static void SetAttenuationConstant(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.attenuationConstant).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetAttenuationScalar(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.attenuationScalar).load(std::memory_order_acquire);
	}

	static void SetAttenuationScalar(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.attenuationScalar).store(a_value, std::memory_order_release);
	}

	static REX::Float32 GetAttenuationExponent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return 0.0_f32;
		}

		return std::atomic_ref(a_light->data.attenuationExponent).load(std::memory_order_acquire);
	}

	static void SetAttenuationExponent(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		REX::Float32 a_value)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		std::atomic_ref(a_light->data.attenuationExponent).store(a_value, std::memory_order_release);
	}

	static bool HasFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		LightFlags a_flag)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return false;
		}

		return a_light->data.flags.any_atomic(a_flag);
	}

	static LightFlags GetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return LightFlags::kNone;
		}

		return a_light->data.flags.get_atomic();
	}

	static void SetFlags(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		LightFlags a_flags)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		a_light->data.flags.store_atomic(a_flags);
	}

	static void SetFlag(RE::BSScript::IVirtualMachine& a_vm, RE::BSScript::StackID a_stackId, RE::BSScript::StaticTag /*a_staticTag*/,
		RE::TESObjectLIGH* a_light,
		LightFlags a_flag,
		bool a_set)
	{
		if (!a_light) [[unlikely]] {
			a_vm.PostError(::Plugin::Internal::Script::ScriptErrors::LIGHT_NULL, a_stackId);
			return;
		}

		a_light->data.flags.set_atomic(a_set, a_flag);
	}

	static LightFlags GetFlag_IsDynamic(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kDynamic; }
	static LightFlags GetFlag_CanBeCarried(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kCanCarry; }
	static LightFlags GetFlag_IsNegative(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kNegative; }
	static LightFlags GetFlag_CanFlicker(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kFlicker; }
	static LightFlags GetFlag_IsDeepCopy(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kDeepCopy; }
	static LightFlags GetFlag_IsOffByDefault(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kOffByDefault; }
	static LightFlags GetFlag_FlickerSlowly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kFlickerSlow; }
	static LightFlags GetFlag_CanPulse(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kPulse; }
	static LightFlags GetFlag_PulseSlowly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kPulseSlow; }
	static LightFlags GetFlag_IsSpotlight(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kSpotlight; }
	static LightFlags GetFlag_HasSpotlightShadow(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kSpotShadow; }
	static LightFlags GetFlag_HasHemiShadow(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kHemiShadow; }
	static LightFlags GetFlag_HasOmniShadow(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kOmniShadow; }
	static LightFlags GetFlag_IsPortalStrict(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kPortalStrict; }
	static LightFlags GetFlag_IsNonShadowSpotlight(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kNonShadowSpot; }
	static LightFlags GetFlag_IsNonSpecular(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kNonSpecular; }
	static LightFlags GetFlag_IsAttenuationOnly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kAttenuationOnly; }
	static LightFlags GetFlag_IsNonShadowBox(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kNonShadowBox; }
	static LightFlags GetFlag_IgnoreRoughness(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kIgnoreRoughness; }
	static LightFlags GetFlag_HasNoRimLighting(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kNoRimLighting; }
	static LightFlags GetFlag_IsAmbientOnly(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightFlags::kAmbientOnly; }

	static LightRecordFlags GetRecordFlag_HasDistantLod(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightRecordFlags::kHasDistantLOD; }
	static LightRecordFlags GetRecordFlag_HasRandomAnimStart(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightRecordFlags::kRandomAnimStart; }
	static LightRecordFlags GetRecordFlag_IsObstacle(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightRecordFlags::kObstacle; }
	static LightRecordFlags GetRecordFlag_IsPortalStrict(RE::BSScript::StaticTag /*a_staticTag*/) noexcept { return LightRecordFlags::kPortalStrict; }

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetSound);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFov);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFov);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFalloffExponent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFalloffExponent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetRadius);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetNearDistance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetNearDistance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetColor);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetColorFade);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetColorFade);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlickerPeriod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlickerPeriod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlickerIntensityAmplitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlickerIntensityAmplitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlickerMovementAmplitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlickerMovementAmplitude);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetGodRays);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetGodRays);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetGodRaysNearDistance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetGodRaysNearDistance);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetLensFlare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetLensFlare);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAttenuationConstant);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAttenuationConstant);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAttenuationScalar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAttenuationScalar);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetAttenuationExponent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetAttenuationExponent);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, HasFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlags);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, SetFlag);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsDynamic);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CanBeCarried);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsNegative);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CanFlicker);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsDeepCopy);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsOffByDefault);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_FlickerSlowly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_CanPulse);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_PulseSlowly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsSpotlight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasSpotlightShadow);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasHemiShadow);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasOmniShadow);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsPortalStrict);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsNonShadowSpotlight);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsNonSpecular);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsAttenuationOnly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsNonShadowBox);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IgnoreRoughness);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_HasNoRimLighting);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetFlag_IsAmbientOnly);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasDistantLod);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_HasRandomAnimStart);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsObstacle);
		RE_REGISTER_VM_FUNCTION_ASYNC(a_vm, SCRIPT_NAME, GetRecordFlag_IsPortalStrict);
	}
}
