#include "Segment.h"

namespace common {
Segment::Segment(const UniqueId &id, const std::vector<UniqueId> &connectors,
                 const std::vector<GeoPoint> points_of_line)
    : m_id(id), m_connectors(connectors), m_points_of_line(points_of_line) {}

UniqueId Segment::get_id() const noexcept { return m_id; }

bool Segment::operator==(const Segment &segment) const {
  return segment.m_id == m_id;
}

std::vector<GeoPoint> Segment::get_points() const noexcept {
  return m_points_of_line;
}
} // namespace common