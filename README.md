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
   
   
