#include "Plugin/Internal/IO/IOSandbox.hpp"

namespace Plugin::Internal::IO
{
	IOSandbox::IOSandbox(std::filesystem::path a_rootDirectory)
		: _rootDirectory(std::move(a_rootDirectory))
	{
		REX::Assert(_rootDirectory.is_absolute());
	}

	IOSandbox::~IOSandbox() noexcept = default;

	bool IOSandbox::IsInSandbox(const std::filesystem::path& a_path) const
	{
		const auto combinedPath = _rootDirectory / a_path;
		const auto resolvedPath = combinedPath.lexically_normal();
		return std::ranges::starts_with(resolvedPath, _rootDirectory);
	}
}
