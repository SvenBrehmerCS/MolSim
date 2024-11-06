/**
 * @file
 *
 * @brief Defines a container for global constants that can be given as command line arguments.
 */

#pragma once

/**
 * @enum FileFormat
 *
 * @brief The enum describes the different file formats used by this program.
 */
enum FileFormat : int {
    /**
     * Define the ascii based vtk file format.
     */
    VTK = 1,

    /**
     * Define the ascii based xyz file format.
     */
    XYZ = 2,
};

/**
 * @class Environment
 *
 * @brief Describe the simulation environment. The simulation environment stores the simulation parameters.
 */
class Environment {
private:
    /**
     * Store the end time. By default it is initialized to 1000.0.
     */
    double t_end = 1000.0;

    /**
     * Store the time delta. By default it is initialized to 0.014.
     */
    double delta_t = 0.014;

    /**
     * Store after how many steps the simulation state should be saved. By default it is initialized to 10.
     */
    int print_step = 10;

    /**
     * Store the input file, which provides the initial simulation state.
     */
    const char* input_file = nullptr;

    /**
     * Store the beginning of the name of the output file. The default is MD_vtk. The output file name looks like this: <name>_<iteration>.<vtu | xyz>
     */
    const char* output_file = "MD_vtk";

    /**
     * Store the output file format. it can either be vtk or xyz, with vtk being the default.
     */
    FileFormat format = VTK;

public:
    /**
     * Create a standard environment with all arguments being initialized to their default. The input file name will be null.
     */
    Environment();

    /**
     * Create a simulation environment from the command line arguments. The constructor also handels the help argument.
     *
     * @param argc The number of arguments passed.
     * @param argv The array storing all passed arguments, including the program name.
     */
    Environment(const int argc, const char* argv[]);

    /**
     * Create a copy of an environment object.
     * 
     * @param env THe environmetn that should be copied.
     */
    Environment(const Environment &env);

    /**
     * Destroy a simulation environment.
     */
    ~Environment();

    /**
     * Get the end time (duration) of the simulation.
     *
     * @return The end time of the simulation.
     */
    double get_t_end() const;

    /**
     * Get the amount of time simulated by a single step.
     *
     * @return The time delta for a single simulation step.
     */
    double get_delta_t() const;

    /**
     * Get how often the simulation output file should be generated.
     *
     * @return An integer indicating which simulation steps should be saved.
     */
    int get_print_step() const;

    /**
     * Get the name of the input file, used to initialize the simulation.
     *
     * @return The file name and path to the input file.
     */
    const char* get_input_file_name() const;

    /**
     * Get the beginning of the name of the output file. All output files will start with this name.
     *
     * @return The file name of the output file.
     */
    const char* get_output_file_name() const;

    /**
     * Get the format of the the output file.
     *
     * @return The file format of the output file.
     */
    FileFormat get_output_file_format() const;
};
