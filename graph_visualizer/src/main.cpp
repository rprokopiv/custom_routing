#include "common/Connector.h"
#include "common/Segment.h"
#include "common/SegmentParser.h"
#include "graph_visualizer/GeoJsonGraphVisualizer.h"
#include "map_matcher/MapMatcher.h"
#include "router/CostCalculatorStrategy.h"
#include "router/Router.h"
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/simple_segregated_storage.hpp>
#include <chrono>
#include <experimental/memory_resource>
#include <filesystem>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace common;

//  GeoPoint startPoint(4.196777, 48.56025);
//  GeoPoint endPoint(2.043457, 48.611122);
//
static const common::GeoPoint right_from_Paris(4.196777, 48.56025);
static const common::GeoPoint left_down_from_Paris(2.043457, 48.611122);
static const common::GeoPoint Orleans(1.873169, 47.897931);
static const common::GeoPoint Tours(0.686646, 47.383437);
static const common::GeoPoint Bourges(2.39502, 47.073863);
static const common::GeoPoint Clermont_Ferrand(3.092651, 45.775168);
static const common::GeoPoint Dijon(5.042725, 47.316183);
static const std::vector<std::pair<common::GeoPoint, common::GeoPoint>> routes{
    {right_from_Paris, left_down_from_Paris},
    {Orleans, Tours},
    {Tours, Bourges},
    {Bourges, Clermont_Ferrand},
    {Clermont_Ferrand, Dijon},
    {Dijon, Tours},
    {Dijon, left_down_from_Paris},
    {Dijon, Orleans},
    {Orleans, right_from_Paris},
    {Tours, Clermont_Ferrand}};

enum class MemoryType { DEFAULT, MEMORY_POOL, MEMORY_ARENA };

void default_memory_calculation() {
  // Assuming m_segments and m_connectors are already populated
  auto start_method = std::chrono::high_resolution_clock::now();
  UnorderedSegments m_segments;

  std::filesystem::path csv_file("./../france_roads_reduced.csv");
  if (!std::filesystem::exists(csv_file)) {
    std::cout << "[ERROR] in main. file " << csv_file << " does not exist\n";
    return;
  }
  common::SegmentParser segment_parser(csv_file);
  segment_parser.read_data();
  auto segments = segment_parser.get_segments();
  auto data_preparation = std::chrono::high_resolution_clock::now();
  auto time_for_data_preparation =
      std::chrono::duration_cast<std::chrono::milliseconds>(data_preparation -
                                                            start_method);
  std::cout << "Execution time of data preparation with default memory: "
            << time_for_data_preparation.count() << " milliseconds"
            << std::endl;
  router::Router router(segments,
                        std::make_unique<router::DeijkstraCostCalculator>());
  for (size_t i = 0; i < routes.size(); ++i) {
    auto start_route_calc = std::chrono::high_resolution_clock::now();
    vector<Segment> path =
        router.findShortestPath(routes[i].first, routes[i].second);

    if (!path.empty()) {
      cout << "Found a path:" << endl;
    } else {
      cout << "No path found." << endl;
    }
    std::stringstream ss;
    ss << "_default_mem_path_" << i << ".json";
    graph_visualizer::GeoJsonGraphVisualizer visualizer(ss.str());
    visualizer.parse(path);
    auto end_route_calc = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_route_calc - start_route_calc);
    std::cout
        << "Execution time of calculaitons with default memory of route id = "
        << i << " time =" << duration.count() << " milliseconds" << std::endl;
  }
  auto end_method = std::chrono::high_resolution_clock::now();

  // Calculate the duration
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_method - start_method);

  std::cout << "Execution time of calculaitons with default memory: "
            << duration.count() << " milliseconds" << std::endl;
}

void memory_arena_calculation() {
  // Assuming m_segments and m_connectors are already populated
  UnorderedSegments m_segments;
  auto start_method = std::chrono::high_resolution_clock::now();
  std::filesystem::path csv_file("./../france_roads_reduced.csv");
  if (!std::filesystem::exists(csv_file)) {
    std::cout << "[ERROR] in main. file " << csv_file << " does not exist\n";
    return;
  }
  constexpr std::size_t segment_size = sizeof(Segment);
  boost::pool<> segment_pool(segment_size);
  constexpr std::size_t preallocate_count = 5082000; // Number of elements
  for (std::size_t i = 0; i < preallocate_count; ++i) {
    segment_pool.ordered_malloc();
  }

  // Create an unordered_set using the pool_allocator
  UnorderedSegmentsWithArena segments;
  common::SegmentParserWithArena segment_parser(csv_file, segments);
  segment_parser.read_data();
  auto data_preparation = std::chrono::high_resolution_clock::now();
  auto time_for_data_preparation =
      std::chrono::duration_cast<std::chrono::milliseconds>(data_preparation -
                                                            start_method);
  std::cout << "Execution time of data preparation with Arena memory: "
            << time_for_data_preparation.count() << " milliseconds"
            << std::endl;
  router::RouterWithMemoryPool router(
      segments, std::make_unique<router::DeijkstraCostCalculator>());
  for (size_t i = 0; i < routes.size(); ++i) {
    auto start_route_calc = std::chrono::high_resolution_clock::now();
    vector<Segment, boost::pool_allocator<common::Segment>> path =
        router.findShortestPath(routes[i].first, routes[i].second);

    if (!path.empty()) {
      cout << "Found a path:" << endl;
    } else {
      cout << "No path found." << endl;
    }
    std::stringstream ss;
    ss << "arena_mem_path_" << i << ".json";
    graph_visualizer::GeoJsonGraphVisualizerWithMemoryPool visualizer(ss.str());
    visualizer.parse(path);
    auto end_route_calc = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_route_calc - start_route_calc);
    std::cout
        << "Execution time of calculaitons with Arena memory of route id = "
        << i << " time =" << duration.count() << " milliseconds" << std::endl;
  }
  auto end_method = std::chrono::high_resolution_clock::now();

  // Calculate the duration
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_method - start_method);

  std::cout << "Execution time of calculaitons with Arena memory: "
            << duration.count() << " milliseconds" << std::endl;
}

void memory_pool_calculation() {
  // Assuming m_segments and m_connectors are already populated
  UnorderedSegmentsWithMemoryPool m_segments;
  auto start_method = std::chrono::high_resolution_clock::now();

  std::filesystem::path csv_file("./../france_roads_reduced.csv");
  if (!std::filesystem::exists(csv_file)) {
    std::cout << "[ERROR] in main. file " << csv_file << " does not exist\n";
    return;
  }
  common::SegmentParserWithMemoryPool segment_parser(csv_file);
  segment_parser.read_data();
  auto segments = segment_parser.get_segments();
  auto data_preparation = std::chrono::high_resolution_clock::now();
  auto time_for_data_preparation =
      std::chrono::duration_cast<std::chrono::milliseconds>(data_preparation -
                                                            start_method);
  std::cout << "Execution time of data preparation with Pool memory: "
            << time_for_data_preparation.count() << " milliseconds"
            << std::endl;
  router::RouterWithMemoryPool router(
      segments, std::make_unique<router::DeijkstraCostCalculator>());
  for (size_t i = 0; i < routes.size(); ++i) {
    auto start_route_calc = std::chrono::high_resolution_clock::now();
    vector<Segment, boost::pool_allocator<common::Segment>> path =
        router.findShortestPath(routes[i].first, routes[i].second);

    if (!path.empty()) {
      cout << "Found a path:" << endl;
    } else {
      cout << "No path found." << endl;
    }

    std::stringstream ss;
    ss << "pool_mem_path_" << i << ".json";
    graph_visualizer::GeoJsonGraphVisualizerWithMemoryPool visualizer(ss.str());
    visualizer.parse(path);
    auto end_route_calc = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_route_calc - start_route_calc);
    std::cout
        << "Execution time of calculaitons with POOL memory of route id = " << i
        << " time =" << duration.count() << " milliseconds" << std::endl;
  }
  auto end_method = std::chrono::high_resolution_clock::now();

  // Calculate the duration
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_method - start_method);

  std::cout << "Execution time of calculaitons with Arena memory: "
            << duration.count() << " milliseconds" << std::endl;
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();
  /*MemoryType mem_type = MemoryType::DEFAULT;
  if (mem_type == MemoryType::DEFAULT) {
    default_memory_calculation();
  } else if (mem_type == MemoryType::MEMORY_ARENA) {

  } else if (mem_type == MemoryType::MEMORY_POOL) {
    memory_pool_calculation();
  } else {
    return -1;
  }*/
  default_memory_calculation();
  memory_arena_calculation();
  memory_pool_calculation();
  auto end = std::chrono::high_resolution_clock::now();

  // Calculate the duration
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Execution time: " << duration.count() << " milliseconds"
            << std::endl;

  return 0;
}
