#pragma once

namespace Plugin::Internal::IO
{
	class IOMutex;
	class IOSandbox;
	class IniFile;
	class JsonFile;
	class TomlFile;
}

namespace Plugin::Internal
{
	class IOManager final
	{
	public:
		IOManager(std::filesystem::path a_rootDirectory);
		~IOManager() noexcept;

		IOManager(const IOManager&) = delete;
		IOManager(IOManager&&) = delete;

		IOManager& operator=(const IOManager&) = delete;
		IOManager& operator=(IOManager&&) = delete;

		[[nodiscard]] auto GetIOMutex() const noexcept { return _ioMutex; }
		[[nodiscard]] auto GetIOSandbox() const noexcept { return _ioSandbox; }
		[[nodiscard]] auto GetIniFile() const noexcept { return _iniFile; }
		[[nodiscard]] auto GetJsonFile() const noexcept { return _jsonFile; }
		[[nodiscard]] auto GetTomlFile() const noexcept { return _tomlFile; }

	private:
		REX::NotNull<std::shared_ptr<IO::IOMutex>> _ioMutex;
		REX::NotNull<std::shared_ptr<IO::IOSandbox>> _ioSandbox;
		REX::NotNull<std::shared_ptr<IO::IniFile>> _iniFile;
		REX::NotNull<std::shared_ptr<IO::JsonFile>> _jsonFile;
		REX::NotNull<std::shared_ptr<IO::TomlFile>> _tomlFile;
	};
}
