#include "SegmentParser.h"
#include <sstream>

namespace common {
SegmentParser::SegmentParser(const std::filesystem::path &path)
    : CSVParser(path) {}

void SegmentParser::parse_line(std::string &str) {
  // Parse the line
  std::istringstream iss(str);
  std::string segmentId;
  std::string connectorsStr;
  std::string pointsStr;

  std::getline(iss, segmentId, '|');
  std::getline(iss, connectorsStr, '|');
  std::getline(iss, pointsStr);

  // Extract connector IDs
  std::vector<UniqueId> connectors;
  std::istringstream connectorsStream(connectorsStr);
  std::string connector;
  while (std::getline(connectorsStream, connector, ',')) {
    connectors.push_back(connector);
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
  m_segments.emplace_back(Segment(segmentId, connectors, points));
}

std::vector<Segment> SegmentParser::get_segments() const noexcept {
  return m_segments;
}

} // namespace common