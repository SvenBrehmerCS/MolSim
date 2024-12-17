#
# The doxygen file has these options:
#   NO_DOC - Disable the doxygen documentation generation if NO_DOC=ON.
#
cmake_minimum_required(VERSION 3.10)

# define project name, version
project(PSEMolDyn_GroupB VERSION 0.0.1)

# let ccmake and cmake-gui offer the default build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

# Update the compile options for the release mode
# TODO: For valgrind profiling -march=native must be disabled.
# TODO: Additional useful compiler options could be: -march=native, -fprofile-generate, -fprofile-use (+ -fprofile-fix)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto=auto -fomit-frame-pointer -mtune=native")

# let ccmake and cmake-gui offer the default log levels TODO:
set(LOG_LEVEL "INFO" CACHE STRING "Set the log level. This must either be OFF, CRITICAL, ERROR, WARN, INFO, DEBUG or TRACE.") 
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_${LOG_LEVEL})

# set Release as the default build type if it is not yet set.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# collect all cpp files
file(GLOB_RECURSE MY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    # header don't need to be included but this might be necessary for some IDEs
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# collect all test.cpp files
file(GLOB_RECURSE MY_TEST
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    # header don't need to be included but this might be necessary for some IDEs
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h"
)

# Get the cpp files that should be tested
file(GLOB_RECURSE MY_TEST_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    # header don't need to be included but this might be necessary for some IDEs
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

list(REMOVE_ITEM MY_TEST_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MolSim.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MolSim.h)

# create make target
add_executable(MolSim ${MY_SRC})

# set cxx standard. You may raise this if you want.
target_compile_features(MolSim
        PUBLIC
            cxx_std_17
)

target_include_directories(MolSim
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_link_libraries(MolSim
        # stuff that is used in headers and source files
        PUBLIC
        XercesC::XercesC
)

# activate all compiler warnings. Clean up your code :P
# depending on the compiler different flags are used
target_compile_options(MolSim
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-Wall -Werror>
            # disable some spam warnings for icpc...
            $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

include(xercesc)

# Generate the doxygen make option
include(doxygen)

# Include the google test module
include(gtest)
enable_testing()

# Add and link the google test
add_executable(MolTest
        ${MY_TEST}
        ${MY_TEST_SRC}
)

target_include_directories(MolTest
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MolTest PRIVATE GTest::gtest_main PUBLIC XercesC::XercesC PUBLIC spdlog::spdlog)

include(GoogleTest)
gtest_discover_tests(MolTest)


#include the spdlog module
include(spdlog)
# message(STATUS "Using libxsd from: ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd")
target_include_directories(MolSim
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
)
target_link_libraries(MolSim PRIVATE spdlog::spdlog)

# Show the compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
