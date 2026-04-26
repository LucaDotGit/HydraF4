#pragma once

namespace Plugin::Internal::Runners
{
	class IFileRunner
	{
	public:
		IFileRunner(
			std::filesystem::path a_directoryPath,
			std::filesystem::path a_fileExtension);

		virtual ~IFileRunner() noexcept;

		IFileRunner(const IFileRunner&) = default;
		IFileRunner(IFileRunner&&) noexcept = default;

		IFileRunner& operator=(const IFileRunner&) = default;
		IFileRunner& operator=(IFileRunner&&) noexcept = default;

		[[nodiscard]] const std::filesystem::path& GetDirectoryPath() const noexcept { return _directoryPath; }
		[[nodiscard]] const std::filesystem::path& GetFileExtension() const noexcept { return _fileExtension; }

		[[nodiscard]] virtual auto Setup(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> = 0;
		[[nodiscard]] virtual auto Run(REX::ThreadPool& a_threadPool) -> std::vector<std::future<void>> = 0;
		virtual void Dispose() noexcept = 0;

		[[nodiscard]] auto LoadDirectoryFiles() const -> std::expected<std::vector<std::filesystem::path>, REX::SystemError>;

	protected:
		using json_t = nlohmann::json;

		std::filesystem::path _directoryPath;
		std::filesystem::path _fileExtension;
	};
}
