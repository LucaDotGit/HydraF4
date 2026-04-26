#pragma once

namespace Plugin::Internal::Game
{
	class IGameSettingCollection
	{
	public:
		IGameSettingCollection() = default;
		virtual ~IGameSettingCollection() = default;

		IGameSettingCollection(const IGameSettingCollection&) = delete;
		IGameSettingCollection(IGameSettingCollection&&) = delete;

		IGameSettingCollection& operator=(const IGameSettingCollection&) = delete;
		IGameSettingCollection& operator=(IGameSettingCollection&&) = delete;

		virtual void OnGameDataReady() = 0;
	};
}
