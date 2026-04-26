#pragma once

// IWYU pragma: begin_exports
#include <SimpleIni.h>
#include <boost/regex.hpp>
#include <ctre-unicode.hpp>
#include <fmt/args.h>
#include <fmt/compile.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <fmt/xchar.h>
#include <nlohmann/json.hpp>
#include <toml.hpp>
// IWYU pragma: end_exports

namespace Plugin
{
	using namespace ctre::literals;
	using namespace fmt::literals;
	using namespace nlohmann::literals;
	using namespace std::literals;
}
