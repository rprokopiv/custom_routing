#pragma once

#include "UniqueId.h"
#include "GeoPoint.h"
#include <unordered_set>

namespace common {

class Connector {
public:
  Connector(const UniqueId &unique_id, const GeoPoint &point)
      : m_id(unique_id), m_point(point) {}

  UniqueId get_id() const;
  GeoPoint get_point() const;
  bool operator==(const Connector &connector) const;

  struct ConnectorHash {
    std::size_t operator()(const Connector &connector) const {
      return std::hash<std::string>()(connector.m_id.get_id());
    }
  };

private:
  UniqueId m_id;
  GeoPoint m_point;
};

using UnorderedConnectors = std::unordered_set<Connector, Connector::ConnectorHash>;

} // namespace common
