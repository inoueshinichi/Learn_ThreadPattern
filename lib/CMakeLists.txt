cmake_minimum_required(VERSION 3.14.0)
message(STATUS "========== [START] thread_pattern library ==========")

# Include
list(APPEND THREAD_PATTERN_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
message(STATUS "ThreadPattern Module Include Dir: ${THREAD_PATTERN_INCLUDE_DIR}")

# Source
# file(GLOB THREAD_PATTERN_SOURCES ${PROJECT_SOURCE_DIR}/lib/*.cpp)

# Include File
file(GLOB THREAD_PATTERB_INCLUDES ${PROJECT_SOURCE_DIR}/include/*.hpp)

# header-only libarary
add_library(threadlib INTERFACE)
target_include_directories(threadlib INTERFACE ${THREAD_PATTERN_INCLUDE_DIR})
# target_sources(thread_pattern PRIVATE ${THREAD_PATTERN_SOURCES})

message(STATUS "========== [END] thread_pattern library ==========")

