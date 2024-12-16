
In order to download data for the whole Europe please run the next command in the duckdb

LOAD spatial; -- noqa
LOAD httpfs;  -- noqa
-- Access the data on AWS in this example
SET s3_region='us-west-2';

COPY (
SELECT
    id, connectors, ST_GeomFromWkb(geometry) as geometry
FROM read_parquet('s3://overturemaps-us-west-2/release/2024-11-13.0/theme=transportation/type=segment/*')
WHERE
    bbox.xmin > -13.611662005206092 AND bbox.xmax < 4.920635432537814 AND
    bbox.ymin > 41.05630580182695 AND bbox.ymax < 71.42665969033948
)
TO 'check_segments.csv' (FORMAT CSV, DELIMITER '|', HEADER);



To download only Germany --



LOAD spatial; -- noqa
LOAD httpfs;  -- noqa
-- Access the data on AWS in this example
SET s3_region='us-west-2';

COPY (
SELECT
    id, connectors, ST_GeomFromWkb(geometry) as geometry
FROM read_parquet('s3://overturemaps-us-west-2/release/2024-11-13.0/theme=transportation/type=segment/*')
WHERE
bbox.xmin > 5.866 AND bbox.xmax < 15.041 AND
        bbox.ymin > 47.270 AND bbox.ymax < 55.099
)
TO 'segments_germany.csv' (FORMAT CSV, DELIMITER '|', HEADER);