#include "UniqueId.h"

namespace common {
UniqueId::UniqueId(const std::string &id) { m_id = id; }

std::string UniqueId::get_id() const { return m_id; }

bool UniqueId::operator==(const UniqueId &id) const { return id.m_id == m_id; }
} // namespace common