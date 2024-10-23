#include "Environment.h"

#include <cmath>
#include <cstring>
#include <float.h>
#include <iostream>
#include <string>

#define btos(__BOOL) (static_cast<bool>(#__BOOL) ? "true" : "false")

static void panic_exit(const char* message) {
    std::cout << message << std::endl;
    exit(EXIT_FAILURE);
}

Environment::Environment() { std::cout << "Initialized with a standard environment." << std::endl; }

Environment::Environment(const int argc, const char* argv[]) {
    for (int i = 0; i < argc; i++) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--h") == 0 || std::strcmp(argv[i], "-help") == 0
            || std::strcmp(argv[i], "--help") == 0) {
            std::cout << "Usage:" << std::endl;
            std::cout << "    " << argv[0] << " <args> <input file>" << std::endl;
            std::cout << std::endl;
            std::cout << "These are the possible arguments:" << std::endl;
            std::cout << std::endl;
            std::cout << "    -h, --h, -help, --help" << std::endl;
            std::cout << "        Print a help message (this message) and exit." << std::endl;
            std::cout << std::endl;
            std::cout << "    -t_end=<end time>" << std::endl;
            std::cout << "        Set the time until wich the simulation should run." << std::endl;
            std::cout << "        The end time must be a positive floating point number." << std::endl;
            std::cout << "        The default end time is 1000.0." << std::endl;
            std::cout << std::endl;
            std::cout << "    -delta_t=<time delta>" << std::endl;
            std::cout << "        Set the time delta with wich the steps should be performed." << std::endl;
            std::cout << "        The delta time must be a strictly positive floating point number." << std::endl;
            std::cout << "        The default time delta is 0.014." << std::endl;
            std::cout << std::endl;
            std::cout << "    -print_step=<print step>" << std::endl;
            std::cout << "        Set the print step with wich the steps should be performed." << std::endl;
            std::cout << "        The print step must be a strictly positive integer." << std::endl;
            std::cout << "        The default print step is 0.014." << std::endl;
            std::cout << std::endl;
            std::cout << "    -out_name=<output file name>" << std::endl;
            std::cout << "        Set the beginning of the output file name as given." << std::endl;
            std::cout << "        The file name must be a string at least one character long." << std::endl;
            std::cout << "        The default output file name is MD_vtk." << std::endl;
            std::cout << std::endl;
            std::cout << "    -output_format=<file format>" << std::endl;
            std::cout << "        Set the format of the output file either to 'vtk' or to 'xyz'." << std::endl;
            std::cout << "        The file format must either be 'vtk' or 'xyz'." << std::endl;
            std::cout << "        The default output file format is vtk." << std::endl;
            std::cout << std::endl;
            std::cout << "Each argument may only be provided once. If no argument is provided the default" << std::endl;
            std::cout << "value is being used. There may not be any blank spaces seperating the option" << std::endl;
            std::cout << "and its value. The output files will be placed in the folder, from where the" << std::endl;
            std::cout << "programm is executed. The output files will have the VTK format." << std::endl;
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
        } else if (std::strncmp(argv[i], "-print_step=", std::strlen("-print_step=")) == 0) {
            // Parse the print steps
            if (default_print_step == false) {
                panic_exit("The option print_step was provided multiple times. Options may only be provided once.");
            }

            size_t idx = 0;

            try {
                print_step = std::stoi(argv[i] + std::strlen("-delta_t="), &idx);
            } catch (const std::exception& e) {
                panic_exit("The option delta_t requires an integer small enough to fit into an int.");
            }

            if (argv[i][idx + std::strlen("-delta_t=")] != 0) {
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
        } else if (std::strcmp(argv[i], "-output_format=vtk") == 0) {
            // Parse the output file format
            if (default_file_format == false) {
                panic_exit("The option output_format was provided multiple times. Options may only be provided once.");
            }

            format = VTK;

            default_file_format = false;
        } else if (std::strcmp(argv[i], "-output_format=xyz") == 0) {
            // Parse the output file format
            if (default_file_format == false) {
                panic_exit("The option output_format was provided multiple times. Options may only be provided once.");
            }

            format = XYZ;

            default_file_format = false;
        } else {
            // Parse the input file
            if (std::strlen(argv[i]) == 0) {
                panic_exit("The length of an argument must not be zero.");
            }

            if (argv[i][0] == '-') {
                panic_exit("An inputfile must not start with '-'. Check if all arguments are spelled correctly.");
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

    std::cout << "The programm was executed using the command line arguments." << std::endl;
    std::cout << "    t_end = " << t_end << " (" << btos(default_end) << ")" << std::endl;
    std::cout << "    delta_t = " << delta_t << " (" << btos(default_delta) << ")" << std::endl;
    std::cout << "    print_step = " << print_step << " (" << btos(default_print_step) << ")" << std::endl;
    std::cout << "    input_file = " << input_file << std::endl;
    std::cout << "    output_file = " << output_file << "(" << btos(default_out_name) << ")" << std::endl;
}

Environment::~Environment() = default;

double Environment::get_t_end() const { return t_end; }

double Environment::get_delta_t() const { return delta_t; }

int Environment::get_print_step() const { return print_step; }

const char* Environment::get_input_file_name() const { return input_file; }

const char* Environment::get_output_file_name() const { return output_file; }

FileFormat Environment::get_output_file_format() const { return format; }
