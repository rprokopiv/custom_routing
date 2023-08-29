#pragma once

#include "CSVParser.h"

namespace common {
class SegmentParser : public CSVParser {
public:
  SegmentParser(const std::filesystem::path &path);
  std::vector<Segment> get_segments() const noexcept;

private:
  void parse_line(std::string &str) override;

  // TODO: vector is definitely not suitable for this
  // but let's get to it when we need it :)
  std::vector<Segment> m_segments;
};
} // namespace common