#include "ConnectorParser.h"
#include <sstream>

namespace common {
ConnectorParser::ConnectorParser(const std::filesystem::path &path)
    : CSVParser(path) {}

void ConnectorParser::parse_line(std::string &str) {
  std::istringstream iss(str);
  // TODO: do we need connectorType?
  std::string connectorType, uniqueId, pointStr;

  // Ignore the first field
  std::getline(iss, connectorType, '|');

  // Read the rest of the fields
  std::getline(iss, uniqueId, '|');
  std::getline(iss, pointStr);

  double x, y;
  // TODO: maybe use smth better to parse coordinates
  if (std::sscanf(pointStr.c_str(), "POINT (%lf %lf)", &x, &y) == 2) {
    UniqueId id(uniqueId);
    GeoPoint point(x, y);

    m_connectors.emplace_back(Connector(id, point));
  }
}

} // namespace common