#pragma once

#include <string>

namespace common {
class UniqueId {
public:
  UniqueId(const std::string &id);

  std::string get_id() const;
  bool operator==(const UniqueId &id) const;

private:
  std::string m_id;
};
} // namespace common