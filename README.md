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
7. Execute the executale by runing `./MolSim <args> <input file>`. For further details refer to the [Usage](#usage) section.

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

To execute the simulation run `./MolSim <args> <input file>`.

These are the possible arguments for the simulation:

| Argument                       | Description                                                                                                                                                    |
| ------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `-h`, `--h`, `-help`, `--help` | Print a help message and terminate immediately.                                                                                                                |
| `-t_end=<end time>`            | Set the time until wich the simulation should run. The end time (duration) must be a positive floating point number. The default end time is 1000.0.           |
| `-delta_t=<time delta>`        | Set the time delta with wich the steps should be performed. The delta time must be a strictly positive floating point number. The default time delta is 0.014. |
| `-print_step=<print step>`     | Set the print step with wich the steps should be performed. The print step must be a strictly positive integer. The default print step is 10.                  |
| `-out_name=<output file name>` | Set the beginning of the output file name as given. The file name must be a string at least one character long. The default output file name is MD_vtk.        |
| `-output_format=<file format>` | Set the format of the output file either to 'vtk' or to 'xyz'. The default output file format is vtk.                                                          |

Each argument may only be provided once. If no argument is provided the default value is being used. There may not be any blank spaces seperating the option and its value. The output files will be placed in the folder, from where the programm is executed. The output files will have the VTK format.

A programm call might look like this:

`./MolSim -t_end=10.0 -print_step=20 ./path/to/input.txt`

This would run a simulation lasting for 10 time units, printing every 20th iteration. The initial state would be given in the file input.txt, every other parameter would have the default value.

`./MolSim -delta_t=1 -out_name=MD ./path/to/input.txt`

This would run a simulation with an update increment of 1 time unit, printing to output files of the format `MD_<iteration>.vtu`. The initial state would be given in the file input.txt, every other parameter would have the default value.