#include "Plugin/Internal/IOManager.hpp"

#include "Plugin/Internal/IO/IOMutex.hpp"
#include "Plugin/Internal/IO/IOSandbox.hpp"
#include "Plugin/Internal/IO/IniFile.hpp"
#include "Plugin/Internal/IO/JsonFile.hpp"
#include "Plugin/Internal/IO/TomlFile.hpp"

namespace Plugin::Internal
{
	IOManager::IOManager(std::filesystem::path a_rootDirectory)
		: _ioMutex(std::make_shared<IO::IOMutex>()),
		  _ioSandbox(std::make_shared<IO::IOSandbox>(std::move(a_rootDirectory))),
		  _iniFile(std::make_shared<IO::IniFile>(_ioMutex)),
		  _jsonFile(std::make_shared<IO::JsonFile>(_ioMutex)),
		  _tomlFile(std::make_shared<IO::TomlFile>(_ioMutex))
	{
	}

	IOManager::~IOManager() noexcept = default;
}
