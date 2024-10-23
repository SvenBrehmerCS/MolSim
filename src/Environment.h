#pragma once

enum FileFormat : int {
    VTK = 1,
    XYZ = 2,
};

class Environment {
private:
    double t_end = 1000.0, delta_t = 0.014;
    bool default_end = true, default_delta = true;
    int print_step = 10;
    bool default_print_step = true;

    char* input_file = nullptr;
    const char* output_file = "MD_vtk";
    bool default_out_name = true;

    FileFormat format = VTK;
    bool default_file_format = true;

public:
    Environment();

    Environment(const int argc, char* argv[]);

    ~Environment();

    double get_t_end() const;

    double get_delta_t() const;

    int get_print_step() const;

    char* get_input_file_name() const;

    const char* get_output_file_name() const;

    FileFormat get_output_file_format() const;
};
