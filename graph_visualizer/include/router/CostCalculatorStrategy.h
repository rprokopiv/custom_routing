#pragma once
#include "common/GeoPoint.h"
#include "common/Segment.h"

namespace router {

class CostCalculatorStrategy {
public:
  virtual ~CostCalculatorStrategy() = default;
  virtual double cost(const common::Segment &a, const common::Segment &b) = 0;
};

class DeijkstraCostCalculator : public CostCalculatorStrategy {
public:
  DeijkstraCostCalculator() : CostCalculatorStrategy() {}
  virtual ~DeijkstraCostCalculator() = default;
  double cost(const common::Segment &a, const common::Segment &b) override {
    return common::GeoPoint::distance_m(a.get_points()[0], b.get_points()[0]);
  }
};
} // namespace router