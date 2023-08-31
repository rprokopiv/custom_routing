#include "SegmentParser.h"

#include <boost/program_options.hpp>

int main(int argc, char *argv[]) {

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()("f", boost::program_options::value<std::string>(),
                     "Input file")(
      "id", boost::program_options::value<std::string>(), "Segment id");

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  std::string file_name;
  std::string id;
  if (vm.count("f")) {
    file_name = vm["f"].as<std::string>();
    std::cout << "Input segment file: " << file_name << std::endl;
  }

  if (vm.count("id")) {
    id = vm["id"].as<std::string>();
    std::cout << "Segment id: " << id << std::endl;
  }
  std::filesystem::path csv_file(file_name);
  if (!std::filesystem::exists(csv_file)) {
    std::cout << "[ERROR] in main. file " << csv_file << " does not exist\n";
    return 0;
  }
  common::SegmentParser segment_parser(csv_file);
  segment_parser.read_data();
  auto segments = segment_parser.get_segments();

  auto pos = segments.find(common::Segment(common::UniqueId(id), {}, {}));
  if (pos == segments.end()) {
    std::cout << "requested segment was not found\n";
    return 0;
  }
  std::cout << std::fixed << std::setprecision(7);
  std::cout << "id of segment = " << pos->get_id().get_id() << "\n";
  std::cout << "points [ ";
  for (auto &i : pos->get_points()) {
    std::cout << i.longitude << " " << i.latitude << ", ";
  }
  std::cout << " ] size = " << pos->get_points().size() << "\n";
  std::cout << "distance of segment = "
            << common::GeoPoint::distance_m(pos->get_points()) << "m\n";

  return 0;
}