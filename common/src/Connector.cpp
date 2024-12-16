#include "common/Connector.h"

namespace common {

bool Connector::operator==(const Connector &connector) const {
  return m_id == connector.m_id && m_point.latitude == connector.m_point.latitude && m_point.longitude == connector.m_point.longitude;
}

  UniqueId Connector::get_id() const { return m_id; }
  GeoPoint Connector::get_point() const { return m_point; }

} // namespace common
