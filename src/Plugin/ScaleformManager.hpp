#pragma once

namespace Plugin::ScaleformManager
{
	inline constexpr auto OBJECT_NAME = "hydra"sv;

	void Setup(REX::NotNull<const F4SE::ScaleformInterface*> a_scaleform);
	bool Callback(::Scaleform::GFx::Movie* a_movie, ::Scaleform::GFx::Value* a_value);
}
