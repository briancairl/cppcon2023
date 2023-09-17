#pragma once

// C++ Standard Library
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include <utility>
#include <vector>

// CppCon
#include <cppcon/search.h>

namespace cppcon::demo::v2
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
    const auto itr = adjacencies_.find(q);
    std::for_each(
      itr->second.begin(),
      itr->second.end(),
      [visitor](const auto& parent_and_edge) mutable
      {
        std::apply(visitor, parent_and_edge);
      });
  }

private:
  std::vector<VertexProperties> vertices_;
  std::unordered_map<vertex_id_t, std::vector<Edge>> adjacencies_;
};

}  // namespace cppcon::demo::v2