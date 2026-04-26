#pragma once

#include "Plugin/Internal/Functors/ISharedDelayFunctor.hpp"

namespace Plugin::Internal::Functors
{
	template <class T>
	ISharedDelayFunctor<T>::ISharedDelayFunctor()
		: _internalVM(RE::GameVM::GetInternalVM())
	{
	}

	template <class T>
	ISharedDelayFunctor<T>::ISharedDelayFunctor(F4SE::SerializationTag /*a_staticTag*/)
		: _internalVM(RE::GameVM::GetInternalVM())
	{
	}

	template <class T>
	ISharedDelayFunctor<T>::~ISharedDelayFunctor() noexcept = default;
}
