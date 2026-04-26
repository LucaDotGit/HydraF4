#include "Plugin/SerializationManager.hpp"

#include "Plugin/Internal/FunctorsManager.hpp"
#include "Plugin/Internal/Save/SaveStream.hpp"
#include "Plugin/Internal/Save/SaveStringPool.hpp"
#include "Plugin/Internal/SaveManager.hpp"
#include "Plugin/InternalManager.hpp"

namespace Plugin::SerializationManager
{
	static constexpr auto STRING_POOL_SAVE_TYPE = REX::HashCrc32("StringPool"sv);
	static constexpr auto STRING_POOL_SAVE_VERSION = 1ui32;

	void Setup(REX::NotNull<const F4SE::SerializationInterface*> a_serializer)
	{
		static constexpr auto FUNCTION_NAME = REX::GetCurrentFunctionName();

		static constinit auto OnceFlag = std::once_flag();
		std::call_once(OnceFlag, [a_serializer]() {
			REX::LogDebug("{}"sv,
				FUNCTION_NAME);

			a_serializer->SetUniqueID(SERIALIZATION_ID);
			a_serializer->SetSaveCallback(Save);
			a_serializer->SetLoadCallback(Load);
			a_serializer->SetRevertCallback(Revert);
		});
	}

	void Load(const F4SE::SerializationInterface* a_serializer)
	{
		if (!a_serializer) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto stringPool = REX::NotNull(std::make_shared<Internal::Save::SaveStringPool>(STRING_POOL_SAVE_TYPE, STRING_POOL_SAVE_VERSION));
		if (!stringPool->Load(*a_serializer, stringPool.get())) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		const auto savables = Plugin::GetInternalManager()->GetSaveManager()->GetSavableInstances();

		auto remainingSavables = std::vector<REX::NotNull<std::shared_ptr<Plugin::Internal::Save::ISaveObject>>>();
		remainingSavables.reserve(savables.size());
		std::ranges::copy(savables, std::back_inserter(remainingSavables));

		while (!remainingSavables.empty()) {
			for (auto it = remainingSavables.begin(); it != remainingSavables.end(); it++) {
				if ((*it)->Load(*a_serializer, stringPool.get())) {
					remainingSavables.erase(it);
					break;
				}
			}
		}
	}

	void Save(const F4SE::SerializationInterface* a_serializer)
	{
		if (!a_serializer) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto savables = Plugin::GetInternalManager()->GetSaveManager()->GetSavableInstances();

		const auto stringPool = REX::NotNull(std::make_shared<Internal::Save::SaveStringPool>(STRING_POOL_SAVE_TYPE, STRING_POOL_SAVE_VERSION));
		const auto savableCount = static_cast<std::uint32_t>(savables.size());

		auto serializerStreams = std::vector<Internal::Save::SaveStream>();
		serializerStreams.reserve(savableCount);

		for (const auto& savable : savables) {
			auto stream = Internal::Save::SaveStream(a_serializer, stringPool.get());
			if (!savable->Save(stream)) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			serializerStreams.push_back(std::move(stream));
		}

		auto stringStream = Internal::Save::SaveStream(a_serializer, stringPool.get());
		if (!stringPool->Save(stringStream)) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		if (!stringPool->Save(*a_serializer, stringStream)) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		for (auto i = 0ui32; i < savableCount; i++) {
			if (!savables[i]->Save(*a_serializer, serializerStreams[i])) [[unlikely]] {
				REX::Assert(false);
				return;
			}
		}
	}

	void Revert([[maybe_unused]] const F4SE::SerializationInterface* a_serializer)
	{
		if (!a_serializer) [[unlikely]] {
			REX::Assert(false);
			return;
		}

		REX::LogDebug("{}"sv,
			REX::GetCurrentFunctionName());

		const auto& internalManager = Plugin::GetInternalManager();
		internalManager->GetFunctorsManager()->OnSaveRevert();

		const auto savables = internalManager->GetSaveManager()->GetSavableInstances();
		for (const auto& savable : savables) {
			savable->Revert();
		}
	}
}
