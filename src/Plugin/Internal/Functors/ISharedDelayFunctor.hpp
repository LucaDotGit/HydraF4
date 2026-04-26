#pragma once

namespace Plugin::Internal::Functors
{
	inline constexpr auto DELAY_FUNCTOR_FACTORY_PREFIX = REX::StaticString("Hydra");

	template <class T>
	class ISharedDelayFunctor
		: public F4SE::IDelayFunctor,
		  public std::enable_shared_from_this<T>
	{
	public:
		explicit ISharedDelayFunctor();
		explicit ISharedDelayFunctor(F4SE::SerializationTag a_serializerTag);

		~ISharedDelayFunctor() noexcept override;

		ISharedDelayFunctor(const ISharedDelayFunctor&) = delete;
		ISharedDelayFunctor(ISharedDelayFunctor&&) = delete;

		ISharedDelayFunctor& operator=(const ISharedDelayFunctor&) = delete;
		ISharedDelayFunctor& operator=(ISharedDelayFunctor&&) = delete;

	protected:
		REX::NotNull<RE::BSTSmartPointer<RE::BSScript::Internal::VirtualMachine>> _internalVM;
	};
}

#include "Plugin/Internal/Functors/ISharedDelayFunctor.inl"
