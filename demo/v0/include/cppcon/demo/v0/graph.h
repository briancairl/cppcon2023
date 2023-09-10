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
        const auto& [succ, weight] = parent_and_edge.second;
        visitor(succ, weight);
      });
  }

private:
  std::vector<VertexProperties> vertices_;
  std::multimap<vertex_id_t, std::pair<vertex_id_t, edge_weight_t>> adjacencies_;
};

}  // namespace cppcon::demo::v0