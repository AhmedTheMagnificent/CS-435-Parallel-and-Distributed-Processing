#pragma once
#include <vector>

// Grid is a flat row-major N×N array
typedef std::vector<double> Grid;

// Initialise grid: top row = 1.0, everything else = 0.0
void init(Grid& g, int N);

// Jacobi relaxation — single-threaded reference
Grid solve_serial(int N, int epochs);

// Jacobi relaxation — OpenMP parallel
Grid solve_openmp(int N, int epochs);

// Max absolute difference between two grids (for correctness check)
double max_diff(const Grid& a, const Grid& b);

