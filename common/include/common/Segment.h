#pragma once

#include "GeoPoint.h"
#include "UniqueId.h"

#include <boost/pool/pool_alloc.hpp>
#include <unordered_set>
#include <vector>

namespace common {

class Segment {
public:
  Segment(const UniqueId &id, const std::vector<UniqueId> &connectors,
          const std::vector<GeoPoint> &points_of_line);
  UniqueId get_id() const noexcept;
  std::vector<GeoPoint> get_points() const noexcept;
  std::vector<UniqueId> get_connectors() const noexcept;
  bool operator==(const Segment &segment) const;

  struct SegmentHash {
    std::size_t operator()(const Segment &segment) const {
      return std::hash<std::string>()(segment.m_id.get_id());
    }
  };

private:
  UniqueId m_id;
  std::vector<UniqueId> m_connectors;
  std::vector<GeoPoint> m_points_of_line;
};
template <typename T> using ArenaAllocator = boost::pool_allocator<T>;

using UnorderedSegments = std::unordered_set<Segment, Segment::SegmentHash>;
using UnorderedSegmentsWithMemoryPool =
    std::unordered_set<Segment, Segment::SegmentHash,
                       std::equal_to<common::Segment>,
                       boost::pool_allocator<Segment>>;
using UnorderedSegmentsWithArena =
    std::unordered_set<Segment, Segment::SegmentHash,
                       std::equal_to<common::Segment>, ArenaAllocator<Segment>>;

} // namespace common
