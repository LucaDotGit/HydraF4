#include "Plugin/Internal/Patches/EditorIdPatch.hpp"

#include "Plugin/Internal/Game/GameEditorIds.hpp"
#include "Plugin/Internal/GameManager.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::Internal::Patches::Impl
{
	template <class T>
	concept TESFormConstraint =
		std::derived_from<T, RE::TESForm> &&
		!std::is_pointer_v<T> &&
		!std::is_reference_v<T>;

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
}

namespace Plugin::Internal::Patches
{
	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void EditorIdPatch::OnXseLoad(REL::HookStore& a_hookStore)
	{
		Impl::Hook<RE::TESForm>::Setup(a_hookStore);
		Impl::Hook<RE::TESObject>::Setup(a_hookStore);
		Impl::Hook<RE::TESBoundObject>::Setup(a_hookStore);
		Impl::Hook<RE::MagicItem>::Setup(a_hookStore);
		Impl::Hook<RE::TESBoundAnimObject>::Setup(a_hookStore);
		Impl::Hook<RE::TESActorBase>::Setup(a_hookStore);
		Impl::Hook<RE::BGSStoryManagerTreeForm>::Setup(a_hookStore);
		Impl::Hook<RE::BGSStoryManagerNodeBase>::Setup(a_hookStore);
		Impl::Hook<RE::BGSKeyword>::Setup(a_hookStore);
		Impl::Hook<RE::BGSLocationRefType>::Setup(a_hookStore);
		Impl::Hook<RE::BGSAction>::Setup(a_hookStore);
		Impl::Hook<RE::BGSTransform>::Setup(a_hookStore);
		Impl::Hook<RE::BGSComponent>::Setup(a_hookStore);
		Impl::Hook<RE::BGSTextureSet>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMenuIcon>::Setup(a_hookStore);
		Impl::Hook<RE::TESGlobal>::Setup(a_hookStore);
		Impl::Hook<RE::BGSDamageType>::Setup(a_hookStore);
		Impl::Hook<RE::TESClass>::Setup(a_hookStore);
		Impl::Hook<RE::TESFaction>::Setup(a_hookStore);
		Impl::Hook<RE::BGSHeadPart>::Setup(a_hookStore);
		Impl::Hook<RE::TESEyes>::Setup(a_hookStore);
		Impl::Hook<RE::TESRace>::Setup(a_hookStore);
		Impl::Hook<RE::TESSound>::Setup(a_hookStore);
		Impl::Hook<RE::BGSAcousticSpace>::Setup(a_hookStore);
		Impl::Hook<RE::EffectSetting>::Setup(a_hookStore);
		Impl::Hook<RE::Script>::Setup(a_hookStore);
		Impl::Hook<RE::TESLandTexture>::Setup(a_hookStore);
		Impl::Hook<RE::EnchantmentItem>::Setup(a_hookStore);
		Impl::Hook<RE::SpellItem>::Setup(a_hookStore);
		Impl::Hook<RE::ScrollItem>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectACTI>::Setup(a_hookStore);
		Impl::Hook<RE::BGSTalkingActivator>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectARMO>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectBOOK>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectCONT>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectDOOR>::Setup(a_hookStore);
		Impl::Hook<RE::IngredientItem>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectLIGH>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectMISC>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectSTAT>::Setup(a_hookStore);
		Impl::Hook<RE::BGSStaticCollection>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMovableStatic>::Setup(a_hookStore);
		Impl::Hook<RE::TESGrass>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectTREE>::Setup(a_hookStore);
		Impl::Hook<RE::TESFlora>::Setup(a_hookStore);
		Impl::Hook<RE::TESFurniture>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectWEAP>::Setup(a_hookStore);
		Impl::Hook<RE::TESAmmo>::Setup(a_hookStore);
		Impl::Hook<RE::TESNPC>::Setup(a_hookStore);
		Impl::Hook<RE::TESLevCharacter>::Setup(a_hookStore);
		Impl::Hook<RE::TESKey>::Setup(a_hookStore);
		Impl::Hook<RE::AlchemyItem>::Setup(a_hookStore);
		Impl::Hook<RE::BGSIdleMarker>::Setup(a_hookStore);
		Impl::Hook<RE::BGSNote>::Setup(a_hookStore);
		Impl::Hook<RE::BGSProjectile>::Setup(a_hookStore);
		Impl::Hook<RE::BGSHazard>::Setup(a_hookStore);
		Impl::Hook<RE::BGSBendableSpline>::Setup(a_hookStore);
		Impl::Hook<RE::TESSoulGem>::Setup(a_hookStore);
		Impl::Hook<RE::BGSTerminal>::Setup(a_hookStore);
		Impl::Hook<RE::TESLevItem>::Setup(a_hookStore);
		Impl::Hook<RE::TESWeather>::Setup(a_hookStore);
		Impl::Hook<RE::TESClimate>::Setup(a_hookStore);
		Impl::Hook<RE::BGSShaderParticleGeometryData>::Setup(a_hookStore);
		Impl::Hook<RE::BGSReferenceEffect>::Setup(a_hookStore);
		Impl::Hook<RE::TESRegion>::Setup(a_hookStore);
		Impl::Hook<RE::NavMeshInfoMap>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectCELL>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectREFR>::Setup(a_hookStore);
		Impl::Hook<RE::Explosion>::Setup(a_hookStore);
		Impl::Hook<RE::Projectile>::Setup(a_hookStore);
		Impl::Hook<RE::Actor>::Setup(a_hookStore);
		Impl::Hook<RE::PlayerCharacter>::Setup(a_hookStore);
		Impl::Hook<RE::MissileProjectile>::Setup(a_hookStore);
		Impl::Hook<RE::ArrowProjectile>::Setup(a_hookStore);
		Impl::Hook<RE::GrenadeProjectile>::Setup(a_hookStore);
		Impl::Hook<RE::BeamProjectile>::Setup(a_hookStore);
		Impl::Hook<RE::FlameProjectile>::Setup(a_hookStore);
		Impl::Hook<RE::ConeProjectile>::Setup(a_hookStore);
		Impl::Hook<RE::BarrierProjectile>::Setup(a_hookStore);
		Impl::Hook<RE::Hazard>::Setup(a_hookStore);
		Impl::Hook<RE::TESWorldSpace>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectLAND>::Setup(a_hookStore);
		Impl::Hook<RE::NavMesh>::Setup(a_hookStore);
		Impl::Hook<RE::TESTopic>::Setup(a_hookStore);
		Impl::Hook<RE::TESTopicInfo>::Setup(a_hookStore);
		Impl::Hook<RE::TESQuest>::Setup(a_hookStore);
		Impl::Hook<RE::TESIdleForm>::Setup(a_hookStore);
		Impl::Hook<RE::TESPackage>::Setup(a_hookStore);
		Impl::Hook<RE::AlarmPackage>::Setup(a_hookStore);
		Impl::Hook<RE::DialoguePackage>::Setup(a_hookStore);
		Impl::Hook<RE::FleePackage>::Setup(a_hookStore);
		Impl::Hook<RE::SpectatorPackage>::Setup(a_hookStore);
		Impl::Hook<RE::TrespassPackage>::Setup(a_hookStore);
		Impl::Hook<RE::TESCombatStyle>::Setup(a_hookStore);
		Impl::Hook<RE::TESLoadScreen>::Setup(a_hookStore);
		Impl::Hook<RE::TESLevSpell>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectANIO>::Setup(a_hookStore);
		Impl::Hook<RE::TESWaterForm>::Setup(a_hookStore);
		Impl::Hook<RE::TESEffectShader>::Setup(a_hookStore);
		Impl::Hook<RE::BGSExplosion>::Setup(a_hookStore);
		Impl::Hook<RE::BGSDebris>::Setup(a_hookStore);
		Impl::Hook<RE::TESImageSpace>::Setup(a_hookStore);
		Impl::Hook<RE::TESImageSpaceModifier>::Setup(a_hookStore);
		Impl::Hook<RE::BGSListForm>::Setup(a_hookStore);
		Impl::Hook<RE::BGSPerk>::Setup(a_hookStore);
		Impl::Hook<RE::BGSBodyPartData>::Setup(a_hookStore);
		Impl::Hook<RE::BGSAddonNode>::Setup(a_hookStore);
		Impl::Hook<RE::ActorValueInfo>::Setup(a_hookStore);
		Impl::Hook<RE::BGSCameraShot>::Setup(a_hookStore);
		Impl::Hook<RE::BGSCameraPath>::Setup(a_hookStore);
		Impl::Hook<RE::BGSVoiceType>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMaterialType>::Setup(a_hookStore);
		Impl::Hook<RE::BGSImpactData>::Setup(a_hookStore);
		Impl::Hook<RE::BGSImpactDataSet>::Setup(a_hookStore);
		Impl::Hook<RE::TESObjectARMA>::Setup(a_hookStore);
		Impl::Hook<RE::BGSEncounterZone>::Setup(a_hookStore);
		Impl::Hook<RE::BGSLocation>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMessage>::Setup(a_hookStore);
		Impl::Hook<RE::BGSDefaultObjectManager>::Setup(a_hookStore);
		Impl::Hook<RE::BGSDefaultObject>::Setup(a_hookStore);
		Impl::Hook<RE::BGSLightingTemplate>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMusicType>::Setup(a_hookStore);
		Impl::Hook<RE::BGSFootstep>::Setup(a_hookStore);
		Impl::Hook<RE::BGSFootstepSet>::Setup(a_hookStore);
		Impl::Hook<RE::BGSStoryManagerBranchNode>::Setup(a_hookStore);
		Impl::Hook<RE::BGSStoryManagerQuestNode>::Setup(a_hookStore);
		Impl::Hook<RE::BGSStoryManagerEventNode>::Setup(a_hookStore);
		Impl::Hook<RE::BGSDialogueBranch>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMusicTrackFormWrapper>::Setup(a_hookStore);
		Impl::Hook<RE::TESWordOfPower>::Setup(a_hookStore);
		Impl::Hook<RE::TESShout>::Setup(a_hookStore);
		Impl::Hook<RE::BGSEquipSlot>::Setup(a_hookStore);
		Impl::Hook<RE::BGSRelationship>::Setup(a_hookStore);
		Impl::Hook<RE::BGSScene>::Setup(a_hookStore);
		Impl::Hook<RE::BGSAssociationType>::Setup(a_hookStore);
		Impl::Hook<RE::BGSOutfit>::Setup(a_hookStore);
		Impl::Hook<RE::BGSArtObject>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMaterialObject>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMovementType>::Setup(a_hookStore);
		Impl::Hook<RE::BGSSoundDescriptorForm>::Setup(a_hookStore);
		Impl::Hook<RE::BGSDualCastData>::Setup(a_hookStore);
		Impl::Hook<RE::BGSSoundCategory>::Setup(a_hookStore);
		Impl::Hook<RE::BGSSoundOutput>::Setup(a_hookStore);
		Impl::Hook<RE::BGSCollisionLayer>::Setup(a_hookStore);
		Impl::Hook<RE::BGSColorForm>::Setup(a_hookStore);
		Impl::Hook<RE::BGSReverbParameters>::Setup(a_hookStore);
		Impl::Hook<RE::BGSPackIn>::Setup(a_hookStore);
		Impl::Hook<RE::BGSReferenceGroup>::Setup(a_hookStore);
		Impl::Hook<RE::BGSAimModel>::Setup(a_hookStore);
		Impl::Hook<RE::BGSConstructibleObject>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMod::Attachment::Mod>::Setup(a_hookStore);
		Impl::Hook<RE::BGSMaterialSwap>::Setup(a_hookStore);
		Impl::Hook<RE::BGSZoomData>::Setup(a_hookStore);
		Impl::Hook<RE::BGSInstanceNamingRules>::Setup(a_hookStore);
		Impl::Hook<RE::BGSSoundKeywordMapping>::Setup(a_hookStore);
		Impl::Hook<RE::BGSAudioEffectChain>::Setup(a_hookStore);
		Impl::Hook<RE::BGSAttractionRule>::Setup(a_hookStore);
		Impl::Hook<RE::BGSSoundCategorySnapshot>::Setup(a_hookStore);
		Impl::Hook<RE::BGSSoundTagSet>::Setup(a_hookStore);
		Impl::Hook<RE::NavMeshObstacleCoverManager>::Setup(a_hookStore);
		Impl::Hook<RE::BGSLensFlare>::Setup(a_hookStore);
		Impl::Hook<RE::BGSGodRays>::Setup(a_hookStore);
		Impl::Hook<RE::BGSObjectVisibilityManager>::Setup(a_hookStore);
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void EditorIdPatch::OnGameDataReady()
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
