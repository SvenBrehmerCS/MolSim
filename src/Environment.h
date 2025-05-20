/**
 * @file
 *
 * @brief Defines a container for global constants that can be given as command line arguments.
 */

#pragma once

#include "utils/Vec.h"

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

    /**
     * Define a membrane supporting lenard jones calculation type.
     */
    LJ_MOL,

    /**
     * Define the smoothed version of the lenard jones calculation.
     */
    LJ_SMOOTH,
};

/**
 * @enum UpdateStrategy
 *
 * @brief This enum describes the parallelization strategy used by the simulation.
 */
enum UpdateStrategy {
    /**
     * Define that no parallelization is used.
     */
    SERIAL,

    /**
     * Defines a strategy that  updates the cells, that are far enough spaced to not interfere with each other, at the same time.
     */
    GRID,

    /**
     * Defines a strategy that divides the cells into slices and executes the same directional update in parallel.
     */
    SLICE,
};

/**
 * @enum OutputFormat
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
     * Define the checkpoint file format.
     */
    CHECKPOINT,

    /**
     * Define the radial distribution function format.
     */
    RDF,
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
     * Store the beginning of the name of the diffusion file. (default: MD_Diff). The file name looks like this: <name>_<iteration>.<vtu | xyz>
     */
    std::string diff_file = "MD_Diff";

    /**
     * Store wether the diffusion files should be created.
     */
    bool generate_diff = false;

    /**
     * Store the output file format. it can either be vtk or xyz, with vtk being the default.
     */
    OutputFormat output_format = VTK;

    /**
     * Stores the number of buckets the domain is divided into.
     */
    size_t RDF_bucket_num = 0;

    /**
     * Stores the size of the buckets the domain is divided into.
     */
    double RDF_bucket_size = 0.0;

    /**
     * Store after how many steps the simulation state should be saved. By default it is initialized to 10.
     */
    int print_step = 10;

    /**
     * Store which calculator should be used for the force calculations.
     */
    CalculatorType calc = LJ_FULL;

    /**
     * Store the parallelization strategy used for updating the cells.
     */
    UpdateStrategy strat = SERIAL;

    /**
     * Store the condition of the XY boundary at the origin.
     */
    BoundaryType xy_near = INF_CONT;
    /**
     * Store the condition of the XZ boundary at the origin.
     */
    BoundaryType xz_near = INF_CONT;
    /**
     * Store the condition of the YZ boundary at the origin.
     */
    BoundaryType yz_near = INF_CONT;
    /**
     * Store the condition of the XY boundary away from the origin.
     */
    BoundaryType xy_far = INF_CONT;
    /**
     * Store the condition of the XZ boundary away from the origin.
     */
    BoundaryType xz_far = INF_CONT;
    /**
     * Store the condition of the YZ boundary away from the origin.
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
     * Store the radius beyond which force calculation is cut off.
     */
    double r_l = 3.0;

    /**
     * Store the total size of the simulation domain.
     */
    Vec<double> domain_size = { 180.0, 90.0, 1.0 };

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

    /**
     * Test if the simulation requires the direct sum implementation.
     *
     * @return A boolean indicating if the direct sum implementation is required.
     */
    const bool requires_direct_sum() const;

    /**
     * Test if the simulation requires the direct sum implementation.
     */
    void assert_boundary_conditions();

    // Getter methods

    /**
     * Get the end time (duration) of the simulation.
     *
     * @return The end time of the simulation.
     */
    inline const double get_t_end() const { return t_end; }

    /**
     * Get the amount of time simulated by a single step.
     *
     * @return The time delta for a single simulation step.
     */
    inline const double get_delta_t() const { return delta_t; }

    /**
     * Get the sigma used for lenard jones simulations.
     *
     * @return The sigma for force calculation.
     */
    inline const double get_sigma() const { return sigma; }

    /**
     * Get the epsilon used for lenard jones simulations.
     *
     * @return The epsilon for force calculation.
     */
    inline const double get_epsilon() const { return epsilon; }

    /**
     * Get how often the simulation output file should be generated.
     *
     * @return An integer indicating which simulation steps should be saved.
     */
    inline const int get_print_step() const { return print_step; }

    /**
     * Get the name of the input file, used to initialize the simulation.
     *
     * @return The file name and path to the input file.
     */
    inline const char* get_input_file_name() const { return input_file; }

    /**
     * Get the format of the the output file. Used to determine needed reader.
     *
     * @return The file format of the input file.
     */
    inline const InputFormat get_input_file_format() const { return input_format; }

    /**
     * Get the beginning of the name of the output file. All output files will start with this name.
     *
     * @return The file name of the output file.
     */
    inline const char* get_output_file_name() const { return output_file.c_str(); }

    /**
     * Get the beginning of the name of the diffusion output. All diffusion files will start with this name.
     *
     * @return The file name of the diffusion file.
     */
    inline const char* get_diff_file_name() const { return diff_file.c_str(); }

    /**
     * Get the wether the diffusion files should be created.
     *
     * @return The boolean representing if the output should be created.
     */
    inline const bool get_generate_diff() const { return generate_diff; }

    /**
     * Get the format of the the output file.
     *
     * @return The file format of the output file.
     */
    inline const OutputFormat get_output_file_format() const { return output_format; }

    /**
     * Get the number of buckets dividing used by the RDF writer to divide the domain.
     *
     * @return The number of all buckets.
     */
    inline const size_t get_RDF_bucket_num() const { return RDF_bucket_num; }

    /**
     * Get the size of the buckets used by the RDF writer to divide the domain.
     *
     * @return The size oft all buckets.
     */
    inline const double get_RDF_bucket_size() const { return RDF_bucket_size; }

    /**
     * Get the calculator type which should be used for the force calculations.
     *
     * @return The calculator type.
     */
    inline const CalculatorType get_calculator_type() const { return calc; }

    /**
     * Get the update strategy used for parallel updating the cells.
     *
     * @return The update strategy.
     */
    inline const UpdateStrategy get_update_strategy() const { return strat; }

    /**
     * Get the Boundary type used in the simulation.
     *
     * @return The boundary type. Will be in form {yz_near, xz_near, xy_near, yz_far, xz_far, xy_far}
     */
    const std::array<BoundaryType, 6> get_boundary_type() const;

    /**
     * Get the cutoff radius beyond which forces no longer affect particles.
     *
     * @return The radius.
     */
    inline const double get_r_cutoff() const { return r_cutoff; }

    /**
     * Get the cutoff radius beyond which forces no longer affect particles.
     *
     * @return The radius.
     */
    inline const double get_r_l() const { return r_l; }

    /**
     * Get the gravity pulling the atoms down.
     *
     * @return The gravity.
     */
    inline const double get_gravity() const { return gravity; }

    /**
     * Get the domain size of the simulation in form of a vector representing the size in {x, y, z} direction.
     *
     * @return The vector representing the size of the domain.
     */
    inline const Vec<double> get_domain_size() const { return domain_size; }

    /**
     * Get the number of dimensions of the simulation.
     *
     * @return The number of dimensions.
     */
    inline const int get_dimensions() const { return dimensions; }

    /**
     * Get the number of steps between thermometer applications.
     *
     * @return The application frequency.
     */
    inline const int get_temp_frequency() const { return temp_frequency; }


    // Setter methods

    /**
     * Set the end time (duration) of the simulation.
     *
     * @param t_end The end time of the simulation.
     */
    inline void set_t_end(const double t_end) { this->t_end = t_end; }

    /**
     * Set the amount of time simulated by a single step.
     *
     * @param delta_t The time delta for a single simulation step.
     */
    inline void set_delta_t(const double delta_t) { this->delta_t = delta_t; }

    /**
     * Set the sigma used for lenard jones simulations.
     *
     * @param sigma The sigma for force calculation.
     */
    inline void set_sigma(const double sigma) { this->sigma = sigma; }

    /**
     * Set the epsilon used for lenard jones simulations.
     *
     * @param epsilon The epsilon for force calculation.
     */
    inline void set_epsilon(const double epsilon) { this->epsilon = epsilon; }

    /**
     * Set how often the simulation output file should be generated.
     *
     * @param print_step An integer indicating which simulation steps should be saved.
     */
    inline void set_print_step(const int print_step) { this->print_step = print_step; }

    /**
     * Set the beginning of the name of the output file. All output files will start with this name.
     *
     * @param output_file_name The file name of the output file.
     */
    inline void set_output_file_name(const std::string& output_file_name) { this->output_file = output_file_name; }

    /**
     * Set the beginning of the name of the diffusion output. All diffusion files will start with this name.
     *
     * @param diff_file_name The file name of the diffusion output.
     */
    inline void set_diff_file_name(const std::string& diff_file_name) { this->diff_file = diff_file_name; }

    /**
     * Set wether the diffusion files should be created.
     *
     * @param generate_diff The boolean representing if the output should be created.
     */
    inline void set_generate_diff(const bool generate_diff) { this->generate_diff = generate_diff; }

    /**
     * Set the format of the the output file.
     *
     * @param output_format The file format of the output file.
     */
    inline void set_output_file_format(const OutputFormat output_format) { this->output_format = output_format; }

    /**
     * Set the number of buckets dividing used by the RDF writer to divide the domain.
     *
     * @param RDF_bucket_num The number of rdf buckets
     */
    inline void set_RDF_bucket_num(const size_t RDF_bucket_num) { this->RDF_bucket_num = RDF_bucket_num; }

    /**
     * Set the size of the buckets used by the RDF writer to divide the domain.
     *
     * @param RDF_bucket_size The size oft all buckets.
     */
    inline void set_RDF_bucket_size(const double RDF_bucket_size) { this->RDF_bucket_size = RDF_bucket_size; }

    /**
     * Set the calculator type which should be used for the force calculations.
     *
     * @param calculator_type The calculator type.
     */
    inline void set_calculator_type(const CalculatorType calculator_type) { this->calc = calculator_type; }

    /**
     * Set the update strategy used for parallel updating the cells.
     *
     * @param update_strategy The update strategy.
     */
    inline void set_update_strategy(const UpdateStrategy update_strategy) { this->strat = update_strategy; }

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
    inline void set_r_cutoff(const double r_cutoff) { this->r_cutoff = r_cutoff; }

    /**
     * Set the cutoff radius beyond which forces no longer affect particles.
     *
     * @param r_l The radius.
     */
    inline void set_r_l(const double r_l) { this->r_l = r_l; }

    /**
     * Set the domain size at whichs end the boundaries are located.
     *
     * @param domain_size Vector of domain size along the axes.
     */
    inline void set_domain_size(const Vec<double> domain_size) { this->domain_size = domain_size; }

    /**
     * Set the number of dimensions of the simulation.
     *
     * @param dimensions The number of dimensions.
     */
    inline void set_dimensions(const int dimensions) { this->dimensions = dimensions; }

    /**
     * Set the number of steps between thermometer applications.
     *
     * @param temp_frequency The application frequency.
     */
    inline void set_temp_frequency(const int temp_frequency) { this->temp_frequency = temp_frequency; }

    /**
     * Set the gravity pulling the atoms down.
     *
     * @param gravity The gravity pulling the atoms down.
     */
    inline void set_gravity(const double gravity) { this->gravity = gravity; }
};
