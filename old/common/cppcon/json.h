#pragma once 

// C++ Stanrdard Library
#include <filesystem>
#include <fstream>
#include <stdexcept>

// PicoJSON
#include <picojson/picojson.h>

namespace cppcon
{

inline picojson::value load_json(const std::filesystem::path& path)
{
  std::ifstream ifs{path};
  picojson::value root;
  std::string err;
  picojson::parse(root, std::istream_iterator<char>{ifs}, std::istream_iterator<char>{}, &err);
  if (!err.empty())
  {
    throw std::runtime_error{std::move(err)};
  }
  return root;
}

}  // namespace cppcon
