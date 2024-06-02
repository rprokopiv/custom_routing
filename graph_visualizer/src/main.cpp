#include "common/Connector.h"
#include "common/Segment.h"
#include "common/SegmentParser.h"
#include "graph_visualizer/GeoJsonGraphVisualizer.h"
#include "map_matcher/MapMatcher.h"
#include "router/CostCalculatorStrategy.h"
#include "router/Router.h"
#include <filesystem>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace common;

int main() {
  // Assuming m_segments and m_connectors are already populated
  UnorderedSegments m_segments;
  UnorderedConnectors m_connectors;

  std::filesystem::path csv_file("./../segment.csv");
  if (!std::filesystem::exists(csv_file)) {
    std::cout << "[ERROR] in main. file " << csv_file << " does not exist\n";
    return 0;
  }
  common::SegmentParser segment_parser(csv_file);
  segment_parser.read_data();
  auto segments = segment_parser.get_segments();

  // GeoPoint startPoint(24.00612781, 49.83432969);
  GeoPoint startPoint(23.999334894, 49.828222887);
  GeoPoint endPoint(24.00206919, 49.83213362);

  router::Router router(segments,
                        std::make_unique<router::DeijkstraCostCalculator>());
  vector<Segment> path = router.findShortestPath(startPoint, endPoint);

  if (!path.empty()) {
    cout << "Found a path:" << endl;
    for (const auto &segment : path) {
      cout << "Segment ID: " << segment.get_id().get_id() << endl;
    }
  } else {
    cout << "No path found." << endl;
  }

  graph_visualizer::GeoJsonGraphVisualizer visualizer("path.json");
  visualizer.parse(path);

  return 0;
}
