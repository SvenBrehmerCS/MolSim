#include "Environment.h"

#include <cmath>
#include <cstring>
#include <float.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

#define btos(__BOOL) (static_cast<bool>(__BOOL) ? "true" : "false")

/**
 * Print an error message and exit immediately with EXIT_FAILURE.
 */
static void panic_exit(const char* message) {
    spdlog::critical(message);
    std::exit(EXIT_FAILURE);
}

Environment::Environment() { spdlog::trace("Initialized with a standard environment."); }

Environment::Environment(const int argc, const char* argv[]) {
    // Test if there is a request for a help message within all passed arguments
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--h") == 0 || std::strcmp(argv[i], "-help") == 0
            || std::strcmp(argv[i], "--help") == 0) {

            // Print the help message
            std::cout << "Usage:" << std::endl;
            std::cout << "    " << argv[0] << " <args> <input file>" << std::endl;
            std::cout << std::endl;
            std::cout << "These are the possible arguments:" << std::endl;
            std::cout << std::endl;
            std::cout << "    -h, --h, -help, --help" << std::endl;
            std::cout << "        Print a help message (this message) and exit." << std::endl;
            std::cout << std::endl;
            std::cout << "    -t_end=<end time>" << std::endl;
            std::cout << "        Set the time until which the simulation should run." << std::endl;
            std::cout << "        The end time must be a positive floating point number." << std::endl;
            std::cout << "        The default end time is 1000.0." << std::endl;
            std::cout << std::endl;
            std::cout << "    -delta_t=<time delta>" << std::endl;
            std::cout << "        Set the time delta with which the steps should be performed." << std::endl;
            std::cout << "        The delta time must be a strictly positive floating point number." << std::endl;
            std::cout << "        The default time delta is 0.014." << std::endl;
            std::cout << std::endl;
            std::cout << "    -sigma=<sigma>" << std::endl;
            std::cout << "        Set the sigma used for the lenard jones simulation." << std::endl;
            std::cout << "        The sigma must be a strictly positive floating point number." << std::endl;
            std::cout << "        The default sigma is 1.0." << std::endl;
            std::cout << std::endl;
            std::cout << "    -epsilon=<epsilon>" << std::endl;
            std::cout << "        Set the epsilon used for the lenard jones simulation." << std::endl;
            std::cout << "        The epsilon must be a strictly positive floating point number." << std::endl;
            std::cout << "        The default epsilon is 5.0." << std::endl;
            std::cout << std::endl;
            std::cout << "    -print_step=<print step>" << std::endl;
            std::cout << "        Set the print step with which the steps should be performed." << std::endl;
            std::cout << "        The print step must be a strictly positive integer." << std::endl;
            std::cout << "        The default print step is 10." << std::endl;
            std::cout << std::endl;
            std::cout << "    -out_name=<output file name>" << std::endl;
            std::cout << "        Set the beginning of the output file name as given." << std::endl;
            std::cout << "        The file name must be a string at least one character long." << std::endl;
            std::cout << "        The default output file name is MD_vtk." << std::endl;
            std::cout << std::endl;
            std::cout << "    -output_format=<file format>" << std::endl;
            std::cout << "        Set the format of the output file either to 'no', 'vtk' or to 'xyz'." << std::endl;
            std::cout << "        The file format must either be 'no' (disable writing files), 'vtk' or 'xyz'." << std::endl;
            std::cout << "        The default output file format is vtk." << std::endl;
            std::cout << std::endl;
            std::cout << "    -log_level=<log level>" << std::endl;
            std::cout << "        Set the log level of the program to a spdlog level." << std::endl;
            std::cout << "        The log level must be one off: off, crit, error, warn, info, debug, trace." << std::endl;
            std::cout << "        The default log level is info." << std::endl;
            std::cout << std::endl;
            std::cout << "    -calc=<force model>" << std::endl;
            std::cout << "        Set the force calculation of the program to a force model." << std::endl;
            std::cout << "        The force model can either be gravity or lj (lenard jones)." << std::endl;
            std::cout << "        The default force model is lj." << std::endl;
            std::cout << std::endl;
            std::cout << "Each argument may only be provided once. If no argument is provided the default" << std::endl;
            std::cout << "value is being used. There may not be any blank spaces seperating the option" << std::endl;
            std::cout << "and its value. The output files will be placed in the folder, from where the" << std::endl;
            std::cout << "program is executed." << std::endl;
            std::cout << std::endl;
            std::cout << "Example:" << std::endl;
            std::cout << "    " << argv[0] << " -t_end=10.0 -print_step=20 ./path/to/input.txt" << std::endl;
            std::cout << std::endl;
            std::cout << "This would run a simulation lasting for 10 time units, printing every 20th" << std::endl;
            std::cout << "iteration. The initial state would be given in the file input.txt, every other" << std::endl;
            std::cout << "parameter would have the default value." << std::endl;

            exit(EXIT_SUCCESS);
        }
    }

    // Initialize the booleans indicating if an argument was already parsed once
    bool default_end = true;
    bool default_delta = true;
    bool default_sigma = true;
    bool default_epsilon = true;
    bool default_print_step = true;
    bool default_out_name = true;
    bool default_file_format = true;
    bool default_log_level = true;
    bool default_calculator = true;

    // Parse all arguments but help.
    for (int i = 1; i < argc; i++) {
        if (std::strncmp(argv[i], "-t_end=", std::strlen("-t_end=")) == 0) {
            // Parse the end time
            if (default_end == false) {
                panic_exit("The option t_end was provided multiple times. Options may only be provided once.");
            }

            size_t idx = 0;

            try {
                t_end = std::stod(argv[i] + std::strlen("-t_end="), &idx);
            } catch (std::exception& e) {
                panic_exit("The option t_end requires a floatingpoint number within the region of a 64 bit float.");
            }

            if (argv[i][idx + std::strlen("-t_end=")] != 0) {
                panic_exit("The option t_end must only have one floating point number as input.");
            }

            if (t_end < 0.0) {
                panic_exit("The option t_end must have a positive value.");
            }

            if (std::isnan(t_end) || std::isinf(t_end)) {
                panic_exit("The option t_end must be a valid number, not NAN or INF.");
            }

            default_end = false;
        } else if (std::strncmp(argv[i], "-delta_t=", std::strlen("-delta_t=")) == 0) {
            // Parse the time delta
            if (default_delta == false) {
                panic_exit("The option delta_t was provided multiple times. Options may only be provided once.");
            }

            size_t idx = 0;

            try {
                delta_t = std::stod(argv[i] + std::strlen("-delta_t="), &idx);
            } catch (const std::exception& e) {
                panic_exit("The option delta_t requires a floatingpoint number within the region of a 64 bit float.");
            }

            if (argv[i][idx + std::strlen("-delta_t=")] != 0) {
                panic_exit("The option delta_t must only have one floating point number as input.");
            }

            if (t_end <= 0.0) {
                panic_exit("The option delta_t must have a strictly positive value.");
            }

            if (std::isnan(delta_t) || std::isinf(delta_t)) {
                panic_exit("The option delta_t must be a valid number, not NAN or INF.");
            }

            default_delta = false;
        } else if (std::strncmp(argv[i], "-sigma=", std::strlen("-sigma=")) == 0) {
            // Parse the time delta
            if (default_sigma == false) {
                panic_exit("The option sigma was provided multiple times. Options may only be provided once.");
            }

            size_t idx = 0;

            try {
                sigma = std::stod(argv[i] + std::strlen("-sigma="), &idx);
            } catch (const std::exception& e) {
                panic_exit("The option sigma requires a floatingpoint number within the region of a 64 bit float.");
            }

            if (argv[i][idx + std::strlen("-sigma=")] != 0) {
                panic_exit("The option sigma must only have one floating point number as input.");
            }

            if (sigma <= 0.0) {
                panic_exit("The option sigma must have a strictly positive value.");
            }

            if (std::isnan(sigma) || std::isinf(sigma)) {
                panic_exit("The option sigma must be a valid number, not NAN or INF.");
            }

            default_sigma = false;
        } else if (std::strncmp(argv[i], "-epsilon=", std::strlen("-epsilon=")) == 0) {
            // Parse the time delta
            if (default_epsilon == false) {
                panic_exit("The option epsilon was provided multiple times. Options may only be provided once.");
            }

            size_t idx = 0;

            try {
                epsilon = std::stod(argv[i] + std::strlen("-epsilon="), &idx);
            } catch (const std::exception& e) {
                panic_exit("The option epsilon requires a floatingpoint number within the region of a 64 bit float.");
            }

            if (argv[i][idx + std::strlen("-epsilon=")] != 0) {
                panic_exit("The option epsilon must only have one floating point number as input.");
            }

            if (epsilon <= 0.0) {
                panic_exit("The option epsilon must have a strictly positive value.");
            }

            if (std::isnan(epsilon) || std::isinf(epsilon)) {
                panic_exit("The option epsilon must be a valid number, not NAN or INF.");
            }

            default_epsilon = false;
        } else if (std::strncmp(argv[i], "-print_step=", std::strlen("-print_step=")) == 0) {
            // Parse the print steps
            if (default_print_step == false) {
                panic_exit("The option print_step was provided multiple times. Options may only be provided once.");
            }

            size_t idx = 0;

            try {
                print_step = std::stoi(argv[i] + std::strlen("-print_step="), &idx);
            } catch (const std::exception& e) {
                panic_exit("The option print_step requires an integer small enough to fit into an int.");
            }

            if (argv[i][idx + std::strlen("-print_step=")] != 0) {
                panic_exit("The option print_step must only have one integer as input.");
            }

            if (print_step <= 0) {
                panic_exit("The option print_step must have a strictly positive value.");
            }

            default_print_step = false;
        } else if (std::strncmp(argv[i], "-out_name=", std::strlen("-out_name=")) == 0) {
            // Parse the name of the output file
            if (default_out_name == false) {
                panic_exit("The option out_name was provided multiple times. Options may only be provided once.");
            }

            if (std::strlen(argv[i] + std::strlen("-out_name=")) == 0) {
                panic_exit("The length of the output file name must not be zero.");
            }

            output_file = argv[i] + std::strlen("-out_name=");

            default_out_name = false;
        } else if (std::strcmp(argv[i], "-output_format=no") == 0) {
            // Parse the output file format
            if (default_file_format == false) {
                panic_exit("The option output_format was provided multiple times. Options may only be provided once.");
            }

            output_format = NO_OUT;

            default_file_format = false;
        } else if (std::strcmp(argv[i], "-output_format=vtk") == 0) {
            // Parse the output file format
            if (default_file_format == false) {
                panic_exit("The option output_format was provided multiple times. Options may only be provided once.");
            }

            output_format = VTK;

            default_file_format = false;
        } else if (std::strcmp(argv[i], "-output_format=xyz") == 0) {
            // Parse the output file format
            if (default_file_format == false) {
                panic_exit("The option output_format was provided multiple times. Options may only be provided once.");
            }

            output_format = XYZ;

            default_file_format = false;
        } else if (std::strcmp(argv[i], "-log_level=off") == 0) {
            // Parse the log level
            if (default_log_level == false) {
                panic_exit("The option log_level was provided multiple times. Options may only be provided once.");
            }

            spdlog::set_level(spdlog::level::off);

            default_log_level = false;
        } else if (std::strcmp(argv[i], "-log_level=crit") == 0) {
            // Parse the output log level
            if (default_log_level == false) {
                panic_exit("The option log_level was provided multiple times. Options may only be provided once.");
            }

            spdlog::set_level(spdlog::level::critical);

            default_log_level = false;
        } else if (std::strcmp(argv[i], "-log_level=error") == 0) {
            // Parse the output log level
            if (default_log_level == false) {
                panic_exit("The option log_level was provided multiple times. Options may only be provided once.");
            }

            spdlog::set_level(spdlog::level::err);

            default_log_level = false;
        } else if (std::strcmp(argv[i], "-log_level=warn") == 0) {
            // Parse the output log level
            if (default_log_level == false) {
                panic_exit("The option log_level was provided multiple times. Options may only be provided once.");
            }

            spdlog::set_level(spdlog::level::warn);

            default_log_level = false;
        } else if (std::strcmp(argv[i], "-log_level=info") == 0) {
            // Parse the output log level
            if (default_log_level == false) {
                panic_exit("The option log_level was provided multiple times. Options may only be provided once.");
            }

            spdlog::set_level(spdlog::level::info);

            default_log_level = false;
        } else if (std::strcmp(argv[i], "-log_level=trace") == 0) {
            // Parse the log level
            if (default_log_level == false) {
                panic_exit("The option log_level was provided multiple times. Options may only be provided once.");
            }

            spdlog::set_level(spdlog::level::trace);

            default_log_level = false;
        } else if (std::strcmp(argv[i], "-log_level=debug") == 0) {
            // Parse the log level
            if (default_log_level == false) {
                panic_exit("The option log_level was provided multiple times. Options may only be provided once.");
            }

            spdlog::set_level(spdlog::level::debug);

            default_log_level = false;
        } else if (std::strcmp(argv[i], "-calc=gravity") == 0) {
            // Parse the calculator type
            if (default_calculator == false) {
                panic_exit("The option calc was provided multiple times. Options may only be provided once.");
            }

            calc = GRAVITY;

            default_calculator = false;
        } else if (std::strcmp(argv[i], "-calc=lj") == 0) {
            // Parse the calculator type
            if (default_calculator == false) {
                panic_exit("The option calc was provided multiple times. Options may only be provided once.");
            }

            calc = LJ_FULL;

            default_calculator = false;
        } else {
            // Parse the input file
            if (std::strlen(argv[i]) == 0) {
                panic_exit("The length of an argument must not be zero.");
            }

            if (argv[i][0] == '-') {
                panic_exit("An input file must not start with '-'. Check if all arguments are spelled correctly.");
            }

            if (input_file != nullptr) {
                panic_exit("One may only provide a single input file.");
            }

            input_file = argv[i];
        }
    }

    if (input_file == nullptr) {
        panic_exit("There was no input file provided.");
    }

    const char* temp = input_file + std::strlen(input_file) - 3;
    // Read the input file
    if (!(std::strcmp(temp, "txt"))) {
        input_format = TXT;
    } else if (!(std::strcmp(temp, "xml"))) {
        input_format = XML;
    } else {
        panic_exit("Unsupported input file type.");
    }

    spdlog::debug("The program was executed using the command line arguments.");
    spdlog::debug("    t_end = {} ({})", t_end, btos(default_end));
    spdlog::debug("    delta_t = {} ({})", delta_t, btos(default_delta));
    spdlog::debug("    sigma = {} ({})", sigma, btos(default_sigma));
    spdlog::debug("    epsilon = {} ({})", epsilon, btos(default_epsilon));
    spdlog::debug("    print_step = {} ({})", print_step, btos(default_print_step));
    spdlog::debug("    input_file = {}", input_file);
    spdlog::debug("    output_file = {} ({})", output_file, btos(default_out_name));
    spdlog::debug("    format = {} ({})", static_cast<int>(output_format), btos(default_file_format));
    spdlog::debug("    log_level = {} ({})", static_cast<int>(spdlog::get_level()), btos(default_log_level));
    spdlog::debug("    calc = {} ({})", static_cast<int>(calc), btos(default_calculator));
}

Environment::Environment(const Environment& env) {
    t_end = env.get_t_end();
    delta_t = env.get_delta_t();
    epsilon = env.get_epsilon();
    sigma = env.get_sigma();
    print_step = env.get_print_step();
    input_file = env.get_input_file_name();
    output_file = env.get_output_file_name();
    output_format = env.get_output_file_format();
    calc = env.get_calculator_type();
    r_cutoff = env.get_r_cutoff();
}

Environment::~Environment() = default;

const char* Environment::get_input_file_name() const { return input_file; }

InputFormat Environment::get_input_file_format() const { return input_format; }

const char* Environment::get_output_file_name() const { return output_file.c_str(); }

OutputFormat Environment::get_output_file_format() const { return output_format; }

CalculatorType Environment::get_calculator_type() const { return calc; }

std::array<BoundaryType, 6> Environment::get_boundary_type() const {
    return std::array<BoundaryType, 6> {
        yz_near,
        xz_near,
        xy_near,
        yz_far,
        xz_far,
        xy_far,
    };
}

int Environment::get_print_step() const { return print_step; }

double Environment::get_sigma() const { return sigma; }

double Environment::get_epsilon() const { return epsilon; }

double Environment::get_delta_t() const { return delta_t; }

double Environment::get_t_end() const { return t_end; }

double Environment::get_r_cutoff() const { return r_cutoff; }

std::array<double, 3> Environment::get_domain_size() const { return domain_size; }

void Environment::set_t_end(const double t_end) { this->t_end = t_end; }

void Environment::set_delta_t(const double delta_t) { this->delta_t = delta_t; }

void Environment::set_sigma(const double sigma) { this->sigma = sigma; }

void Environment::set_epsilon(const double epsilon) { this->epsilon = epsilon; }

void Environment::set_print_step(const int print_step) { this->print_step = print_step; }

void Environment::set_output_file_name(std::string& output_file_name) { this->output_file = output_file_name; }

void Environment::set_output_file_format(const OutputFormat output_format) { this->output_format = output_format; }

void Environment::set_calculator_type(const CalculatorType calculator_type) { this->calc = calculator_type; }

void Environment::set_boundary_type(const std::array<BoundaryType, 6> boundary_type) {
    yz_near = boundary_type[0];
    xz_near = boundary_type[1];
    xy_near = boundary_type[2];
    yz_far = boundary_type[3];
    xz_far = boundary_type[4];
    xy_far = boundary_type[5];
}

void Environment::set_r_cutoff(const double r_cutoff) { this->r_cutoff = r_cutoff; }

void Environment::set_domain_size(const std::array<double, 3> domain_size) { this->domain_size = domain_size; }
