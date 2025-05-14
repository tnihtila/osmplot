# osmplot

Parse Openstreetmap OSM files and plot buildings and ways.

## Overview

`osmplot` is a small tool designed to process OpenStreetMap (OSM) data files. It provides functionality to parse data and visualize map elements like buildings and ways.

This tool is written in **C++**.

## Features

- Parse OpenStreetMap OSM files.
- Plot buildings and ways from the parsed data.
- Lightweight implementation.

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
   
   
