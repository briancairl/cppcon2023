#pragma once

// C++ Standard Library
#include <algorithm>
#include <filesystem>
#include <ranges>
#include <utility>
#include <vector>

// CppCon
#include <cppcon/search.h>

namespace cppcon::demo::a3
{

class Graph
{
public:
  explicit Graph(const std::filesystem::path& json);

  void shuffle(const std::vector<std::size_t>& indices);

  const VertexProperties& vertex(vertex_id_t q) const { return vertices_[q]; }

  std::size_t vertex_count() const { return vertices_.size(); }

  template<typename EdgeVisitorT>
  void for_each_edge(vertex_id_t q, EdgeVisitorT&& visitor) const
  {
    std::for_each(
      adjacencies_[q].begin(),
      adjacencies_[q].end(),
      [q, visitor](const auto& child_and_edge_weight) mutable
      {
        const auto& [succ, edge_weight] = child_and_edge_weight;
        visitor(succ, edge_weight);
      });
  }

private:
  std::vector<VertexProperties> vertices_;
  std::vector<std::ranges::subrange<const Edge*>> adjacencies_;
  std::vector<Edge> edges_;
};

}  // namespace cppcon::demo::a3