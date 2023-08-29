#include "UniqueId.h"

namespace common {
UniqueId::UniqueId(const std::string &id) { m_id = id; }
std::string UniqueId::get_id() const { return m_id; }
} // namespace common