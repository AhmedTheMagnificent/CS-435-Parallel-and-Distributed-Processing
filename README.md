# Laplace Solver - Parallel and Distributed Processing

A high-performance implementation of a 2D Laplace equation solver using the Jacobi relaxation method, demonstrating parallel and distributed computing techniques with multiple parallelization strategies.

## Overview

This project implements the solution to the 2D Laplace equation on an N×N grid using the iterative Jacobi relaxation method. Three implementations are provided:

- **Serial**: Single-threaded reference implementation
- **OpenMP**: Shared-memory parallelization for multi-core systems
- **MPI**: Distributed-memory parallelization for multi-node clusters

## Project Structure

```
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── include/
│   └── laplace.hpp         # Header file with core solver interface
├── src/
│   ├── laplace.cpp         # Serial and OpenMP implementations
│   ├── mpi_laplace.cpp     # MPI distributed implementation
│   └── benchmark.cpp       # Benchmarking tool
├── scripts/
│   ├── run.slurm           # SLURM job submission script for HPC clusters
│   └── plot.py             # Python plotting utility for results
└── build/                  # Build artifacts directory
```

## Algorithm

The Jacobi relaxation method iteratively updates each interior grid point with the average of its four neighbors:

```
next[i,j] = 0.25 * (curr[i-1,j] + curr[i+1,j] + curr[i,j-1] + curr[i,j+1])
```

**Boundary conditions**: Top boundary = 1.0, all other boundaries = 0.0

## Dependencies

- **C++11** or later
- **CMake 2.8.12+** for building
- **OpenMP** for shared-memory parallelization
- **MPI** (OpenMPI or MPICH) for distributed computing
- **Python 3** (optional, for plotting)

### Installation

**Ubuntu/Debian:**
```bash
sudo apt-get install cmake libopenmpi-dev openmpi-bin build-essential
```

**macOS (with Homebrew):**
```bash
brew install cmake open-mpi
```

**HPC Clusters:**
Available via module system (see `run.slurm` for example)

## Building

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

This produces three executables:
- `benchmark` - Serial and OpenMP implementations
- `mpi_laplace` - MPI distributed implementation

## Usage

### Serial and OpenMP Benchmarks

```bash
./build/benchmark
```

Sets `OMP_NUM_THREADS` for multi-threaded execution. Configure thread count:

```bash
OMP_NUM_THREADS=8 ./build/benchmark
```

### MPI Execution

```bash
mpirun -np <num_processes> ./build/mpi_laplace <grid_size> <iterations>
```

**Example:**
```bash
mpirun -np 4 ./build/mpi_laplace 1024 2000
```

Solves a 1024×1024 grid over 2000 iterations using 4 MPI processes.

## HPC Cluster Submission

Submit to a SLURM-based HPC cluster:

```bash
sbatch scripts/run.slurm
```

The script:
- Requests 2 nodes with 8 tasks per node (16 total processes)
- Loads necessary modules (GCC, OpenMPI, CMake)
- Builds the project in Release mode
- Runs benchmarks on both OpenMP and MPI implementations

Monitor job status:
```bash
squeue -u $USER
```

## Performance Analysis

The project generates `timing.csv` containing execution times. Visualize results with:

```bash
python scripts/plot.py
```

Compare performance across:
- Serial baseline
- OpenMP with varying thread counts
- MPI with different process counts and grid sizes

## Output

Timing data is logged in CSV format showing:
- Grid size (N)
- Number of iterations
- Number of processes/threads
- Execution time (seconds)

## Implementation Details

### Serial (`solve_serial`)
- Baseline single-threaded implementation
- Used for correctness verification

### OpenMP (`solve_openmp`)
- Parallel loop using `#pragma omp parallel for`
- Loop collapsing for efficient work distribution
- Static scheduling for load balancing

### MPI (`mpi_laplace`)
- Domain decomposition across processes
- Distributed grid partitioning
- Halo exchange for boundary data

## Correctness Verification

The `max_diff()` function computes the maximum absolute difference between results from different implementations to ensure numerical consistency across parallelization strategies.

## Performance Considerations

- **Grid size**: Larger grids improve parallelization efficiency
- **Iteration count**: More iterations amortize communication overhead
- **Thread/Process count**: Optimal count depends on hardware
- **Network bandwidth**: Critical for MPI on slower interconnects

## Notes

- Compiled with `-O3 -march=native` optimization flags in Release mode
- C++11 standard (`-std=c++0x`)
- Flat row-major array storage for cache efficiency

## License

For course use in CS-435: Parallel and Distributed Processing

## References

- Jacobi Method: Classical iterative solver for PDEs
- OpenMP: https://www.openmp.org/
- MPI Standard: https://www.mpi-forum.org/
