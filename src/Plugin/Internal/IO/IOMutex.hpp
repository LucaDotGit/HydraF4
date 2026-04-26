#pragma once

namespace Plugin::Internal::IO
{
	class IOMutex final
	{
	public:
		class SharedIOLock final
		{
		public:
			SharedIOLock(
				REX::NotNull<std::shared_ptr<std::shared_mutex>> a_mutexOwner,
				std::shared_lock<std::shared_mutex>&& a_mutexLock) noexcept;

			~SharedIOLock() noexcept = default;

			SharedIOLock(const SharedIOLock&) = delete;
			SharedIOLock(SharedIOLock&&) noexcept = default;

			SharedIOLock& operator=(const SharedIOLock&) = delete;
			SharedIOLock& operator=(SharedIOLock&&) noexcept = default;

			REX::NotNull<std::shared_ptr<std::shared_mutex>> mutexOwner;
			std::shared_lock<std::shared_mutex> mutexLock;
		};

		class UniqueIOLock final
		{
		public:
			UniqueIOLock(
				REX::NotNull<std::shared_ptr<std::shared_mutex>> a_mutexOwner,
				std::unique_lock<std::shared_mutex>&& a_mutexLock) noexcept;

			~UniqueIOLock() noexcept = default;

			UniqueIOLock(const UniqueIOLock&) = delete;
			UniqueIOLock(UniqueIOLock&&) noexcept = default;

			UniqueIOLock& operator=(const UniqueIOLock&) = delete;
			UniqueIOLock& operator=(UniqueIOLock&&) noexcept = default;

			REX::NotNull<std::shared_ptr<std::shared_mutex>> mutexOwner;
			std::unique_lock<std::shared_mutex> mutexLock;
		};

		IOMutex() = default;
		~IOMutex() noexcept = default;

		IOMutex(const IOMutex&) = delete;
		IOMutex(IOMutex&&) = delete;

		IOMutex& operator=(const IOMutex&) = delete;
		IOMutex& operator=(IOMutex&&) = delete;

		[[nodiscard]] SharedIOLock LockShared(const std::filesystem::path& a_path);
		[[nodiscard]] UniqueIOLock LockUnique(const std::filesystem::path& a_path);

	private:
		[[nodiscard]] auto GetOrCreateMutex(const std::filesystem::path& a_path) -> REX::NotNull<std::shared_ptr<std::shared_mutex>>;

		mutable std::shared_mutex _mapMutex;
		std::unordered_map<std::filesystem::path, std::weak_ptr<std::shared_mutex>> _mutexMap;
	};
}
