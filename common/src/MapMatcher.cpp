#include "map_matcher/MapMatcher.h"
#include <limits>

namespace common {
const double ACCEPTABLE_DISTANCE = 5.0;

MapMatcher::MapMatcher(const UnorderedSegments &segments)
    : m_segments(segments) {}

Segment MapMatcher::match(const GeoPoint &point) {
  Segment closest_segment = *m_segments.begin();
  double min_distance = std::numeric_limits<double>::max();

  for (const auto &segment : m_segments) {
      double distance = GeoPoint::distance_m(point, segment.get_points()[0]);
      if (distance < min_distance) {
        min_distance = distance;
        closest_segment = segment;
      }
      if( min_distance < ACCEPTABLE_DISTANCE )
      {
        break;
      }
  }

  return closest_segment;
}

} // namespace common
