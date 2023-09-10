#pragma once 

// C++ Stanrdard Library
#include <filesystem>

// PicoJSON
#include <picojson/picojson.h>

namespace cppcon::demo
{

picojson::value load_json(const std::filesystem::path& path);

}  // namespace cppcon::demo
