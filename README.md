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

## Documentation

For generating the Doxygen documentation:

1. Install doxygen on your computer with `sudo apt install doxygen`
2. Install the required graphviz library for the documentation with `sudo apt install graphviz`
3. Enter the [build](./build/) directory by running `cd build`.
4. Execute `cmake ..` to generate the Makefile.
5. Run `make` or `cmake --build .` to build the executable.
6. You can now generate the documentation by running `make doc_doxygen`
7. the documentation will be created in [Dokumentation in html](./build/docs/html/index.html)


## Usage

