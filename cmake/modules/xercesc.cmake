include(FetchContent)

set(XERCESC_VERSION "3.3.0")

FetchContent_Declare(
        xercesc
        GIT_REPOSITORY https://github.com/apache/xerces-c.git
        GIT_TAG v${XERCESC_VERSION}
)

FetchContent_MakeAvailable(xercesc)

set(XercesC_INCLUDE_DIR ${xercesc_SOURCE_DIR}/src)
set(XercesC_LIBRARY xerces-c)

add_library(XercesC::XercesC INTERFACE IMPORTED)
target_include_directories(XercesC::XercesC INTERFACE ${XercesC_INCLUDE_DIR})
target_link_libraries(XercesC::XercesC INTERFACE xerces-c)