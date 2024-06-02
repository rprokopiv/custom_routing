#pragma once

#include <string>

namespace common {

class UniqueId {
public:
  UniqueId(const std::string &id) : m_id(id) {}
  std::string get_id() const { return m_id; }
  bool operator==(const UniqueId &other) const { return m_id == other.m_id; }
  bool operator!=(const UniqueId &other) const { return m_id != other.m_id; }

  struct UniqueIdHash {
    std::size_t operator()(const UniqueId &unique_id) const {
      return std::hash<std::string>()(unique_id.get_id());
    }
  };

private:
  std::string m_id;
};

} // namespace common
