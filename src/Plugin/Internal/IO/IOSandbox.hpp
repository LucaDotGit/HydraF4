#pragma once

namespace Plugin::Internal::IO
{
	class IOSandbox final
	{
	public:
		IOSandbox(std::filesystem::path a_rootDirectory);
		~IOSandbox() noexcept;

		IOSandbox(const IOSandbox&) = delete;
		IOSandbox(IOSandbox&&) = delete;

		IOSandbox& operator=(const IOSandbox&) = delete;
		IOSandbox& operator=(IOSandbox&&) = delete;

		[[nodiscard]] const auto& GetRootDirectory() const noexcept { return _rootDirectory; }

		[[nodiscard]] bool IsInSandbox(const std::filesystem::path& a_path) const;

	private:
		std::filesystem::path _rootDirectory;
	};
}
