# osmplot

Tool parses Openstreetmap OSM files and plots its buildings and ways. 

## Overview

`osmplot` is a small C++ tool to process OpenStreetMap (OSM) data files. It currently provides functionality to parse data and visualize map elements like buildings and ways. It plots lanes of specific width around the way lines ensuring they do not overlap with any of the buildings beside to the way.

## Prerequisities

osmplot requires the X11 development package

On Debian/Ubuntu:
   ```bash
   sudo apt-get install libx11-dev
   ```

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/tnihtila/osmplot.git
   ```
   
2. Build
   ```bash
   cd osmplot
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Test with an example input OSM file
   ```bash
   ./bin/osmplot ../input/jurvala.osm
   ```

## Usage

Run the program with an OSM XML file as input:

```bash
./bin/osmplot <path-to-osm-file>
```

A window will open displaying the parsed map with buildings, ways, and lanes. The display will remain open until you close it manually.

## Input File Format

- The input must be an OpenStreetMap XML file (typically with `.osm` extension).
- The file should contain `<bounds>`, `<node>`, and `<way>` elements as exported from OSM.
- Example input file: `input/jurvala.osm` (provided in this repository).

## Features

- Parses OSM XML files for nodes, ways, and buildings.
- Visualizes buildings as polygons and ways as lines.
- Plots lanes of configurable width around ways, avoiding overlap with buildings.
- Uses the [CImg](https://cimg.eu/) library for image rendering.

## Dependencies

- C++11 or newer
- [CImg](https://cimg.eu/) (header-only, included)
- X11 development libraries (for display)
- CMake (for building)

## Troubleshooting

- If you encounter errors related to X11, ensure `libx11-dev` is installed.
- If the program does not display a window, check your X server or run in a graphical environment.

## License

This project is licensed under the MIT License.

## Author & Contact

Timo Nihtilä (<timo.nihtila@gmail.com>)

Contributions and issues are welcome via the [GitHub repository](https://github.com/tnihtila/osmplot).


