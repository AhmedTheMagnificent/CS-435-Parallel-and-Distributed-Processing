#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>


static void init(std::vector<double>& g, int N, int rank) {
    std::fill(g.begin(), g.end(), 0.0);
    if (rank == 0)
        for (int j = 0; j < N; ++j)
            g[1*N + j] = 1.0; // top boundary lives in rank 0's first real row
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    const int N      = argc > 1 ? atoi(argv[1]) : 512;
    const int epochs = argc > 2 ? atoi(argv[2]) : 2000;

    // Distribute rows evenly; first (N % nprocs) ranks get one extra row
    const int local_rows = N / nprocs + (rank < N % nprocs ? 1 : 0);
    const int row_start  = rank * (N / nprocs) + std::min(rank, N % nprocs);

    // Buffer: local_rows real rows + 1 ghost row top + 1 ghost row bottom
    const int buf = local_rows + 2;
    std::vector<double> curr(buf * N), next(buf * N);
    init(curr, N, rank);
    init(next, N, rank);

    const int north = (rank > 0)          ? rank - 1 : MPI_PROC_NULL;
    const int south = (rank < nprocs - 1) ? rank + 1 : MPI_PROC_NULL;

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    for (int iter = 0; iter < epochs; ++iter) {

        // --- Halo exchange ---
        // Send first real row north; receive from south into bottom ghost
        MPI_Sendrecv(&curr[1*N],              N, MPI_DOUBLE, north, 0,
                     &curr[(local_rows+1)*N], N, MPI_DOUBLE, south, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Send last real row south; receive from north into top ghost
        MPI_Sendrecv(&curr[local_rows*N], N, MPI_DOUBLE, south, 1,
                     &curr[0],           N, MPI_DOUBLE, north, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // --- Jacobi update ---
        std::copy(curr.begin(), curr.end(), next.begin());

        for (int i = 1; i <= local_rows; ++i) {
            const int global_i = row_start + (i - 1);
            if (global_i == 0 || global_i == N - 1) continue; // skip boundary rows

            for (int j = 1; j < N - 1; ++j)
                next[i*N+j] = 0.25 * (curr[(i-1)*N+j] + curr[(i+1)*N+j] +
                                       curr[i*N+j-1]   + curr[i*N+j+1]);
        }

        std::swap(curr, next);
    }

    double elapsed = MPI_Wtime() - t_start;

    // Gather max elapsed time across all ranks
    double max_time;
    MPI_Reduce(&elapsed, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
        std::cout << "MPI Laplace: N=" << N << " epochs=" << epochs
                  << " nprocs=" << nprocs << " time=" << max_time << "s\n";

    MPI_Finalize();
    return 0;
}

