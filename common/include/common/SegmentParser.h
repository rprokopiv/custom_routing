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

class SegmentParserWithMemoryPool : public CSVParser {
public:
  SegmentParserWithMemoryPool(const std::filesystem::path &path);
  UnorderedSegmentsWithMemoryPool get_segments() const noexcept;

private:
  void parse_line(std::string &str) override;

  UnorderedSegmentsWithMemoryPool m_segments;
};

class SegmentParserWithArena : public CSVParser {
public:
  SegmentParserWithArena(const std::filesystem::path &path,
                         UnorderedSegmentsWithArena &segments);
  UnorderedSegmentsWithArena get_segments() const noexcept;

private:
  void parse_line(std::string &str) override;

  UnorderedSegmentsWithArena &m_segments;
};
} // namespace common