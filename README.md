# MolSim Group B

This file contains instructions on how to build the molecular dynamics program, developed by Frederik Willemsen, Sven Brehmer and Patrick Metscher.

## Building

For building the project follow these steps:

1. Make sure that your CC and CXX environment variables are set to gcc and g++ respectively.
2. Install the libxerces-c library. On Ubuntu it can be installed by executing `sudo apt-get install libxerces-c-dev`.
3. Create the [build](./build/) directory by running `mkdir build`.
4. Enter the [build](./build/) directory by running `cd build`.
5. Execute `cmake ..` to generate the Makefile.
6. Run `make` or `cmake --build .` to build the executable.
7. Execute the executable by running `./MolSim <args> <input file>`. For further details refer to the [Usage](#usage) section.

For speeding up the compilation process it is recommended to append `-j #cores` to the `make` command.

## Documentation

For generating the Doxygen documentation:

1. Install doxygen on your computer with `sudo apt install doxygen`.
2. Install the required graphviz library for the documentation with `sudo apt install graphviz`.
3. Build the project according to [Building](#building) (Steps 1.-5.).
4. You can now generate the documentation by running `make doc_doxygen`.
5. The documentation will be created in [build/docs/html](./build/docs/html/index.html) in html format.

## Tests

For executing the tests follow these steps:

1. First build the project following the steps 1-6 in the section [Building](#building).
2. Run the tests by calling `ctest` in the build directory.
3. For detailed debugging refer to the [TestLog.log](./build/Testing/Temporary/LastTest.log)

## Usage

To execute the simulation run `./MolSim <args> <input file>`.

These are the possible arguments for the simulation:

| Argument                       | Description                                                                                                                                                     |
| ------------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `-h`, `--h`, `-help`, `--help` | Print a help message and terminate immediately.                                                                                                                 |
| `-t_end=<end time>`            | Set the time until which the simulation should run. The end time (duration) must be a positive floating point number. The default end time is 1000.0.           |
| `-delta_t=<time delta>`        | Set the time delta with which the steps should be performed. The delta time must be a strictly positive floating point number. The default time delta is 0.014. |
| `-sigma=<sigma>`               | Set the sigma used for the lenard jones simulation. The sigma must be a strictly positive floating point number. The default sigma is 1.0.                      |
| `-epsilon=<epsilon>`           | Set the epsilon used for the lenard jones simulation. The epsilon must be a strictly positive floating point number. The default epsilon is 5.0.                |
| `-print_step=<print step>`     | Set the print step with which the steps should be performed. The print step must be a strictly positive integer. The default print step is 10.                  |
| `-out_name=<output file name>` | Set the beginning of the output file name as given. The file name must be a string at least one character long. The default output file name is MD_vtk.         |
| `-output_format=<file format>` | Set the format of the output file either to 'no' (write no output files), 'vtk' or to 'xyz'. The default output file format is vtk.                             |
| `-log_level=<log level>`       | Set the log level to one of the standard spd log levels ('off', 'crit', 'error', 'warn', 'info', 'debug', 'trace'). The default level is info.                  |
| `-calc=<force model>`          | Set the force model of the calculator either to 'gravity' or 'lj' (Lenard Jones). The default force model is lj.                                                |

Each argument may only be provided once. If no argument is provided the default value is being used. There may not be any blank spaces seperating the option and its value. The output files will be placed in the folder, from where the program is executed. The output files will have the VTK format.

A program call might look like this:

`./MolSim -t_end=10.0 -print_step=20 ./path/to/input.txt`

This would run a simulation lasting for 10 time units, printing every 20th iteration. The initial state would be given in the file input.txt, every other parameter would have the default value.

`./MolSim -delta_t=1 -out_name=MD -calc=gravity ./path/to/input.txt`

This would run a simulation with an update increment of 1 time unit, printing to output files of the format `MD_<iteration>.vtu`. The gravity force model would be used for simulating based on the initial state, given in the file input.txt. Every other parameter would have the default value.

## XML File Input

You can specify a XML File as input on the command line, when passing a XML File over the command line, be sure to follow these steps:
1. Set the input.xsd Schema as the NoNameSpaceLocation of your XML File.
Initialize the Tags in the XML File, the values will be used to run the simulation. 
2. You can specify any number of single particles, cuboids or discs, apart from that do not leave a Tag empty, if there is no default value for that element.
3. See the table of Elements in the file below

# Elements of the XSD Schema

| Element/Type      | Description                                                                      | Attributes                               |
|-------------------|----------------------------------------------------------------------------------|-----------------------------------------|
| **simulation**    | Represents the entire simulation.                                               | `type="sim_t"`                          |
| **output**        | Parameters for generating the output file.                                      | `type="output_t"`                       |
| **param**         | Parameters describing how the simulation will be executed.                      | `type="param_t"`                        |
| **particle**      | Information needed to generate a single particle.                               | `type="particle_t", minOccurs="0", maxOccurs="unbounded"` |
| **cuboid**        | Information needed to generate a cuboid of particles.                           | `type="cuboid_t", minOccurs="0", maxOccurs="unbounded"` |
| **disc**          | Information needed to generate a disc of particles.                             | `type="disc_t", minOccurs="0", maxOccurs="unbounded"` |
| **name**          | Desired name prefix for the output file.                                        | `type="xs:string", default="MD_vtk"`    |
| **format**        | Desired output format.                                                          | `default="VTK"`                         |
| **frequency**     | Number of time steps between outputs.                                           | `default="10"`                          |
| **calc**          | Force calculation mode (e.g., Lennard-Jones).                                   | `default="LJ_FULL"`                     |
| **bound**         | Boundary condition for the simulation.                                          | `default="INF_CONT"`                    |
| **epsilon**       | Depth of the Lennard-Jones potential well.                                      | `default="5.0"`                         |
| **sigma**         | Zero-crossing of the Lennard-Jones potential.                                   | `default="1.0"`                         |
| **delta_t**       | Simulation time step.                                                           | `default="0.014"`                       |
| **t_end**         | End time for the simulation.                                                    | `default="1000.0"`                      |
| **dimensions**    | Number of coordinates affected by Brownian motion.                              | `default="3"`                           |
| **r_cutoff**      | Distance beyond which force calculations are neglected.                         | `default="3.0"`                         |
| **position**      | Position of the particle or object.                                             | `type="dvector"`                        |
| **velocity**      | Velocity of the particle or object.                                             | `type="dvector"`                        |
| **m**             | Mass of the particle.                                                           | `type="xs:double"`                      |
| **count**         | Number of particles in each direction for cuboids.                              | `type="uivector"`                       |
| **h**             | Distance between particles in cuboids or discs.                                 | `type="xs:double"`                      |
| **b_motion**      | Average Brownian motion in the cuboid or disc.                                  | `default="0.0"`                         |
| **center**        | Center position of the disc.                                                    | `type="dvector"`                        |
| **r**             | Radius of the disc in terms of molecules along the radius.                      | `type="xs:double"`                      |


