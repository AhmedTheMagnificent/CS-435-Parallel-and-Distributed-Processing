#include "laplace.hpp"
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <vector>

int main() {
    // Grid sizes and corresponding epochs to test
    const std::vector<int> sizes  = {128, 256, 512, 1024};
    const std::vector<int> epochs = {500, 1000, 2000, 4000};

    // Correctness check on small grid
    {
        Grid s = solve_serial(64, 200);
        Grid o = solve_openmp(64, 200);
        std::cout << "Correctness check (N=64): max_diff = " << max_diff(s, o) << "\n\n";
    }

    std::ofstream csv("timing.csv");
    csv << "N,serial_s,openmp_s\n";

    std::cout << "N\t\tSerial(s)\tOpenMP(s)\tSpeedup\n";
    std::cout << "------------------------------------------------------\n";

    // Declare timer structs here
    struct timeval start_time, end_time;

    for (size_t k = 0; k < sizes.size(); ++k) {
        int N = sizes[k];
        int ep = epochs[k];

        // --- Time the serial version ---
        gettimeofday(&start_time, NULL);
        solve_serial(N, ep);
        gettimeofday(&end_time, NULL);
        double ts = (end_time.tv_sec - start_time.tv_sec) + 
                    (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

        // --- Time the OpenMP version ---
        gettimeofday(&start_time, NULL);
        solve_openmp(N, ep);
        gettimeofday(&end_time, NULL);
        double to = (end_time.tv_sec - start_time.tv_sec) + 
                    (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

        std::cout << N << "\t\t" << ts << "\t\t" << to << "\t\t" << ts/to << "x\n";
        csv << N << "," << ts << "," << to << "\n";
    }

    std::cout << "\nResults saved to timing.csv\n";
    return 0;
}
