/**
 * @file
 *
 * @brief Defines a container for global constants that can be given as command line arguments.
 */

#pragma once

/**
 * @enum CalculatorType
 *
 * @brief This enum describes the different calculators used by this program.
 */
enum CalculatorType {
    /**
     * Define the gravity calculation type.
     */
    GRAVITY,

    /**
     * Define the lenard jones calculation type without range cut-offs.
     */
    LJ_FULL,
};

/**
 * @enum FileFormat
 *
 * @brief The enum describes the different file formats used by this program.
 */
enum FileFormat {
    /**
     * Define the no output format.
     */
    NO_OUT,

    /**
     * Define the ascii based vtk file format.
     */
    VTK,

    /**
     * Define the ascii based xyz file format.
     */
    XYZ
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
     * Store the epsilon used for the Lenard-Jones calculation. By default it is initialized to 5.0.
     */
    double epsilon = 5.0;

    /**
     * Store the sigma used for the Lenard-Jones calculation. By default it is initialized to 1.0.
     */
    double sigma = 1.0;

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

    /**
     * Store which calculator should be used for the force calculations.
     */
    CalculatorType calc = LJ_FULL;

    /**
     * Store the radius beyond which force calculation is cut off.
     */
    double r_cutoff = 3.0;

public:
    /**
     * Create a standard environment with all arguments being initialized to their default. The input file name will be null.
     */
    Environment();

    /**
     * Create a simulation environment from the command line arguments. The constructor also handles the help argument.
     *
     * @param argc The number of arguments passed.
     * @param argv The array storing all passed arguments, including the program name.
     */
    Environment(const int argc, const char* argv[]);

    /**
     * Create a copy of an environment object.
     *
     * @param env THe environment that should be copied.
     */
    Environment(const Environment& env);

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
     * Get the sigma used for lenard jones simulations.
     *
     * @return The sigma for force calculation.
     */
    double get_sigma() const;

    /**
     * Get the epsilon used for lenard jones simulations.
     *
     * @return The epsilon for force calculation.
     */
    double get_epsilon() const;

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

    /**
     * Get the calculator type which should be used for the force calculations.
     *
     * @return The calculator type.
     */
    CalculatorType get_calculator_type() const;

    /**
     * Get the cutoff radius beyond which forces no longer affect particles.
     *
     * @return The radius.
     */
    double get_r_cutoff() const;

    /**
     * Set the end time (duration) of the simulation.
     *
     * @param t_end The end time of the simulation.
     */
    void set_t_end(const double t_end);


    /**
     * Set the amount of time simulated by a single step.
     *
     * @param delta_t The time delta for a single simulation step.
     */
    void set_delta_t(const double delta_t);


    /**
     * Get the sigma used for lenard jones simulations.
     * @param sigma The sigma for force calculation.
     */
    void set_sigma(const double sigma);


    /**
     * Set the epsilon used for lenard jones simulations.
     *
     * @param epsilon The epsilon for force calculation.
     */
    void set_epsilon(const double epsilon);


    /**
     * Set how often the simulation output file should be generated.
     *
     * @param print_step An integer indicating which simulation steps should be saved.
     */
    void set_print_step(const int print_step);


    /**
     * Set the beginning of the name of the output file. All output files will start with this name.
     *
     * @param output_file_name The file name of the output file.
     */
    void set_output_file_name(const char* output_file_name);


    /**
     * Set the format of the the output file.
     *
     * @param format The file format of the output file.
     */
    void set_output_file_format(const FileFormat format);


    /**
     * Set the calculator type which should be used for the force calculations.
     *
     * @param calculator_type The calculator type.
     */
    void set_calculator_type(const CalculatorType calculator_type);

    /**
     * Set the cutoff radius beyond which forces no longer affect particles.
     *
     * @param r_cutoff The radius.
     */
    void set_r_cutoff(const double r_cutoff);
};
