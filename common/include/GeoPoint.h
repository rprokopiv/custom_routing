#pragma once

#include <algorithm>
#include <iostream>
#include <math.h>

constexpr double PI = 3.14159265358979323846;
constexpr double RADIUS = 6372797.56085;
constexpr double GRADOS_RADIANES = PI / 180;

#include <vector>

namespace common {
struct GeoPoint {
  double latitude;  // широта
  double longitude; // довгота
  GeoPoint(double longitude, double latitude)
      : latitude(latitude), longitude(longitude) {}
  // calcualtes distance between two points
  static double distance_m(const GeoPoint &a, const GeoPoint &b) {
    double haversine;
    double distance;

    double latitude1 = a.latitude * GRADOS_RADIANES;
    double longitude1 = a.longitude * GRADOS_RADIANES;
    double latitude2 = b.latitude * GRADOS_RADIANES;
    double longitude2 = b.longitude * GRADOS_RADIANES;

    haversine = (pow(sin((1.0 / 2) * (latitude2 - latitude1)), 2)) +
                ((cos(latitude1)) * (cos(latitude2)) *
                 (pow(sin((1.0 / 2) * (longitude2 - longitude1)), 2)));

    distance = RADIUS * 2 * asin(std::min(1.0, sqrt(haversine)));

    return distance;
  }

  // calculates distance in meters for the given vector of poits.
  // First point to be used is under index 0 and goes incrementally to the end
  static double distance_m(const std::vector<GeoPoint> &points) {
    if (points.empty() || points.size() == 1) {
      std::cout << "[WARN] no points to calculate distance\n";
      return 0;
    }
    double distance = 0;
    for (size_t i = 0; i < points.size() - 1; ++i) {
      distance += distance_m(points[i], points[i + 1]);
    }
    return distance;
  }
};
} // namespace common