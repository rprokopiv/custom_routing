#include "router/Router.h"

namespace router {

Router::Router(
    const common::UnorderedSegments &segments,
    std::unique_ptr<router::CostCalculatorStrategy> &&cost_calculator)
    : m_segments(segments), m_cost_calculator(std::move(cost_calculator)) {
  buildConnectorToSegmentsMap();
}

std::vector<common::Segment>
Router::findShortestPath(const common::GeoPoint &startPoint,
                         const common::GeoPoint &endPoint) {
  common::MapMatcher matcher(m_segments);
  common::Segment startSegment = matcher.match(startPoint);
  common::Segment endSegment = matcher.match(endPoint);

  return dijkstra(startSegment, endSegment);
}

void Router::set_cost_calculator(
    std::unique_ptr<router::CostCalculatorStrategy> &cost_calculator) {
  m_cost_calculator = std::move(cost_calculator);
}

void Router::buildConnectorToSegmentsMap() {
  for (const auto &segment : m_segments) {
    for (const auto &connectorId : segment.get_connectors()) {
      m_connectorToSegmentsMap[connectorId].push_back(segment);
    }
  }
}

std::vector<common::Segment>
Router::dijkstra(const common::Segment &startSegment,
                 const common::Segment &endSegment) {
  if (!m_cost_calculator) {
    return {};
  }
  std::unordered_map<common::UniqueId, double, common::UniqueId::UniqueIdHash>
      distances;
  std::unordered_map<common::UniqueId, common::UniqueId,
                     common::UniqueId::UniqueIdHash>
      previous;
  std::priority_queue<SegmentNode, std::vector<SegmentNode>,
                      std::greater<SegmentNode>>
      pq;
  std::unordered_set<common::UniqueId, common::UniqueId::UniqueIdHash> visited;
  std::vector<common::Segment> segments_checked;

  for (const auto &segment : m_segments) {
    distances.emplace(segment.get_id(), std::numeric_limits<double>::max());
  }

  distances[startSegment.get_id()] = 0.0;
  pq.push({startSegment, 0.0});

  while (!pq.empty()) {
    SegmentNode current = pq.top();
    pq.pop();
    if (visited.find(current.segment.get_id()) != visited.end()) {
      continue;
    }

    visited.insert(current.segment.get_id());

    if (current.segment.get_id() == endSegment.get_id()) {
      break;
    }

    for (const auto &connectorId : current.segment.get_connectors()) {
      for (const auto &neighbor : m_connectorToSegmentsMap[connectorId]) {
        if (visited.find(neighbor.get_id()) == visited.end()) {
          segments_checked.emplace_back(neighbor);
          double newDist = distances[current.segment.get_id()] +
                           m_cost_calculator->cost(endSegment, neighbor);
          if (newDist < distances[neighbor.get_id()]) {
            distances[neighbor.get_id()] = newDist;
          }
          auto result =
              previous.insert({neighbor.get_id(), current.segment.get_id()});
          if (!result.second) { // Key already exists
            result.first->second = current.segment.get_id();
          }
          pq.push({neighbor, newDist});
        }
      }
    }
  }
  graph_visualizer::GeoJsonGraphVisualizer visualizer("checked_segments.json");
  visualizer.parse(segments_checked);

  std::vector<common::Segment> path;
  common::UniqueId currentId = endSegment.get_id();
  while (currentId != startSegment.get_id()) {
    try {
      path.push_back(getSegmentById(currentId));
      currentId = previous.at(currentId); // Use 'at' instead of 'operator[]' to
                                          // avoid default constructor issue
    } catch (const std::out_of_range &e) {
      std::cerr << "Error: Unable to find previous segment for ID: "
                << currentId.get_id() << std::endl;
      return {};
    }
  }
  path.push_back(startSegment);
  reverse(path.begin(), path.end());
  return path;
}

common::Segment Router::getSegmentById(const common::UniqueId &id) {
  for (const auto &segment : m_segments) {
    if (segment.get_id() == id) {
      return segment;
    }
  }
  throw std::runtime_error("Segment with given ID not found.");
}

} // namespace router