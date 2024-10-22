# if BUILD_DOC is activated, the documentation will be generated with 'make doc_doxygen', because doc_doxygen specifies the add_custom_target
# which is implemented below, doxygen will generate a new folder /docs in /build where the documentation can be found, /build directory is referred to as
# CMAKE_CURRENT_BINARY_DIR

# Option to build documentation (default is ON) turn off with cmake -DBUILD_DOC=OFF .. and then make
# make doxygen optional
option(BUILD_DOC "DOCUMENTATION WITH DOXYGEN" ON)
# Check if Doxygen is available
find_package(Doxygen)
if (DOXYGEN_FOUND)
    if (BUILD_DOC)
        # Path to the Doxyfile relative to the current source directory
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
        # Path to the doxygen output file in /build/docs
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)
        # Custom target to run Doxygen
        add_custom_target(
                doc_doxygen
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        #exclude the doxygen target from the all / default target.
        set_target_properties(doc_doxygen PROPERTIES EXCLUDE_FROM_ALL TRUE)
    else()
        message("DOXYGEN documentation will not be generated (BUILD_DOC is OFF), turn on with cmake -DBUILD_DOC=ON ..")
    endif()
else()
    message("Doxygen not found, documentation target will not be created, INSTALL with 'sudo apt install doxygen'")
endif()