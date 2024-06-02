#pragma once

#include "common/GeoPoint.h"
#include "common/Segment.h"
#include <unordered_set>

namespace common {

class MapMatcher {
public:
  MapMatcher(const UnorderedSegments &segments);
  virtual ~MapMatcher() = default;

  Segment match(const GeoPoint &point);

private:
  const UnorderedSegments &m_segments;
};

} // namespace common
