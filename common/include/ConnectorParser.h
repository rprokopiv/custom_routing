#pragma once

#include "CSVParser.h"
#include "Connector.h"
#include <vector>

namespace common {
class ConnectorParser : public CSVParser {
public:
  ConnectorParser(const std::filesystem::path &path);

private:
  void parse_line(std::string &str) override;

  // TODO: vector is definitely not suitable for this
  // but let's get to it when we need it :)
  std::vector<Connector> m_connectors;
};
} // namespace common