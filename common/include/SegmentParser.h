#pragma once

#include "CSVParser.h"
#include "Segment.h"

namespace common {
class SegmentParser : public CSVParser {
public:
  SegmentParser(const std::filesystem::path &path);
  UnorderedSegments get_segments() const noexcept;

private:
  void parse_line(std::string &str) override;

  UnorderedSegments m_segments;
};
} // namespace common