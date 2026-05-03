#include "Plugin/PapyrusManager.hpp"

#include "Plugin/Papyrus/Forms/Activator.hpp"
#include "Plugin/Papyrus/Forms/ActiveMagicEffect.hpp"
#include "Plugin/Papyrus/Forms/Actor.hpp"
#include "Plugin/Papyrus/Forms/ActorBase.hpp"
#include "Plugin/Papyrus/Forms/AimModel.hpp"
#include "Plugin/Papyrus/Forms/Alias.hpp"
#include "Plugin/Papyrus/Forms/Ammo.hpp"
#include "Plugin/Papyrus/Forms/ArmorAddon.hpp"
#include "Plugin/Papyrus/Forms/Book.hpp"
#include "Plugin/Papyrus/Forms/Cell.hpp"
#include "Plugin/Papyrus/Forms/Climate.hpp"
#include "Plugin/Papyrus/Forms/ColorForm.hpp"
#include "Plugin/Papyrus/Forms/Component.hpp"
#include "Plugin/Papyrus/Forms/ConstructibleObject.hpp"
#include "Plugin/Papyrus/Forms/Container.hpp"
#include "Plugin/Papyrus/Forms/DamageType.hpp"
#include "Plugin/Papyrus/Forms/Door.hpp"
#include "Plugin/Papyrus/Forms/Enchantment.hpp"
#include "Plugin/Papyrus/Forms/EncounterZone.hpp"
#include "Plugin/Papyrus/Forms/Explosion.hpp"
#include "Plugin/Papyrus/Forms/Faction.hpp"
#include "Plugin/Papyrus/Forms/Flora.hpp"
#include "Plugin/Papyrus/Forms/Form.hpp"
#include "Plugin/Papyrus/Forms/FormList.hpp"
#include "Plugin/Papyrus/Forms/GlobalVariable.hpp"
#include "Plugin/Papyrus/Forms/HeadPart.hpp"
#include "Plugin/Papyrus/Forms/Holotape.hpp"
#include "Plugin/Papyrus/Forms/Idle.hpp"
#include "Plugin/Papyrus/Forms/InputEnableLayer.hpp"
#include "Plugin/Papyrus/Forms/InstanceNamingRules.hpp"
#include "Plugin/Papyrus/Forms/LeveledActor.hpp"
#include "Plugin/Papyrus/Forms/LeveledItem.hpp"
#include "Plugin/Papyrus/Forms/LeveledList.hpp"
#include "Plugin/Papyrus/Forms/Light.hpp"
#include "Plugin/Papyrus/Forms/Location.hpp"
#include "Plugin/Papyrus/Forms/MagicEffect.hpp"
#include "Plugin/Papyrus/Forms/MagicItem.hpp"
#include "Plugin/Papyrus/Forms/Message.hpp"
#include "Plugin/Papyrus/Forms/MiscObject.hpp"
#include "Plugin/Papyrus/Forms/MovementType.hpp"
#include "Plugin/Papyrus/Forms/ObjectReference.hpp"
#include "Plugin/Papyrus/Forms/Outfit.hpp"
#include "Plugin/Papyrus/Forms/Perk.hpp"
#include "Plugin/Papyrus/Forms/Potion.hpp"
#include "Plugin/Papyrus/Forms/Projectile.hpp"
#include "Plugin/Papyrus/Forms/Spell.hpp"
#include "Plugin/Papyrus/Forms/WorldSpace.hpp"

#include "Plugin/Papyrus/IO/Directory.hpp"
#include "Plugin/Papyrus/IO/File.hpp"
#include "Plugin/Papyrus/IO/Ini.hpp"
#include "Plugin/Papyrus/IO/Json.hpp"
#include "Plugin/Papyrus/IO/Link.hpp"
#include "Plugin/Papyrus/IO/Path.hpp"
#include "Plugin/Papyrus/IO/Permissions.hpp"
#include "Plugin/Papyrus/IO/Space.hpp"
#include "Plugin/Papyrus/IO/Toml.hpp"

#include "Plugin/Papyrus/Arrays.hpp"
#include "Plugin/Papyrus/Assert.hpp"
#include "Plugin/Papyrus/Boolean.hpp"
#include "Plugin/Papyrus/Camera.hpp"
#include "Plugin/Papyrus/Char.hpp"
#include "Plugin/Papyrus/Colors.hpp"
#include "Plugin/Papyrus/Console.hpp"
#include "Plugin/Papyrus/Crypto.hpp"
#include "Plugin/Papyrus/DateTimes.hpp"
#include "Plugin/Papyrus/Events.hpp"
#include "Plugin/Papyrus/Float32.hpp"
#include "Plugin/Papyrus/Float64.hpp"
#include "Plugin/Papyrus/FunctionRefs.hpp"
#include "Plugin/Papyrus/Game.hpp"
#include "Plugin/Papyrus/GameSettings.hpp"
#include "Plugin/Papyrus/Hashing.hpp"
#include "Plugin/Papyrus/IniSettings.hpp"
#include "Plugin/Papyrus/Input.hpp"
#include "Plugin/Papyrus/Int32.hpp"
#include "Plugin/Papyrus/Int64.hpp"
#include "Plugin/Papyrus/Internal.hpp"
#include "Plugin/Papyrus/MemMap.hpp"
#include "Plugin/Papyrus/MemSet.hpp"
#include "Plugin/Papyrus/MiscStats.hpp"
#include "Plugin/Papyrus/Mutex.hpp"
#include "Plugin/Papyrus/NetImmerse.hpp"
#include "Plugin/Papyrus/Operator.hpp"
#include "Plugin/Papyrus/Pairs.hpp"
#include "Plugin/Papyrus/Plugins.hpp"
#include "Plugin/Papyrus/Random.hpp"
#include "Plugin/Papyrus/Regex.hpp"
#include "Plugin/Papyrus/SaveMap.hpp"
#include "Plugin/Papyrus/SaveSet.hpp"
#include "Plugin/Papyrus/Saves.hpp"
#include "Plugin/Papyrus/Scaleform.hpp"
#include "Plugin/Papyrus/ScriptObject.hpp"
#include "Plugin/Papyrus/ScriptStack.hpp"
#include "Plugin/Papyrus/ScriptType.hpp"
#include "Plugin/Papyrus/Sky.hpp"
#include "Plugin/Papyrus/StrictStrings.hpp"
#include "Plugin/Papyrus/StringPairs.hpp"
#include "Plugin/Papyrus/Strings.hpp"
#include "Plugin/Papyrus/StructObject.hpp"
#include "Plugin/Papyrus/StructType.hpp"
#include "Plugin/Papyrus/TempMap.hpp"
#include "Plugin/Papyrus/TempSet.hpp"
#include "Plugin/Papyrus/Time.hpp"
#include "Plugin/Papyrus/Timer.hpp"
#include "Plugin/Papyrus/Translations.hpp"
#include "Plugin/Papyrus/UI.hpp"
#include "Plugin/Papyrus/UInt32.hpp"
#include "Plugin/Papyrus/UInt64.hpp"
#include "Plugin/Papyrus/Uuids.hpp"
#include "Plugin/Papyrus/VarPairs.hpp"
#include "Plugin/Papyrus/Vectors2.hpp"
#include "Plugin/Papyrus/Vectors3.hpp"
#include "Plugin/Papyrus/Vectors4.hpp"
#include "Plugin/Papyrus/Versions.hpp"
#include "Plugin/Papyrus/Wildcards.hpp"
#include "Plugin/Papyrus/Xse.hpp"

namespace Plugin::PapyrusManager::Impl
{
	[[nodiscard]] static std::uint32_t RegisterType(
		REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm,
		REX::zstring_view a_name,
		RE::FormType a_formType)
	{
		REX::LogTrace(R"(Registering script type "{}" with form type {})"sv,
			a_name, a_formType);

		auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!(a_vm->GetScriptObjectTypeNoLoad(a_formType, objectTypeInfo) && objectTypeInfo && objectTypeInfo->GetName() == a_name) &&
			!a_vm->RegisterObjectType(a_formType, a_name.data())) [[unlikely]] {
			REX::Fail(R"(Failed to register script type "{}" with form type {})"sv,
				a_name, a_formType);
		}

		return 1;
	}

	[[nodiscard]] static std::uint32_t RegisterFunctions(
		REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm,
		std::string_view a_scriptName,
		const REX::NotNull<std::function<void(REX::NotNull<RE::BSScript::IVirtualMachine*>)>>& a_registrationFunctor)
	{
		REX::LogTrace(R"(Registering functions for script type "{}")"sv,
			a_scriptName);

		auto objectTypeInfo = RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo>();
		if (!a_vm->GetScriptObjectType(RE::BSFixedString(a_scriptName), objectTypeInfo) || !objectTypeInfo) [[unlikely]] {
			REX::Fail(R"(Failed to load script type "{}".)"sv,
				a_scriptName);
		}

		std::invoke(*a_registrationFunctor, a_vm);
		return 1;
	}
}

namespace Plugin::PapyrusManager
{
	void Setup(REX::NotNull<const F4SE::PapyrusInterface*> a_script)
	{
		static constexpr auto FUNCTION_NAME = REX::GetCurrentFunctionName();

		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, [a_script]() {
			REX::LogDebug("{}"sv,
				FUNCTION_NAME);

			a_script->Register(Callback);
		});
	}

	bool Callback(RE::BSScript::IVirtualMachine* a_vm)
	{
		if (!a_vm) [[unlikely]] {
			REX::Assert(false);
			return false;
		}

		RegisterTypes(a_vm);
		RegisterFunctions(a_vm);

		return true;
	}

	void RegisterTypes(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
#define REGISTER_TYPE(a_vm, a_name, a_type) \
	totalCount += Impl::RegisterType(a_vm, a_name, a_type::FORM_TYPE)

			REGISTER_TYPE(a_vm, "AcousticSpace"sv, RE::BGSAcousticSpace);
			REGISTER_TYPE(a_vm, "AddonNode"sv, RE::BGSAddonNode);
			REGISTER_TYPE(a_vm, "AimModel"sv, RE::BGSAimModel);
			REGISTER_TYPE(a_vm, "AnimObject"sv, RE::TESObjectANIO);
			REGISTER_TYPE(a_vm, "ArrowProjectile"sv, RE::ArrowProjectile);
			REGISTER_TYPE(a_vm, "ArtObject"sv, RE::BGSArtObject);
			REGISTER_TYPE(a_vm, "AttractionRule"sv, RE::BGSAttractionRule);
			REGISTER_TYPE(a_vm, "AudioEffectChain"sv, RE::BGSAudioEffectChain);
			REGISTER_TYPE(a_vm, "BarrierProjectile"sv, RE::BarrierProjectile);
			REGISTER_TYPE(a_vm, "BeamProjectile"sv, RE::BeamProjectile);
			REGISTER_TYPE(a_vm, "BendableSpline"sv, RE::BGSBendableSpline);
			REGISTER_TYPE(a_vm, "BodyPartData"sv, RE::BGSBodyPartData);
			REGISTER_TYPE(a_vm, "CameraPath"sv, RE::BGSCameraPath);
			REGISTER_TYPE(a_vm, "Climate"sv, RE::TESClimate);
			REGISTER_TYPE(a_vm, "CollisionLayer"sv, RE::BGSCollisionLayer);
			REGISTER_TYPE(a_vm, "ColorForm"sv, RE::BGSColorForm);
			REGISTER_TYPE(a_vm, "ConeProjectile"sv, RE::ConeProjectile);
			REGISTER_TYPE(a_vm, "DamageType"sv, RE::BGSDamageType);
			REGISTER_TYPE(a_vm, "Debris"sv, RE::BGSDebris);
			REGISTER_TYPE(a_vm, "DefaultObjectManager"sv, RE::BGSDefaultObjectManager);
			REGISTER_TYPE(a_vm, "DialogueBranch"sv, RE::BGSDialogueBranch);
			REGISTER_TYPE(a_vm, "DualCastData"sv, RE::BGSDualCastData);
			REGISTER_TYPE(a_vm, "Footstep"sv, RE::BGSFootstep);
			REGISTER_TYPE(a_vm, "FootstepSet"sv, RE::BGSFootstepSet);
			REGISTER_TYPE(a_vm, "FlameProjectile"sv, RE::FlameProjectile);
			REGISTER_TYPE(a_vm, "GodRays"sv, RE::BGSGodRays);
			REGISTER_TYPE(a_vm, "Grass"sv, RE::TESGrass);
			REGISTER_TYPE(a_vm, "GrenadeProjectile"sv, RE::GrenadeProjectile);
			REGISTER_TYPE(a_vm, "ImageSpace"sv, RE::TESImageSpace);
			REGISTER_TYPE(a_vm, "ImpactData"sv, RE::BGSImpactData);
			REGISTER_TYPE(a_vm, "Landscape"sv, RE::TESObjectLAND);
			REGISTER_TYPE(a_vm, "LandscapeTexture"sv, RE::TESLandTexture);
			REGISTER_TYPE(a_vm, "LensFlare"sv, RE::BGSLensFlare);
			REGISTER_TYPE(a_vm, "LightingTemplate"sv, RE::BGSLightingTemplate);
			REGISTER_TYPE(a_vm, "LoadScreen"sv, RE::TESLoadScreen);
			REGISTER_TYPE(a_vm, "MusicTrack"sv, RE::BGSMusicTrackFormWrapper);
			REGISTER_TYPE(a_vm, "MaterialObject"sv, RE::BGSMaterialObject);
			REGISTER_TYPE(a_vm, "MaterialType"sv, RE::BGSMaterialType);
			REGISTER_TYPE(a_vm, "MissileProjectile"sv, RE::MissileProjectile);
			REGISTER_TYPE(a_vm, "MovementType"sv, RE::BGSMovementType);
			REGISTER_TYPE(a_vm, "NavMesh"sv, RE::NavMesh);
			REGISTER_TYPE(a_vm, "NavMeshInfoMap"sv, RE::NavMeshInfoMap);
			REGISTER_TYPE(a_vm, "NavMeshObstacleManager"sv, RE::NavMeshObstacleCoverManager);
			REGISTER_TYPE(a_vm, "ObjectVisibilityManager"sv, RE::BGSObjectVisibilityManager);
			REGISTER_TYPE(a_vm, "PackIn"sv, RE::BGSPackIn);
			REGISTER_TYPE(a_vm, "PlacedHazard"sv, RE::Hazard);
			REGISTER_TYPE(a_vm, "ReferenceGroup"sv, RE::BGSReferenceGroup);
			REGISTER_TYPE(a_vm, "Region"sv, RE::TESRegion);
			REGISTER_TYPE(a_vm, "Relationship"sv, RE::BGSRelationship);
			REGISTER_TYPE(a_vm, "ReverbParameters"sv, RE::BGSReverbParameters);
			REGISTER_TYPE(a_vm, "StaticCollection"sv, RE::BGSStaticCollection);
			REGISTER_TYPE(a_vm, "SoundKeywordMapping"sv, RE::BGSSoundKeywordMapping);
			REGISTER_TYPE(a_vm, "SoundMarker"sv, RE::TESSound);
			REGISTER_TYPE(a_vm, "SoundTagSet"sv, RE::BGSSoundTagSet);
			REGISTER_TYPE(a_vm, "StoryManagerBranchNode"sv, RE::BGSStoryManagerBranchNode);
			REGISTER_TYPE(a_vm, "StoryManagerEventNode"sv, RE::BGSStoryManagerEventNode);
			REGISTER_TYPE(a_vm, "StoryManagerQuestNode"sv, RE::BGSStoryManagerQuestNode);
			REGISTER_TYPE(a_vm, "Transform"sv, RE::BGSTransform);
			REGISTER_TYPE(a_vm, "Tree"sv, RE::TESObjectTREE);
			REGISTER_TYPE(a_vm, "ZoomData"sv, RE::BGSZoomData);

#undef REGISTER_TYPE
		}

		stopwatch.Stop();

		REX::LogDebug(R"(Registered {} script types in {})"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}

	void RegisterFunctions(REX::NotNull<RE::BSScript::IVirtualMachine*> a_vm)
	{
		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		auto stopwatch = REX::Stopwatch::StartNew();
		auto totalCount = 0ui32;

		{
#define REGISTER_FUNCTIONS(a_vm, a_type) \
	totalCount += Impl::RegisterFunctions(a_vm, a_type::SCRIPT_NAME, &a_type::RegisterFunctions)

			{
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Activator);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::ActiveMagicEffect);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Actor);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::ActorBase);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::AimModel);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Alias);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Ammo);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::ArmorAddon);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Book);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Cell);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Climate);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::ColorForm);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Component);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::ConstructibleObject);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Container);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::DamageType);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Door);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Enchantment);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::EncounterZone);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Explosion);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Faction);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Flora);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Form);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::FormList);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::GlobalVariable);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::HeadPart);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Holotape);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Idle);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::InputEnableLayer);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::InstanceNamingRules);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::LeveledActor);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::LeveledItem);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::LeveledList);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Light);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Location);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::MagicEffect);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::MagicItem);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Message);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::MiscObject);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::MovementType);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::ObjectReference);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Outfit);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Perk);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Potion);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Projectile);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::Spell);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Forms::WorldSpace);
			}

			{
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::Directory);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::File);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::Ini);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::Json);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::Link);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::Path);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::Permissions);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::Space);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IO::Toml);
			}

			{
				REGISTER_FUNCTIONS(a_vm, Papyrus::Arrays);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Assert);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Boolean);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Camera);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Char);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Colors);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Console);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Crypto);
				REGISTER_FUNCTIONS(a_vm, Papyrus::DateTimes);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Events);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Float32);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Float64);
				REGISTER_FUNCTIONS(a_vm, Papyrus::FunctionRefs);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Game);
				REGISTER_FUNCTIONS(a_vm, Papyrus::GameSettings);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Hashing);
				REGISTER_FUNCTIONS(a_vm, Papyrus::IniSettings);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Input);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Int32);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Int64);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Internal);
				REGISTER_FUNCTIONS(a_vm, Papyrus::MemMap);
				REGISTER_FUNCTIONS(a_vm, Papyrus::MemSet);
				REGISTER_FUNCTIONS(a_vm, Papyrus::MiscStats);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Mutex);
				REGISTER_FUNCTIONS(a_vm, Papyrus::NetImmerse);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Operator);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Pairs);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Plugins);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Random);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Regex);
				REGISTER_FUNCTIONS(a_vm, Papyrus::SaveMap);
				REGISTER_FUNCTIONS(a_vm, Papyrus::SaveSet);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Saves);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Scaleform);
				REGISTER_FUNCTIONS(a_vm, Papyrus::ScriptObject);
				REGISTER_FUNCTIONS(a_vm, Papyrus::ScriptStack);
				REGISTER_FUNCTIONS(a_vm, Papyrus::ScriptType);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Sky);
				REGISTER_FUNCTIONS(a_vm, Papyrus::StrictStrings);
				REGISTER_FUNCTIONS(a_vm, Papyrus::StringPairs);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Strings);
				REGISTER_FUNCTIONS(a_vm, Papyrus::StructObject);
				REGISTER_FUNCTIONS(a_vm, Papyrus::StructType);
				REGISTER_FUNCTIONS(a_vm, Papyrus::TempMap);
				REGISTER_FUNCTIONS(a_vm, Papyrus::TempSet);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Time);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Timer);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Translations);
				REGISTER_FUNCTIONS(a_vm, Papyrus::UI);
				REGISTER_FUNCTIONS(a_vm, Papyrus::UInt32);
				REGISTER_FUNCTIONS(a_vm, Papyrus::UInt64);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Uuids);
				REGISTER_FUNCTIONS(a_vm, Papyrus::VarPairs);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Vectors2);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Vectors3);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Vectors4);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Versions);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Wildcards);
				REGISTER_FUNCTIONS(a_vm, Papyrus::Xse);
			}

#undef REGISTER_FUNCTIONS
		}

		stopwatch.Stop();

		REX::LogDebug(R"(Registered all script functions of {} script types in {})"sv,
			totalCount, std::chrono::duration_cast<std::chrono::duration<REX::Float64, std::milli>>(stopwatch.GetElapsedTime()));
	}
}
