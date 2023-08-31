#pragma once
#include <filesystem>
#include <string>

namespace common {
class CSVParser {
public:
  CSVParser(const std::filesystem::path &path);
  virtual ~CSVParser() = default;
  virtual void read_data() final;

protected:
  virtual void parse_line(std::string &str) = 0;

  std::filesystem::path m_path;
};
} // namespace common