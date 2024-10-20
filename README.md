# MolSim Group B

This file contains instructions on how to build the molecular dynamics programm, developed by Frederik Willemsen, Sven Brehmer and Patrick Metscher.

## Building

For building the project follow these steps:

1. Make sure that your CC and CXX environment variables are set to gcc and g++ respectively.
2. Install the libxerces-c library. On Ubuntu it can be installed by executing `sudo apt-get install libxerces-c-dev`.
3. Create the [build](./build/) directory by running `mkdir build`.
4. Enter the [build](./build/) directory by running `cd build`.
5. Execute `cmake ..` to generate the Makefile.
6. Run `make` or `cmake --build .` to build the executable.
7. Execute the executale by runing `./MolSim <args>`. For further details refer to the [Usage](#usage) section.

## Usage

