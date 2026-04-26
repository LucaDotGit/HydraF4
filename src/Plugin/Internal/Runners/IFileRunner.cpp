#include "Plugin/Internal/Runners/IFileRunner.hpp"

namespace Plugin::Internal::Runners
{
	IFileRunner::IFileRunner(
		std::filesystem::path a_directoryPath,
		std::filesystem::path a_fileExtension)
		: _directoryPath(std::move(a_directoryPath)),
		  _fileExtension(std::move(a_fileExtension))
	{
	}

	IFileRunner::~IFileRunner() noexcept = default;

	auto IFileRunner::LoadDirectoryFiles() const -> std::expected<std::vector<std::filesystem::path>, REX::SystemError>
	{
		constexpr auto SEARCH_RECURSIVELY = true;

		const auto fileNamePattern = REX::Format("*{}"sv, _fileExtension.generic_string());
		return REX::GetFilesInDirectory(_directoryPath, fileNamePattern, SEARCH_RECURSIVELY);
	}
}
