#pragma once

// C++ Standard Library
#include <algorithm>
#include <filesystem>
#include <map>
#include <utility>
#include <vector>

// CppCon
#include <cppcon/dijkstras.h>

namespace cppcon::demo::v0
{

class Graph
{
public:
  explicit Graph(const std::filesystem::path& json);

  std::vector<std::size_t> shuffle();

  const VertexProperties& vertex(vertex_id_t q) const { return vertices_[q]; }

  std::size_t vertex_count() const { return vertices_.size(); }

  template<typename EdgeVisitorT>
  void for_each_edge(vertex_id_t q, EdgeVisitorT&& visitor) const
  {
    const auto [first, last] = adjacencies_.equal_range(q);
    std::for_each(
      first,
      last,
      [visitor](const auto& parent_and_edge) mutable
      {
        std::apply(visitor, parent_and_edge.second);
      });
  }

private:
  std::vector<VertexProperties> vertices_;
  std::multimap<vertex_id_t, std::pair<vertex_id_t, EdgeProperties>> adjacencies_;
};

}  // namespace cppcon::demo::v0