#include "Plugin/Internal/Patches/EditorIdPatch.hpp"

#include "Plugin/Internal/Game/GameEditorIds.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Internal::Patches::EditorIdPatch::Impl
{
	template <class T>
	concept TESFormConstraint =
		std::derived_from<T, RE::TESForm> &&
		!std::is_pointer_v<T> &&
		!std::is_reference_v<T>;
}

namespace Plugin::Internal::Patches::EditorIdPatch
{
	template <Impl::TESFormConstraint T>
	class Hook final
	{
	public:
		Hook() = delete;

		static void Setup(REL::HookStore& a_hookStore)
		{
			constexpr const auto& RTTI_ID = T::RTTI;
			constexpr const auto& VTABLE_ID = T::VTABLE[0];

			const auto typeDescriptor = REL::Relocation<const RE::RTTI::TypeDescriptor*>{ RTTI_ID };
			REX::Ensure(static_cast<bool>(typeDescriptor));

			const auto typeDescriptorName = std::string_view(typeDescriptor->name());

			auto hookName01 = REX::Format("{}::GetFormEditorID"sv, typeDescriptorName);
			auto hookName02 = REX::Format("{}::SetFormEditorID"sv, typeDescriptorName);

			auto getFormEditorIDHook = std::make_shared<typename decltype(GetFormEditorIDHook)::element_type>(
				std::move(hookName01), VTABLE_ID, 0x3A, Hook::GetFormEditorID);
			auto setFormEditorIDHook = std::make_shared<typename decltype(SetFormEditorIDHook)::element_type>(
				std::move(hookName02), VTABLE_ID, 0x3B, Hook::SetFormEditorID);

			a_hookStore.Add(getFormEditorIDHook);
			a_hookStore.Add(setFormEditorIDHook);

			GetFormEditorIDHook = std::move(getFormEditorIDHook);
			SetFormEditorIDHook = std::move(setFormEditorIDHook);
		}

	private:
		static const char* GetFormEditorID(const RE::TESForm* a_self)
		{
			const auto editorId = Plugin::GetInternalManager()->GetGameManager()->GetEditorIds()->GetFormEditorId(a_self);
			if (editorId.has_value()) {
				return editorId->data();
			}

			return std::invoke(*GetFormEditorIDHook, a_self);
		}

		static bool SetFormEditorID(RE::TESForm* a_self, const char* a_editorId)
		{
			const auto result = Plugin::GetInternalManager()->GetGameManager()->GetEditorIds()->SetFormEditorId(a_self, a_editorId);
			std::ignore = std::invoke(*SetFormEditorIDHook, a_self, a_editorId);
			return result;
		}

		inline static constinit auto GetFormEditorIDHook = std::shared_ptr<REL::HookVft<decltype(&Hook::GetFormEditorID)>>();
		inline static constinit auto SetFormEditorIDHook = std::shared_ptr<REL::HookVft<decltype(&Hook::SetFormEditorID)>>();
	};

	void OnXseLoad(REL::HookStore& a_hookStore)
	{
		Hook<RE::TESForm>::Setup(a_hookStore);
		Hook<RE::TESObject>::Setup(a_hookStore);
		Hook<RE::TESBoundObject>::Setup(a_hookStore);
		Hook<RE::MagicItem>::Setup(a_hookStore);
		Hook<RE::TESBoundAnimObject>::Setup(a_hookStore);
		Hook<RE::TESActorBase>::Setup(a_hookStore);
		Hook<RE::BGSStoryManagerTreeForm>::Setup(a_hookStore);
		Hook<RE::BGSStoryManagerNodeBase>::Setup(a_hookStore);
		Hook<RE::BGSKeyword>::Setup(a_hookStore);
		Hook<RE::BGSLocationRefType>::Setup(a_hookStore);
		Hook<RE::BGSAction>::Setup(a_hookStore);
		Hook<RE::BGSTransform>::Setup(a_hookStore);
		Hook<RE::BGSComponent>::Setup(a_hookStore);
		Hook<RE::BGSTextureSet>::Setup(a_hookStore);
		Hook<RE::BGSMenuIcon>::Setup(a_hookStore);
		Hook<RE::TESGlobal>::Setup(a_hookStore);
		Hook<RE::BGSDamageType>::Setup(a_hookStore);
		Hook<RE::TESClass>::Setup(a_hookStore);
		Hook<RE::TESFaction>::Setup(a_hookStore);
		Hook<RE::BGSHeadPart>::Setup(a_hookStore);
		Hook<RE::TESEyes>::Setup(a_hookStore);
		Hook<RE::TESRace>::Setup(a_hookStore);
		Hook<RE::TESSound>::Setup(a_hookStore);
		Hook<RE::BGSAcousticSpace>::Setup(a_hookStore);
		Hook<RE::EffectSetting>::Setup(a_hookStore);
		Hook<RE::Script>::Setup(a_hookStore);
		Hook<RE::TESLandTexture>::Setup(a_hookStore);
		Hook<RE::EnchantmentItem>::Setup(a_hookStore);
		Hook<RE::SpellItem>::Setup(a_hookStore);
		Hook<RE::ScrollItem>::Setup(a_hookStore);
		Hook<RE::TESObjectACTI>::Setup(a_hookStore);
		Hook<RE::BGSTalkingActivator>::Setup(a_hookStore);
		Hook<RE::TESObjectARMO>::Setup(a_hookStore);
		Hook<RE::TESObjectBOOK>::Setup(a_hookStore);
		Hook<RE::TESObjectCONT>::Setup(a_hookStore);
		Hook<RE::TESObjectDOOR>::Setup(a_hookStore);
		Hook<RE::IngredientItem>::Setup(a_hookStore);
		Hook<RE::TESObjectLIGH>::Setup(a_hookStore);
		Hook<RE::TESObjectMISC>::Setup(a_hookStore);
		Hook<RE::TESObjectSTAT>::Setup(a_hookStore);
		Hook<RE::BGSStaticCollection>::Setup(a_hookStore);
		Hook<RE::BGSMovableStatic>::Setup(a_hookStore);
		Hook<RE::TESGrass>::Setup(a_hookStore);
		Hook<RE::TESObjectTREE>::Setup(a_hookStore);
		Hook<RE::TESFlora>::Setup(a_hookStore);
		Hook<RE::TESFurniture>::Setup(a_hookStore);
		Hook<RE::TESObjectWEAP>::Setup(a_hookStore);
		Hook<RE::TESAmmo>::Setup(a_hookStore);
		Hook<RE::TESNPC>::Setup(a_hookStore);
		Hook<RE::TESLevCharacter>::Setup(a_hookStore);
		Hook<RE::TESKey>::Setup(a_hookStore);
		Hook<RE::AlchemyItem>::Setup(a_hookStore);
		Hook<RE::BGSIdleMarker>::Setup(a_hookStore);
		Hook<RE::BGSNote>::Setup(a_hookStore);
		Hook<RE::BGSProjectile>::Setup(a_hookStore);
		Hook<RE::BGSHazard>::Setup(a_hookStore);
		Hook<RE::BGSBendableSpline>::Setup(a_hookStore);
		Hook<RE::TESSoulGem>::Setup(a_hookStore);
		Hook<RE::BGSTerminal>::Setup(a_hookStore);
		Hook<RE::TESLevItem>::Setup(a_hookStore);
		Hook<RE::TESWeather>::Setup(a_hookStore);
		Hook<RE::TESClimate>::Setup(a_hookStore);
		Hook<RE::BGSShaderParticleGeometryData>::Setup(a_hookStore);
		Hook<RE::BGSReferenceEffect>::Setup(a_hookStore);
		Hook<RE::TESRegion>::Setup(a_hookStore);
		Hook<RE::NavMeshInfoMap>::Setup(a_hookStore);
		Hook<RE::TESObjectCELL>::Setup(a_hookStore);
		Hook<RE::TESObjectREFR>::Setup(a_hookStore);
		Hook<RE::Explosion>::Setup(a_hookStore);
		Hook<RE::Projectile>::Setup(a_hookStore);
		Hook<RE::Actor>::Setup(a_hookStore);
		Hook<RE::PlayerCharacter>::Setup(a_hookStore);
		Hook<RE::MissileProjectile>::Setup(a_hookStore);
		Hook<RE::ArrowProjectile>::Setup(a_hookStore);
		Hook<RE::GrenadeProjectile>::Setup(a_hookStore);
		Hook<RE::BeamProjectile>::Setup(a_hookStore);
		Hook<RE::FlameProjectile>::Setup(a_hookStore);
		Hook<RE::ConeProjectile>::Setup(a_hookStore);
		Hook<RE::BarrierProjectile>::Setup(a_hookStore);
		Hook<RE::Hazard>::Setup(a_hookStore);
		Hook<RE::TESWorldSpace>::Setup(a_hookStore);
		Hook<RE::TESObjectLAND>::Setup(a_hookStore);
		Hook<RE::NavMesh>::Setup(a_hookStore);
		Hook<RE::TESTopic>::Setup(a_hookStore);
		Hook<RE::TESTopicInfo>::Setup(a_hookStore);
		Hook<RE::TESQuest>::Setup(a_hookStore);
		Hook<RE::TESIdleForm>::Setup(a_hookStore);
		Hook<RE::TESPackage>::Setup(a_hookStore);
		Hook<RE::AlarmPackage>::Setup(a_hookStore);
		Hook<RE::DialoguePackage>::Setup(a_hookStore);
		Hook<RE::FleePackage>::Setup(a_hookStore);
		Hook<RE::SpectatorPackage>::Setup(a_hookStore);
		Hook<RE::TrespassPackage>::Setup(a_hookStore);
		Hook<RE::TESCombatStyle>::Setup(a_hookStore);
		Hook<RE::TESLoadScreen>::Setup(a_hookStore);
		Hook<RE::TESLevSpell>::Setup(a_hookStore);
		Hook<RE::TESObjectANIO>::Setup(a_hookStore);
		Hook<RE::TESWaterForm>::Setup(a_hookStore);
		Hook<RE::TESEffectShader>::Setup(a_hookStore);
		Hook<RE::BGSExplosion>::Setup(a_hookStore);
		Hook<RE::BGSDebris>::Setup(a_hookStore);
		Hook<RE::TESImageSpace>::Setup(a_hookStore);
		Hook<RE::TESImageSpaceModifier>::Setup(a_hookStore);
		Hook<RE::BGSListForm>::Setup(a_hookStore);
		Hook<RE::BGSPerk>::Setup(a_hookStore);
		Hook<RE::BGSBodyPartData>::Setup(a_hookStore);
		Hook<RE::BGSAddonNode>::Setup(a_hookStore);
		Hook<RE::ActorValueInfo>::Setup(a_hookStore);
		Hook<RE::BGSCameraShot>::Setup(a_hookStore);
		Hook<RE::BGSCameraPath>::Setup(a_hookStore);
		Hook<RE::BGSVoiceType>::Setup(a_hookStore);
		Hook<RE::BGSMaterialType>::Setup(a_hookStore);
		Hook<RE::BGSImpactData>::Setup(a_hookStore);
		Hook<RE::BGSImpactDataSet>::Setup(a_hookStore);
		Hook<RE::TESObjectARMA>::Setup(a_hookStore);
		Hook<RE::BGSEncounterZone>::Setup(a_hookStore);
		Hook<RE::BGSLocation>::Setup(a_hookStore);
		Hook<RE::BGSMessage>::Setup(a_hookStore);
		Hook<RE::BGSDefaultObjectManager>::Setup(a_hookStore);
		Hook<RE::BGSDefaultObject>::Setup(a_hookStore);
		Hook<RE::BGSLightingTemplate>::Setup(a_hookStore);
		Hook<RE::BGSMusicType>::Setup(a_hookStore);
		Hook<RE::BGSFootstep>::Setup(a_hookStore);
		Hook<RE::BGSFootstepSet>::Setup(a_hookStore);
		Hook<RE::BGSStoryManagerBranchNode>::Setup(a_hookStore);
		Hook<RE::BGSStoryManagerQuestNode>::Setup(a_hookStore);
		Hook<RE::BGSStoryManagerEventNode>::Setup(a_hookStore);
		Hook<RE::BGSDialogueBranch>::Setup(a_hookStore);
		Hook<RE::BGSMusicTrackFormWrapper>::Setup(a_hookStore);
		Hook<RE::TESWordOfPower>::Setup(a_hookStore);
		Hook<RE::TESShout>::Setup(a_hookStore);
		Hook<RE::BGSEquipSlot>::Setup(a_hookStore);
		Hook<RE::BGSRelationship>::Setup(a_hookStore);
		Hook<RE::BGSScene>::Setup(a_hookStore);
		Hook<RE::BGSAssociationType>::Setup(a_hookStore);
		Hook<RE::BGSOutfit>::Setup(a_hookStore);
		Hook<RE::BGSArtObject>::Setup(a_hookStore);
		Hook<RE::BGSMaterialObject>::Setup(a_hookStore);
		Hook<RE::BGSMovementType>::Setup(a_hookStore);
		Hook<RE::BGSSoundDescriptorForm>::Setup(a_hookStore);
		Hook<RE::BGSDualCastData>::Setup(a_hookStore);
		Hook<RE::BGSSoundCategory>::Setup(a_hookStore);
		Hook<RE::BGSSoundOutput>::Setup(a_hookStore);
		Hook<RE::BGSCollisionLayer>::Setup(a_hookStore);
		Hook<RE::BGSColorForm>::Setup(a_hookStore);
		Hook<RE::BGSReverbParameters>::Setup(a_hookStore);
		Hook<RE::BGSPackIn>::Setup(a_hookStore);
		Hook<RE::BGSReferenceGroup>::Setup(a_hookStore);
		Hook<RE::BGSAimModel>::Setup(a_hookStore);
		Hook<RE::BGSConstructibleObject>::Setup(a_hookStore);
		Hook<RE::BGSMod::Attachment::Mod>::Setup(a_hookStore);
		Hook<RE::BGSMaterialSwap>::Setup(a_hookStore);
		Hook<RE::BGSZoomData>::Setup(a_hookStore);
		Hook<RE::BGSInstanceNamingRules>::Setup(a_hookStore);
		Hook<RE::BGSSoundKeywordMapping>::Setup(a_hookStore);
		Hook<RE::BGSAudioEffectChain>::Setup(a_hookStore);
		Hook<RE::BGSAttractionRule>::Setup(a_hookStore);
		Hook<RE::BGSSoundCategorySnapshot>::Setup(a_hookStore);
		Hook<RE::BGSSoundTagSet>::Setup(a_hookStore);
		Hook<RE::NavMeshObstacleCoverManager>::Setup(a_hookStore);
		Hook<RE::BGSLensFlare>::Setup(a_hookStore);
		Hook<RE::BGSGodRays>::Setup(a_hookStore);
		Hook<RE::BGSObjectVisibilityManager>::Setup(a_hookStore);
	}

	void OnGameDataReady()
	{
		auto* playerRef = RE::PlayerCharacter::GetSingleton();
		if (!playerRef) [[unlikely]] {
			REX::Fail("Failed to get the game's player reference."sv);
		}

		auto* player = playerRef->GetActorBase();
		if (!player) [[unlikely]] {
			REX::Fail("Failed to get the game's player base object."sv);
		}

		const auto editorIds = Plugin::GetInternalManager()->GetGameManager()->GetEditorIds();

		std::ignore = editorIds->SetFormEditorId(player, RE::TESForm::PLAYER_BASE_EDITOR_ID.data());
		std::ignore = editorIds->SetFormEditorId(playerRef, RE::TESForm::PLAYER_REF_EDITOR_ID.data());
	}
}
