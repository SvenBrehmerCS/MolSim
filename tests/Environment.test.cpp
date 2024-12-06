#include <Environment.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

// Test if the arguments are parsed correctly if only a path to an input file is provided
TEST(EnvironmentConstructor, EnvironmentOnlyPath1) {
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Test that the default values are correct
    EXPECT_DOUBLE_EQ(env.get_t_end(), 1000.0) << "The end time should be initialized to its default value.";
    EXPECT_DOUBLE_EQ(env.get_delta_t(), 0.014) << "The time delta should be initialized to its default value.";
    EXPECT_EQ(env.get_print_step(), 10) << "The print step should be initialized to its default value.";
    EXPECT_STREQ(env.get_output_file_name(), "MD_vtk") << "The file output name should be initialized to its default value.";
    EXPECT_EQ(env.get_output_file_format(), VTK) << "The output file format should be initialized to its default value.";
    EXPECT_EQ(env.get_epsilon(), 5.0) << "The epsilon should be initialized to its default value.";
    EXPECT_EQ(env.get_sigma(), 1.0) << "The sigma should be initialized to its default value.";
    EXPECT_EQ(spdlog::get_level(), spdlog::level::info) << "The log level should be initialized to its default value.";

    // Test that the parsed values are correct
    EXPECT_STREQ(env.get_input_file_name(), "path/to/input.txt") << "The input file path must be the same as provided.";
}

// Test if the arguments are parsed correctly if only a path to an input file is provided
TEST(EnvironmentConstructor, EnvironmentOnlyPath2) {
    const char* argv[] = {
        "../test/MolSim",
        "./path/to/input.xml",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Test that the default values are correct
    EXPECT_DOUBLE_EQ(env.get_t_end(), 1000.0) << "The end time should be initialized to its default value.";
    EXPECT_DOUBLE_EQ(env.get_delta_t(), 0.014) << "The time delta should be initialized to its default value.";
    EXPECT_EQ(env.get_print_step(), 10) << "The print step should be initialized to its default value.";
    EXPECT_STREQ(env.get_output_file_name(), "MD_vtk") << "The file output name should be initialized to its default value.";
    EXPECT_EQ(env.get_output_file_format(), VTK) << "The output file format should be initialized to its default value.";
    EXPECT_EQ(env.get_epsilon(), 5.0) << "The epsilon should be initialized to its default value.";
    EXPECT_EQ(env.get_sigma(), 1.0) << "The sigma should be initialized to its default value.";
    EXPECT_EQ(spdlog::get_level(), spdlog::level::info) << "The log level should be initialized to its default value.";

    // Test that the parsed values are correct
    EXPECT_STREQ(env.get_input_file_name(), "./path/to/input.xml") << "The input file path must be the same as provided.";
}

// Test if the arguments are parsed correctly if they are all passed with their default.
TEST(EnvironmentConstructor, EnvironmentAllDefault) {
    const char* argv[] = {
        "../test/MolSim",
        "-output_format=vtk",
        "-delta_t=0.014",
        "-print_step=10",
        "-t_end=1000.0",
        "-epsilon=5.0",
        "-log_level=info",
        "-out_name=MD_vtk",
        "./path/to/input.xml",
        "-sigma=1.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Test that the values are correct
    EXPECT_DOUBLE_EQ(env.get_t_end(), 1000.0) << "The end time should be initialized to its default value.";
    EXPECT_DOUBLE_EQ(env.get_delta_t(), 0.014) << "The time delta should be initialized to its default value.";
    EXPECT_EQ(env.get_print_step(), 10) << "The print step should be initialized to its default value.";
    EXPECT_STREQ(env.get_output_file_name(), "MD_vtk") << "The file output name should be initialized to its default value.";
    EXPECT_EQ(env.get_output_file_format(), VTK) << "The output file format should be initialized to its default value.";
    EXPECT_EQ(env.get_epsilon(), 5.0) << "The epsilon should be initialized to its default value.";
    EXPECT_EQ(env.get_sigma(), 1.0) << "The sigma should be initialized to its default value.";
    EXPECT_EQ(spdlog::get_level(), spdlog::level::info) << "The log level should be initialized to its default value.";
    EXPECT_STREQ(env.get_input_file_name(), "./path/to/input.xml") << "The input file path must be the same as provided.";
}

// Test if an error is thrown if no argument is provided
TEST(EnvironmentConstructor, EnvironmentNoInput) {
    const char* argv[] = {
        "./no/ms.out",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_EXIT(env = Environment(argc, argv), testing::ExitedWithCode(EXIT_FAILURE), "");
}

// Test if a help argument is handled correctly if it is the only argument
TEST(EnvironmentConstructor, EnvironmentHelpInput1) {
    const char* argv[] = {
        "../MolSim/ms",
        "--help",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_EXIT(env = Environment(argc, argv), testing::ExitedWithCode(EXIT_SUCCESS), "");
}

// Test if a help argument is handled correctly if it is one of many argument
TEST(EnvironmentConstructor, EnvironmentHelpInput2) {
    const char* argv[] = {
        "./../test/MolSim",
        "./path/to/error.txt",
        "-h",
        "-t_end=20.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_EXIT(env = Environment(argc, argv), testing::ExitedWithCode(EXIT_SUCCESS), "");
}

// Test if a help argument is handled correctly if the argument set is illegal
TEST(EnvironmentConstructor, EnvironmentHelpInput3) {
    const char* argv[] = {
        "./../test/MolSim",
        "-t_end=7.0",
        "./path/to/error.txt",
        "-t_end=20.0",
        "-help",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_EXIT(env = Environment(argc, argv), testing::ExitedWithCode(EXIT_SUCCESS), "");
}

// Test if a help argument is handled correctly if it is provided multiple times
TEST(EnvironmentConstructor, EnvironmentHelpInput4) {
    const char* argv[] = {
        "./../test/MolSim",
        "-t_end=7.0",
        "--h",
        "./path/to/error.txt",
        "-sigma=3.0",
        "-help",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_EXIT(env = Environment(argc, argv), testing::ExitedWithCode(EXIT_SUCCESS), "");
}

// Test if a normal set of arguments is handled correctly
TEST(EnvironmentConstructor, EnvironmentValidArguments1) {
    const char* argv[] = {
        "./../test/MolSim",
        "-t_end=22.3",
        "./input/file.txt",
        "-epsilon=7.0",
        "-delta_t=1",
        "-output_format=no",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Test that the default values are correct
    EXPECT_EQ(env.get_print_step(), 10) << "The print step should be initialized to its default value.";
    EXPECT_STREQ(env.get_output_file_name(), "MD_vtk") << "The file output name should be initialized to its default value.";
    EXPECT_EQ(env.get_sigma(), 1.0) << "The sigma should be initialized to its default value.";
    EXPECT_EQ(spdlog::get_level(), spdlog::level::info) << "The log level should be initialized to its default value.";

    // Test that the parsed values are correct
    EXPECT_DOUBLE_EQ(env.get_t_end(), 22.3) << "The end time must be the same as provided.";
    EXPECT_DOUBLE_EQ(env.get_delta_t(), 1) << "The time delta must be the same as provided.";
    EXPECT_EQ(env.get_output_file_format(), NO_OUT) << "The output file format must be the same as provided.";
    EXPECT_EQ(env.get_epsilon(), 7.0) << "The epsilon must have the same value as provided.";
    EXPECT_STREQ(env.get_input_file_name(), "./input/file.txt") << "The input file path must be the same as provided.";
}

// Test if a normal set of arguments is handled correctly
TEST(EnvironmentConstructor, EnvironmentValidArguments2) {
    const char* argv[] = {
        "./../test/MolSim",
        "-out_name=Test",
        "-output_format=vtk",
        "-print_step=100",
        "-delta_t=1E-8",
        "-epsilon=2.2",
        "../input/main_text_file.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Test that the default values are correct
    EXPECT_DOUBLE_EQ(env.get_t_end(), 1000.0) << "The end time should be initialized to its default value.";
    EXPECT_DOUBLE_EQ(env.get_sigma(), 1.0) << "The sigma should be initialized to its default value.";
    EXPECT_EQ(spdlog::get_level(), spdlog::level::info) << "The log level must be the same as provided.";

    // Test that the parsed values are correct
    EXPECT_DOUBLE_EQ(env.get_delta_t(), 1E-8) << "The time delta must be the same as provided.";
    EXPECT_EQ(env.get_print_step(), 100) << "The print step must be the same as provided.";
    EXPECT_STREQ(env.get_output_file_name(), "Test") << "The file output name must be the same as provided.";
    EXPECT_EQ(env.get_output_file_format(), VTK) << "The output file format must be the same as provided.";
    EXPECT_STREQ(env.get_input_file_name(), "../input/main_text_file.txt") << "The input file path must be the same as provided.";
    EXPECT_DOUBLE_EQ(env.get_epsilon(), 2.2) << "The epsilon must be the same as provided.";
}

// Test if all arguments at once are handled correctly
TEST(EnvironmentConstructor, EnvironmentAllArgumentsValid) {
    const char* argv[] = {
        "./msim.exe",
        "-out_name=res/help/test",
        "-output_format=xyz",
        "-t_end=12345.54321E3",
        "-print_step=12",
        "-delta_t=0.0001",
        "-log_level=warn",
        "-sigma=3",
        "-epsilon=1E5",
        "../input/main_text_file.xml",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Test that the parsed values are correct
    EXPECT_DOUBLE_EQ(env.get_t_end(), 12345.54321E3) << "The end time must be the same as provided.";
    EXPECT_DOUBLE_EQ(env.get_delta_t(), 0.0001) << "The time delta must be the same as provided.";
    EXPECT_EQ(env.get_sigma(), 3) << "The sigma must be the same as provided.";
    EXPECT_EQ(env.get_epsilon(), 1E5) << "The epsilon must be the same as provided.";
    EXPECT_EQ(env.get_print_step(), 12) << "The print step must be the same as provided.";
    EXPECT_STREQ(env.get_output_file_name(), "res/help/test") << "The file output name must be the same as provided.";
    EXPECT_EQ(env.get_output_file_format(), XYZ) << "The output file format must be the same as provided.";
    EXPECT_STREQ(env.get_input_file_name(), "../input/main_text_file.xml") << "The input file path must be the same as provided.";
    EXPECT_EQ(spdlog::get_level(), spdlog::level::warn) << "The log level must be the same as provided.";
}

// Test if duplicate -t_end is recognized
TEST(EnvironmentConstructor, EnvironmentDuplicateTEnd) {
    const char* argv[] = {
        "./msim.exe",
        "-t_end=1.7",
        "-output_format=xyz",
        "-t_end=1E4",
        "../input/main_text_file.xml",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_EXIT(env = Environment(argc, argv), testing::ExitedWithCode(EXIT_FAILURE), "");
}

// Test if float -print_step is recognized
TEST(EnvironmentConstructor, EnvironmentFloatPrintStep) {
    const char* argv[] = {
        "./msim.exe",
        "-t_end=0.3",
        "-delta_t=100.0",
        "-print_step=0.5",
        "../input/ignore.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_EXIT(env = Environment(argc, argv), testing::ExitedWithCode(EXIT_FAILURE), "");
}

// Test if duplicate -log_level is recognized
TEST(EnvironmentConstructor, EnvironmentDuplicateLogLevel) {
    const char* argv[] = {
        "./msim.exe",
        "-log_level=info",
        "-sigma=100.0",
        "-epsilon=0.5",
        "-log_level=off",
        "../input/ignore.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_EXIT(env = Environment(argc, argv), testing::ExitedWithCode(EXIT_FAILURE), "");
}
