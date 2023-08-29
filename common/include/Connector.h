#pragma once
#include "GeoPoint.h"
#include "UniqueId.h"

namespace common {
class Connector {
public:
  Connector(const UniqueId &unique_id, const GeoPoint &point)
      : m_id(unique_id), m_point(point) {}

  UniqueId get_id() const { return m_id; }
  GeoPoint get_point() const { return m_point; }

private:
  UniqueId m_id;
  GeoPoint m_point;
};
} // namespace common