#include "laplace.hpp"
#include <algorithm>
#include <cmath>

#ifdef _OPENMP
#include <omp.h>
#endif

void init(Grid& g, int N) {
    g.assign(N * N, 0.0);
    for (int j = 0; j < N; ++j)
        g[j] = 1.0; // top boundary
}

Grid solve_serial(int N, int epochs) {
    Grid curr(N * N), next(N * N);
    init(curr, N);
    init(next, N);

    for (int iter = 0; iter < epochs; ++iter) {
        for (int i = 1; i < N - 1; ++i)
            for (int j = 1; j < N - 1; ++j)
                next[i*N+j] = 0.25 * (curr[(i-1)*N+j] + curr[(i+1)*N+j] +
                                       curr[i*N+j-1]   + curr[i*N+j+1]);
        std::swap(curr, next);
    }
    return curr;
}

Grid solve_openmp(int N, int epochs) {
    Grid curr(N * N), next(N * N);
    init(curr, N);
    init(next, N);

    for (int iter = 0; iter < epochs; ++iter) {
        #pragma omp parallel for collapse(2) schedule(static)
        for (int i = 1; i < N - 1; ++i)
            for (int j = 1; j < N - 1; ++j)
                next[i*N+j] = 0.25 * (curr[(i-1)*N+j] + curr[(i+1)*N+j] +
                                       curr[i*N+j-1]   + curr[i*N+j+1]);
        std::swap(curr, next);
    }
    return curr;
}

double max_diff(const Grid& a, const Grid& b) {
    double d = 0.0;
    for (size_t i = 0; i < a.size(); ++i)
        d = std::max(d, std::abs(a[i] - b[i]));
    return d;
}

