#include "common/SegmentParser.h"
#include <experimental/memory_resource>
#include <sstream>
#include <unordered_set>

namespace common {
SegmentParser::SegmentParser(const std::filesystem::path &path)
    : CSVParser(path) {}

void SegmentParser::parse_line(std::string &str) {
  // Parse the line
  std::istringstream iss(str);
  std::string segmentId;
  std::string connectorsStr;
  std::string pointsStr;

  if (!std::getline(iss, segmentId, '|') ||
      !std::getline(iss, connectorsStr, '|') || !std::getline(iss, pointsStr)) {
    throw std::runtime_error("Malformed input line: " + str);
  }

  // Extract connector IDs (parsing as JSON-like structure)
  std::vector<UniqueId> connectors;
  if (connectorsStr.front() == '[' && connectorsStr.back() == ']') {
    connectorsStr = connectorsStr.substr(1, connectorsStr.size() - 2);
    std::istringstream connectorsStream(connectorsStr);
    std::string connectorEntry;
    while (std::getline(connectorsStream, connectorEntry, '}')) {
      auto idStart = connectorEntry.find("connector_id':");
      if (idStart != std::string::npos) {
        idStart += 14; // Length of "connector_id':"
        idStart = connectorEntry.find_first_not_of(' ', idStart);
        size_t idEnd = connectorEntry.find(',', idStart);
        if (idEnd == std::string::npos)
          idEnd = connectorEntry.size();
        connectors.push_back(connectorEntry.substr(idStart, idEnd - idStart));
        /*std::cout << "|" << connectors.back().get_id()
                  << "|size = " << connectors.back().get_id().size()
                  << std::endl;*/
      }
    }
  }

  // Extract GeoPoints
  std::vector<GeoPoint> points;
  size_t start = pointsStr.find_first_of('(') + 1;
  size_t end = pointsStr.find_last_of(')');
  std::string pointsData = pointsStr.substr(start, end - start);
  std::istringstream pointsStream(pointsData);
  double longitude;
  double latitude;
  char comma;
  while (pointsStream >> longitude >> latitude >> comma) {
    points.push_back(GeoPoint(longitude, latitude));
  }
  points.push_back(GeoPoint(longitude, latitude));
  // Create the Segment object
  m_segments.emplace(Segment(segmentId, connectors, points));
}

UnorderedSegments SegmentParser::get_segments() const noexcept {
  return m_segments;
}

SegmentParserWithMemoryPool::SegmentParserWithMemoryPool(
    const std::filesystem::path &path)
    : CSVParser(path) {}

void SegmentParserWithMemoryPool::parse_line(std::string &str) {
  // Parse the line
  std::istringstream iss(str);
  std::string segmentId;
  std::string connectorsStr;
  std::string pointsStr;

  if (!std::getline(iss, segmentId, '|') ||
      !std::getline(iss, connectorsStr, '|') || !std::getline(iss, pointsStr)) {
    throw std::runtime_error("Malformed input line: " + str);
  }

  // Extract connector IDs (parsing as JSON-like structure)
  std::vector<UniqueId> connectors;
  if (connectorsStr.front() == '[' && connectorsStr.back() == ']') {
    connectorsStr = connectorsStr.substr(1, connectorsStr.size() - 2);
    std::istringstream connectorsStream(connectorsStr);
    std::string connectorEntry;
    while (std::getline(connectorsStream, connectorEntry, '}')) {
      auto idStart = connectorEntry.find("connector_id':");
      if (idStart != std::string::npos) {
        idStart += 14; // Length of "connector_id':"
        idStart = connectorEntry.find_first_not_of(' ', idStart);
        size_t idEnd = connectorEntry.find(',', idStart);
        if (idEnd == std::string::npos)
          idEnd = connectorEntry.size();
        connectors.push_back(connectorEntry.substr(idStart, idEnd - idStart));
        /*std::cout << "|" << connectors.back().get_id()
                  << "|size = " << connectors.back().get_id().size()
                  << std::endl;*/
      }
    }
  }

  // Extract GeoPoints
  std::vector<GeoPoint> points;
  size_t start = pointsStr.find_first_of('(') + 1;
  size_t end = pointsStr.find_last_of(')');
  std::string pointsData = pointsStr.substr(start, end - start);
  std::istringstream pointsStream(pointsData);
  double longitude;
  double latitude;
  char comma;
  while (pointsStream >> longitude >> latitude >> comma) {
    points.push_back(GeoPoint(longitude, latitude));
  }
  points.push_back(GeoPoint(longitude, latitude));
  // Create the Segment object
  m_segments.emplace(Segment(segmentId, connectors, points));
}

UnorderedSegmentsWithMemoryPool
SegmentParserWithMemoryPool::get_segments() const noexcept {
  return m_segments;
}

SegmentParserWithArena::SegmentParserWithArena(
    const std::filesystem::path &path, UnorderedSegmentsWithArena &segments)
    : CSVParser(path), m_segments(segments) {}

void SegmentParserWithArena::parse_line(std::string &str) {
  // Parse the line
  std::istringstream iss(str);
  std::string segmentId;
  std::string connectorsStr;
  std::string pointsStr;

  if (!std::getline(iss, segmentId, '|') ||
      !std::getline(iss, connectorsStr, '|') || !std::getline(iss, pointsStr)) {
    throw std::runtime_error("Malformed input line: " + str);
  }

  // Extract connector IDs (parsing as JSON-like structure)
  std::vector<UniqueId> connectors;
  if (connectorsStr.front() == '[' && connectorsStr.back() == ']') {
    connectorsStr = connectorsStr.substr(1, connectorsStr.size() - 2);
    std::istringstream connectorsStream(connectorsStr);
    std::string connectorEntry;
    while (std::getline(connectorsStream, connectorEntry, '}')) {
      auto idStart = connectorEntry.find("connector_id':");
      if (idStart != std::string::npos) {
        idStart += 14; // Length of "connector_id':"
        idStart = connectorEntry.find_first_not_of(' ', idStart);
        size_t idEnd = connectorEntry.find(',', idStart);
        if (idEnd == std::string::npos)
          idEnd = connectorEntry.size();
        connectors.push_back(connectorEntry.substr(idStart, idEnd - idStart));
        /*std::cout << "|" << connectors.back().get_id()
                  << "|size = " << connectors.back().get_id().size()
                  << std::endl;*/
      }
    }
  }

  // Extract GeoPoints
  std::vector<GeoPoint> points;
  size_t start = pointsStr.find_first_of('(') + 1;
  size_t end = pointsStr.find_last_of(')');
  std::string pointsData = pointsStr.substr(start, end - start);
  std::istringstream pointsStream(pointsData);
  double longitude;
  double latitude;
  char comma;
  while (pointsStream >> longitude >> latitude >> comma) {
    points.push_back(GeoPoint(longitude, latitude));
  }
  points.push_back(GeoPoint(longitude, latitude));
  // Create the Segment object
  m_segments.emplace(Segment(segmentId, connectors, points));
}

UnorderedSegmentsWithArena
SegmentParserWithArena::get_segments() const noexcept {
  return m_segments;
}
} // namespace common