#include "Plugin/Internal/IO/IOMutex.hpp"

namespace Plugin::Internal::IO
{
	IOMutex::SharedIOLock::SharedIOLock(
		REX::NotNull<std::shared_ptr<std::shared_mutex>> a_mutexOwner,
		std::shared_lock<std::shared_mutex>&& a_mutexLock) noexcept
		: mutexOwner(std::move(a_mutexOwner)),
		  mutexLock(std::move(a_mutexLock))
	{
	}

	IOMutex::UniqueIOLock::UniqueIOLock(
		REX::NotNull<std::shared_ptr<std::shared_mutex>> a_mutexOwner,
		std::unique_lock<std::shared_mutex>&& a_mutexLock) noexcept
		: mutexOwner(std::move(a_mutexOwner)),
		  mutexLock(std::move(a_mutexLock))
	{
	}

	IOMutex::SharedIOLock IOMutex::LockShared(const std::filesystem::path& a_path)
	{
		auto mutexOwner = GetOrCreateMutex(a_path);
		auto mutexLock = std::shared_lock(*(mutexOwner.get()));
		return { std::move(mutexOwner), std::move(mutexLock) };
	}

	IOMutex::UniqueIOLock IOMutex::LockUnique(const std::filesystem::path& a_path)
	{
		auto mutexOwner = GetOrCreateMutex(a_path);
		auto mutexLock = std::unique_lock(*(mutexOwner.get()));
		return { std::move(mutexOwner), std::move(mutexLock) };
	}

	auto IOMutex::GetOrCreateMutex(const std::filesystem::path& a_path) -> REX::NotNull<std::shared_ptr<std::shared_mutex>>
	{
		const auto normalizedPath = a_path.lexically_normal();
		const auto mapLock = std::scoped_lock(_mapMutex);

		const auto mutexIt = _mutexMap.find(normalizedPath);
		if (mutexIt != _mutexMap.end()) {
			const auto sharedMutex = mutexIt->second.lock();
			if (sharedMutex) {
				return sharedMutex;
			}
		}

		auto newMutex = REX::NotNull(std::make_shared<std::shared_mutex>());
		_mutexMap.emplace(normalizedPath, newMutex.get());
		return newMutex;
	}
}
