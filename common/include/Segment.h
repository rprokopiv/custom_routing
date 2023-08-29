#pragma once
#include "GeoPoint.h"
#include "UniqueId.h"

namespace common {
class Segment {
public:
  Segment(const UniqueId &id, const std::vector<UniqueId> &connectors,
          const std::vector<GeoPoint> points_of_line);
  UniqueId get_id() const noexcept;
  std::vector<GeoPoint> get_points() const noexcept;

private:
  UniqueId m_id;
  // TODO: consider better container for connectors here
  std::vector<UniqueId> m_connectors;
  std::vector<GeoPoint> m_points_of_line;
};
} // namespace common