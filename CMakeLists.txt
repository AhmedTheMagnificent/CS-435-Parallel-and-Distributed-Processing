cmake_minimum_required(VERSION 2.8.12)
project(LaplaceSolver CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

add_library(laplace src/laplace.cpp)
target_include_directories(laplace PUBLIC include)

add_executable(benchmark src/benchmark.cpp)
target_link_libraries(benchmark laplace)

add_executable(mpi_laplace src/mpi_laplace.cpp)
target_include_directories(mpi_laplace PRIVATE include ${MPI_CXX_INCLUDE_PATH} ${MPI_C_INCLUDE_PATH})
target_link_libraries(mpi_laplace laplace ${MPI_CXX_LIBRARIES} ${MPI_C_LIBRARIES})
