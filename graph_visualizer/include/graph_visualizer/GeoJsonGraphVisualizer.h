#pragma once

#include "graph_visualizer/AbstractGraphVisualizer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace graph_visualizer {

class GeoJsonGraphVisualizer : public AbstractGraphVisualizer {
public:
  GeoJsonGraphVisualizer(const std::filesystem::path &output_file = "geojsongraph.json")
      : AbstractGraphVisualizer(output_file) {}

  void parse(const std::vector<common::Segment> &segments) override;

private:
  void addSegment(rapidjson::Document &doc, const common::Segment &segment);
};

} // namespace graph_visualizer
