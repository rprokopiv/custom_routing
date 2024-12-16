import csv
import re

# Define the bounding box for Germany
GERMANY_BBOX = {
    "xmin": 5.866,
    "xmax": 15.041,
    "ymin": 47.270,
    "ymax": 55.099
}
FRANCE_BBOX = {
    "xmin": -5.0,
    "xmax": 9.6,
    "ymin": 41.3,
    "ymax": 51.1
}

FRANCE_BBOX_REDUCED = {
    "xmin": -0.8,
    "xmax": 5.4,
    "ymin": 43.45,
    "ymax": 48.95
}

# Input and output file paths
INPUT_FILE = "france_roads.csv"  # Replace with your input file path
OUTPUT_FILE = "france_roads_reduced.csv"  # Replace with your output file path

def extract_coordinates(linestring):
    """
    Extracts a list of (longitude, latitude) tuples from a WKT LINESTRING.
    Example: 'LINESTRING (-8.6540663 41.0585578, -8.6541508 41.0585446)'
    Returns: [(-8.6540663, 41.0585578), (-8.6541508, 41.0585446)]
    """
    coordinates = []
    match = re.search(r"LINESTRING \((.+)\)", linestring)
    if match:
        points = match.group(1).split(",")
        for point in points:
            lon, lat = map(float, point.strip().split())
            # print(f"lon= {lon}, lat = {lat}")
            coordinates.append((lon, lat))
    return coordinates

def is_within_bbox(coords):
    """
    Checks if any point in the list of coordinates is within the bounding box.
    """
    for lon, lat in coords:
        if (
            FRANCE_BBOX_REDUCED["xmin"] <= lon <= FRANCE_BBOX_REDUCED["xmax"] and
            FRANCE_BBOX_REDUCED["ymin"] <= lat <= FRANCE_BBOX_REDUCED["ymax"]
        ):
            return True
    return False

def filter_csv(input_file, output_file):
    """
    Filters the input CSV file to retain only rows with geometries within the bounding box.
    """
    with open(input_file, mode="r", newline="", encoding="utf-8") as infile, \
         open(output_file, mode="w", newline="", encoding="utf-8") as outfile:
        
        reader = csv.reader(infile, delimiter="|")
        writer = csv.writer(outfile, delimiter="|")
        
        # Write the header row
        header = next(reader)
        id_idx = 0
        connectors_idx = 1
        geometry_idx = 2
        writer.writerow(["id", "connectors", "geometry"])
        id = 1
        
        for row in reader:
            geometry = row[geometry_idx]
            coords = extract_coordinates(geometry)
            if is_within_bbox(coords):
                # Write the filtered row
                writer.writerow([row[id_idx], row[connectors_idx], row[geometry_idx]])
                print(id)
                id += 1

if __name__ == "__main__":
    filter_csv(INPUT_FILE, OUTPUT_FILE)
    print(f"Filtered data saved to {OUTPUT_FILE}")
