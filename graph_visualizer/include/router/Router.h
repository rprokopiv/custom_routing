#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "common/Connector.h"
#include "common/Segment.h"
#include "common/SegmentParser.h"
#include "graph_visualizer/GeoJsonGraphVisualizer.h"
#include "map_matcher/MapMatcher.h"
#include "router/CostCalculatorStrategy.h"

namespace router {
struct SegmentNode {
  common::Segment segment;
  double cost;

  bool operator>(const SegmentNode &other) const { return cost > other.cost; }
};

class Router {
public:
  Router(const common::UnorderedSegments &segments,
         std::unique_ptr<router::CostCalculatorStrategy> &&cost_calculator);

  std::vector<common::Segment>
  findShortestPath(const common::GeoPoint &startPoint,
                   const common::GeoPoint &endPoint);

  void set_cost_calculator(
      std::unique_ptr<router::CostCalculatorStrategy> &cost_calculator);

private:
  const common::UnorderedSegments &m_segments;
  std::unique_ptr<router::CostCalculatorStrategy> m_cost_calculator;
  std::unordered_map<common::UniqueId, std::vector<common::Segment>,
                     common::UniqueId::UniqueIdHash>
      m_connectorToSegmentsMap;

  void buildConnectorToSegmentsMap();

  std::vector<common::Segment> dijkstra(const common::Segment &startSegment,
                                        const common::Segment &endSegment);

  common::Segment getSegmentById(const common::UniqueId &id);
};

class RouterWithMemoryPool {
public:
  RouterWithMemoryPool(
      const common::UnorderedSegmentsWithMemoryPool &segments,
      std::unique_ptr<router::CostCalculatorStrategy> &&cost_calculator);

  std::vector<common::Segment, boost::pool_allocator<common::Segment>>
  findShortestPath(const common::GeoPoint &startPoint,
                   const common::GeoPoint &endPoint);

  void set_cost_calculator(
      std::unique_ptr<router::CostCalculatorStrategy> &cost_calculator);

private:
  const common::UnorderedSegmentsWithMemoryPool &m_segments;
  std::unique_ptr<router::CostCalculatorStrategy> m_cost_calculator;
  std::unordered_map<common::UniqueId, std::vector<common::Segment>,
                     common::UniqueId::UniqueIdHash>
      m_connectorToSegmentsMap;

  void buildConnectorToSegmentsMap();

  std::vector<common::Segment, boost::pool_allocator<common::Segment>>
  dijkstra(const common::Segment &startSegment,
           const common::Segment &endSegment);

  common::Segment getSegmentById(const common::UniqueId &id);
};
} // namespace router