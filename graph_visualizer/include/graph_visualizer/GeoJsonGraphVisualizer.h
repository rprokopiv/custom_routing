#pragma once

#include "common/Segment.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <filesystem>

namespace graph_visualizer {

class GeoJsonGraphVisualizer {
public:
  GeoJsonGraphVisualizer(
      const std::filesystem::path &output_file = "geojsongraph.json")
      : m_output_file(output_file) {}

  void parse(const std::vector<common::Segment> &segments);

private:
  void addSegment(rapidjson::Document &doc, const common::Segment &segment);

  std::filesystem::path m_output_file;
};

class GeoJsonGraphVisualizerWithMemoryPool {
public:
  GeoJsonGraphVisualizerWithMemoryPool(
      const std::filesystem::path &output_file = "geojsongraph.json")
      : m_output_file(output_file) {}

  void parse(
      const std::vector<common::Segment, boost::pool_allocator<common::Segment>>
          &segments);

private:
  void addSegment(rapidjson::Document &doc, const common::Segment &segment);

  std::filesystem::path m_output_file;
};

} // namespace graph_visualizer
