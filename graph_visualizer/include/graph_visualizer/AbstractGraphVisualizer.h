#pragma once

#include "common/GeoPoint.h"
#include "common/Segment.h"
#include <filesystem>
#include <map>
#include <vector>

namespace graph_visualizer {

class AbstractGraphVisualizer {
public:
  AbstractGraphVisualizer(const std::filesystem::path &output_file)
      : m_output_file(output_file) {}
  virtual void parse(const std::vector<common::Segment> &segments) = 0;

protected:
  std::filesystem::path m_output_file;
};

} // namespace graph_visualizer
