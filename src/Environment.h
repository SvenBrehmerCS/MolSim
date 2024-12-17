/**
 * @file
 *
 * @brief Defines a container for global constants that can be given as command line arguments.
 */

#pragma once

#include "utils/ArrayUtils.h"

#include <string>

/**
 * @enum BoundaryType
 *
 * @brief Define the different options for a boundary condition.
 */
enum BoundaryType {
    /**
     * Define the old square algorithm which does not require any boundaries.
     */
    INF_CONT,

    /**
     * Define the linked cell algorithm with a reflective boundary, using ghost particles.
     */
    HALO,

    /**
     * Define the linked cell algorithm with a reflective boundary, using hard reflection.
     */
    HARD,

    /**
     * Define the linked cell algorithm with periodic boundaries.
     */
    PERIODIC,

    /**
     * Define the linked cell algorithm with outflow boundaries.
     */
    OUTFLOW,
};

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
enum OutputFormat {
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
    XYZ,

    /**
     * Define the checkpoint file format
     */
    CHECKPOINT,
};

/**
 * @enum InputFormat
 *
 * @brief The enum describes the type of input file used.
 */
enum InputFormat {
    /**
     * Define a standard text file format.
     */
    TXT,

    /**
     * Define the XML file format.
     */
    XML,
};

/**
 * @class Environment
 *
 * @brief Describe the simulation environment. The simulation environment stores the simulation parameters.
 */
class Environment {
private:
    /**
     * Store the input file, which provides the initial simulation state.
     */
    const char* input_file = nullptr;

    /**
     * Store the input file format. Automatically set based on input file.
     */
    InputFormat input_format;

    /**
     * Store the beginning of the name of the output file. The default is MD_vtk. The output file name looks like this: <name>_<iteration>.<vtu | xyz>
     */
    std::string output_file = "MD_vtk";

    /**
     * Store the output file format. it can either be vtk or xyz, with vtk being the default.
     */
    OutputFormat output_format = VTK;

    /**
     * Store after how many steps the simulation state should be saved. By default it is initialized to 10.
     */
    int print_step = 10;

    /**
     * Store which calculator should be used for the force calculations.
     */
    CalculatorType calc = LJ_FULL;

    /**
     * Store the condition of the XY boundary at the origin:
     */
    BoundaryType xy_near = INF_CONT;
    /**
     * Store the condition of the XZ boundary at the origin:
     */
    BoundaryType xz_near = INF_CONT;
    /**
     * Store the condition of the YZ boundary at the origin:
     */
    BoundaryType yz_near = INF_CONT;
    /**
     * Store the condition of the XY boundary away from the origin:
     */
    BoundaryType xy_far = INF_CONT;
    /**
     * Store the condition of the XZ boundary away from the origin:
     */
    BoundaryType xz_far = INF_CONT;
    /**
     * Store the condition of the YZ boundary away from the origin:
     */
    BoundaryType yz_far = INF_CONT;


    /**
     * Store the epsilon used for the Lenard-Jones calculation. By default it is initialized to 5.0.
     */
    double epsilon = 5.0;

    /**
     * Store the sigma used for the Lenard-Jones calculation. By default it is initialized to 1.0.
     */
    double sigma = 1.0;
    /**
     * Store the time delta. By default it is initialized to 0.014.
     */
    double delta_t = 0.014;

    /**
     * Store the end time. By default it is initialized to 1000.0.
     */
    double t_end = 1000.0;

    /**
     * Specifies the number of dimensions of the simulation.
     */
    int dimensions = 3;

    /**
     * Store the radius beyond which force calculation is cut off.
     */
    double r_cutoff = 3.0;

    /**
     * Store the total size of the simulation domain.
     */
    std::array<double, 3> domain_size = { 180.0, 90.0, 1.0 };

    /**
     * Specifies the name of the input_file_checkpoint.
     */
    std::string checkpoint_file_name;

    /**
     * Specifies the number of steps between thermometer applications.
     */
    int temp_frequency = 1000;

    /**
     * Store the gravity pulling the atoms down.
     */
    double gravity = 0.0;

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
     * Destroy a simulation environment.
     */
    ~Environment();

    // Getter methods

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
     * Get the format of the the output file. Used to determine needed reader.
     *
     * @return The file format of the input file.
     */
    InputFormat get_input_file_format() const;

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
    OutputFormat get_output_file_format() const;

    /**
     * Get the calculator type which should be used for the force calculations.
     *
     * @return The calculator type.
     */
    CalculatorType get_calculator_type() const;

    /**
     * Get the Boundary type used in the simulation.
     *
     * @return The boundary type. Will be in form {yz_near, xz_near, xy_near, yz_far, xz_far, xy_far}
     */
    std::array<BoundaryType, 6> get_boundary_type() const;

    /**
     * Get the cutoff radius beyond which forces no longer affect particles.
     *
     * @return The radius.
     */
    double get_r_cutoff() const;


    /**
     * Get the gravity pulling the atoms down.
     *
     * @return The gravity.
     */
    double get_gravity() const;

    /**
     * Get the domain size of the simulation in form of a vector representing the size in {x, y, z} direction.
     *
     * @return The vector representing the size of the domain.
     */
    std::array<double, 3> get_domain_size() const;

    /**
     * Test if the simulation requires the direct sum implementation.
     *
     * @return A boolean indicating if the direct sum implementation is required.
     */
    bool requires_direct_sum() const;

    /**
     * Test if the simulation requires the direct sum implementation.
     */
    void assert_boundary_conditions();

    /**
     * Get the number of dimensions of the simulation.
     *
     * @return The number of dimensions.
     */
    int get_dimensions() const;

    /**
     * Get the number of steps between thermometer applications.
     *
     * @return The application frequency.
     */
    int get_temp_frequency() const;

    // Setter methods

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
     * Set the sigma used for lenard jones simulations.
     *
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
    void set_output_file_name(std::string& output_file_name);

    /**
     * Set the format of the the output file.
     *
     * @param format The file format of the output file.
     */
    void set_output_file_format(const OutputFormat format);

    /**
     * Set the calculator type which should be used for the force calculations.
     *
     * @param calculator_type The calculator type.
     */
    void set_calculator_type(const CalculatorType calculator_type);

    /**
     * Set the boundary type used in the simulation.
     *
     * @param boundary_type The boundary type.
     */
    void set_boundary_type(const std::array<BoundaryType, 6> boundary_type);

    /**
     * Set the cutoff radius beyond which forces no longer affect particles.
     *
     * @param r_cutoff The radius.
     */
    void set_r_cutoff(const double r_cutoff);

    /**
     * Set the domain size at whichs end the boundaries are located.
     *
     * @param domain_size Vector of domain size along the axes.
     */
    void set_domain_size(const std::array<double, 3> domain_size);

    /**
     *
     * get the checkpoint file name
     */
    const char* get_checkpoint_file_name() const;

    /**
     * set the checkpoint_file_name
     * @param checkpoint_file_name
     */
    void set_checkpoint_file_name(std::string& checkpoint_file_name);

    /**
     * Set the number of dimensions of the simulation.
     *
     * @param dimensions The number of dimensions.
     */
    void set_dimensions(const int dimensions);

    /**
     * Set the number of steps between thermometer applications.
     *
     * @param temp_frequency The application frequency.
     */
    void set_temp_frequency(const int temp_frequency);

    /**
     * Set the gravity pulling the atoms down.
     *
     * @param g The gravity pulling the atoms down.
     */
    void set_gravity(const double g);
};
