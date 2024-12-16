#include "graph_visualizer/GeoJsonGraphVisualizer.h"
#include <fstream>

namespace graph_visualizer {

void GeoJsonGraphVisualizer::parse(
    const std::vector<common::Segment> &segments) {
  rapidjson::Document doc;
  doc.SetObject();
  rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();

  rapidjson::Value features(rapidjson::kArrayType);
  doc.AddMember("type", "FeatureCollection", allocator);
  doc.AddMember("features", features, allocator);

  for (const auto &segment : segments) {
    addSegment(doc, segment);
  }

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);

  std::ofstream ofs(m_output_file);
  ofs << buffer.GetString();
  ofs.close();
}

void GeoJsonGraphVisualizer::addSegment(rapidjson::Document &doc,
                                        const common::Segment &segment) {
  rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();

  // Create the GeoJSON Feature object
  rapidjson::Value feature(rapidjson::kObjectType);
  feature.AddMember("type", "Feature", allocator);

  // Create the geometry object
  rapidjson::Value geometry(rapidjson::kObjectType);
  geometry.AddMember("type", "LineString", allocator);

  // Create the coordinates array
  rapidjson::Value coordinates(rapidjson::kArrayType);

  for (const auto &point : segment.get_points()) {
    rapidjson::Value coord(rapidjson::kArrayType);
    coord.PushBack(point.longitude, allocator)
        .PushBack(point.latitude, allocator);
    coordinates.PushBack(coord, allocator);
  }

  geometry.AddMember("coordinates", coordinates, allocator);

  // Add geometry to the feature
  feature.AddMember("geometry", geometry, allocator);

  // Add properties to the feature (if needed, you can extend this part)
  rapidjson::Value properties(rapidjson::kObjectType);
  feature.AddMember("properties", properties, allocator);

  // Add feature to the features array in the document
  if (!doc.HasMember("features")) {
    rapidjson::Value features(rapidjson::kArrayType);
    doc.AddMember("features", features, allocator);
  }
  doc["features"].PushBack(feature, allocator);
}

void GeoJsonGraphVisualizerWithMemoryPool::parse(
    const std::vector<common::Segment, boost::pool_allocator<common::Segment>>
        &segments) {
  rapidjson::Document doc;
  doc.SetObject();
  rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();

  rapidjson::Value features(rapidjson::kArrayType);
  doc.AddMember("type", "FeatureCollection", allocator);
  doc.AddMember("features", features, allocator);

  for (const auto &segment : segments) {
    addSegment(doc, segment);
  }

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);

  std::ofstream ofs(m_output_file);
  ofs << buffer.GetString();
  ofs.close();
}

void GeoJsonGraphVisualizerWithMemoryPool::addSegment(
    rapidjson::Document &doc, const common::Segment &segment) {
  rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();

  // Create the GeoJSON Feature object
  rapidjson::Value feature(rapidjson::kObjectType);
  feature.AddMember("type", "Feature", allocator);

  // Create the geometry object
  rapidjson::Value geometry(rapidjson::kObjectType);
  geometry.AddMember("type", "LineString", allocator);

  // Create the coordinates array
  rapidjson::Value coordinates(rapidjson::kArrayType);

  for (const auto &point : segment.get_points()) {
    rapidjson::Value coord(rapidjson::kArrayType);
    coord.PushBack(point.longitude, allocator)
        .PushBack(point.latitude, allocator);
    coordinates.PushBack(coord, allocator);
  }

  geometry.AddMember("coordinates", coordinates, allocator);

  // Add geometry to the feature
  feature.AddMember("geometry", geometry, allocator);

  // Add properties to the feature (if needed, you can extend this part)
  rapidjson::Value properties(rapidjson::kObjectType);
  feature.AddMember("properties", properties, allocator);

  // Add feature to the features array in the document
  if (!doc.HasMember("features")) {
    rapidjson::Value features(rapidjson::kArrayType);
    doc.AddMember("features", features, allocator);
  }
  doc["features"].PushBack(feature, allocator);
}

} // namespace graph_visualizer
