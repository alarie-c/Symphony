cmake_minimum_required(VERSION 3.10...3.30)
project(SYMPH-C LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Werror -Wall -Wextra -Wpedantic)

# ------------------- Generate .a ------------------- #
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather all .cpp files in src/, then exclude main.cpp
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
set(SYMPH_SOURCES ${SOURCES})
list(FILTER SYMPH_SOURCES EXCLUDE REGEX ".*/main.cpp$")

# Build symph library from all logic except main
add_library(symph STATIC ${SYMPH_SOURCES})
target_include_directories(symph PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ------------------- Create `symphc` Target ------------------- #
add_executable(symphc src/main.cpp)
target_link_libraries(symphc PRIVATE symph)

# ------------------- Create `tests` Target -------------------- #
file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS "tests/*.cpp")
add_executable(tests ${TESTS})
target_link_libraries(tests PRIVATE symph)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/tests)