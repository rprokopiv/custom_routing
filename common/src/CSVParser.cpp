#include "CSVParser.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace common {
CSVParser::CSVParser(const std::filesystem::path &path) { m_path = path; }

void CSVParser::read_data() {
  if (!std::filesystem::exists(m_path)) {
    std::cout << "[ERROR] file " << m_path << " does not exist\n";
    return;
  }
  std::ifstream file(m_path);

  if (!file.is_open()) {
    std::cout << "[ERROR] failed to open " << m_path << "\n";
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    parse_line(line);
  }

  file.close();
}
} // namespace common