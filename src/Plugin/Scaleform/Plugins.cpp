#include "Plugin/Scaleform/Plugins.hpp"

#include "Plugin/Internal/Core/ScaleformUtility.hpp"

namespace Plugin::Scaleform::Plugins
{
	class isPluginLoaded final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		isPluginLoaded() = default;
		~isPluginLoaded() override = default;

		isPluginLoaded(const isPluginLoaded&) = delete;
		isPluginLoaded(isPluginLoaded&&) = delete;

		isPluginLoaded& operator=(const isPluginLoaded&) = delete;
		isPluginLoaded& operator=(isPluginLoaded&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto pluginName = ::Plugin::Internal::Core::ScaleformUtility::ToString(a_params.args[0]);
			if (!pluginName.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			const auto* plugin = dataHandler->FindLoadedFileByName(pluginName.value());
			paramResult->SetBoolean(plugin != nullptr);
		}
	};

	class isNthPluginLoaded final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		isNthPluginLoaded() = default;
		~isNthPluginLoaded() override = default;

		isNthPluginLoaded(const isNthPluginLoaded&) = delete;
		isNthPluginLoaded(isNthPluginLoaded&&) = delete;

		isNthPluginLoaded& operator=(const isNthPluginLoaded&) = delete;
		isNthPluginLoaded& operator=(isNthPluginLoaded&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto pluginIndex = ::Plugin::Internal::Core::ScaleformUtility::ToInt(a_params.args[0]);
			if (!pluginIndex.has_value()) {
				paramResult->SetBoolean(false);
				return;
			}

			const auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetBoolean(false);
				return;
			}

			const auto* plugin = dataHandler->FindLoadedFileByIndex(static_cast<std::uint16_t>(pluginIndex.value()));
			paramResult->SetBoolean(plugin != nullptr);
		}
	};

	class getPluginCount final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getPluginCount() = default;
		~getPluginCount() override = default;

		getPluginCount(const getPluginCount&) = delete;
		getPluginCount(getPluginCount&&) = delete;

		getPluginCount& operator=(const getPluginCount&) = delete;
		getPluginCount& operator=(getPluginCount&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 0) {
				paramResult->SetUInt(0);
				return;
			}

			const auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetUInt(0);
				return;
			}

			const auto pluginCount = dataHandler->GetFileCount();
			paramResult->SetUInt(pluginCount);
		}
	};

	class getPluginIndex final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getPluginIndex() = default;
		~getPluginIndex() override = default;

		getPluginIndex(const getPluginIndex&) = delete;
		getPluginIndex(getPluginIndex&&) = delete;

		getPluginIndex& operator=(const getPluginIndex&) = delete;
		getPluginIndex& operator=(getPluginIndex&&) = delete;

		void Call(const Params& a_params) override
		{
			constexpr auto DEFAULT_RESULT = -1i32;

			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetInt(DEFAULT_RESULT);
				return;
			}

			const auto pluginName = ::Plugin::Internal::Core::ScaleformUtility::ToString(a_params.args[0]);
			if (!pluginName.has_value()) {
				paramResult->SetInt(DEFAULT_RESULT);
				return;
			}

			const auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetInt(DEFAULT_RESULT);
				return;
			}

			const auto pluginIndex = dataHandler->FindLoadedFileIndex(pluginName.value());
			if (!pluginIndex.has_value()) {
				paramResult->SetInt(DEFAULT_RESULT);
				return;
			}

			paramResult->SetInt(pluginIndex.value());
		}
	};

	class getNthPluginName final
		: public ::Scaleform::GFx::FunctionHandler
	{
	public:
		getNthPluginName() = default;
		~getNthPluginName() override = default;

		getNthPluginName(const getNthPluginName&) = delete;
		getNthPluginName(getNthPluginName&&) = delete;

		getNthPluginName& operator=(const getNthPluginName&) = delete;
		getNthPluginName& operator=(getNthPluginName&&) = delete;

		void Call(const Params& a_params) override
		{
			auto* paramResult = a_params.returnValue;
			if (!paramResult) [[unlikely]] {
				REX::Assert(false);
				return;
			}

			if (a_params.argCount != 1) {
				paramResult->SetNull();
				return;
			}

			const auto pluginIndex = ::Plugin::Internal::Core::ScaleformUtility::ToInt(a_params.args[0]);
			if (!pluginIndex.has_value()) {
				paramResult->SetNull();
				return;
			}

			const auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetNull();
				return;
			}

			const auto* pluginName = dataHandler->FindLoadedFileByIndex(static_cast<std::uint16_t>(pluginIndex.value()));
			if (!pluginName) {
				paramResult->SetNull();
				return;
			}

			const auto movieRoot = a_params.GetMovieRoot();
			if (!movieRoot) [[unlikely]] {
				REX::Assert(false);
				paramResult->SetNull();
				return;
			}

			auto gfxPluginName = ::Scaleform::GFx::Value();
			movieRoot->CreateString(std::addressof(gfxPluginName), pluginName->filename.data());

			paramResult->operator=(std::move(gfxPluginName));
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

		RE_REGISTER_SF_FUNCTION(a_movie, a_value, isPluginLoaded);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, isNthPluginLoaded);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getPluginCount);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getPluginIndex);
		RE_REGISTER_SF_FUNCTION(a_movie, a_value, getNthPluginName);
	}
}
