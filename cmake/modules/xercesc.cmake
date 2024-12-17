include(FetchContent)

set(XERCESC_VERSION "3.3.0")

FetchContent_Declare(
        xercesc
        GIT_REPOSITORY https://github.com/apache/xerces-c.git
        GIT_TAG v${XERCESC_VERSION}
)