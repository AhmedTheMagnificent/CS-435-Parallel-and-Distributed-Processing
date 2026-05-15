#!/bin/bash
#SBATCH --job-name=laplace
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=8
#SBATCH --cpus-per-task=1
#SBATCH --mem=8G
#SBATCH --time=00:30:00
#SBATCH --output=laplace_%j.out

module load gcc openmpi cmake

cd $SLURM_SUBMIT_DIR
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release && make -j$(nproc)
cd ..

echo "=== Serial + OpenMP ==="
OMP_NUM_THREADS=8 ./build/benchmark

echo ""
echo "=== MPI ==="
for N in 256 512 1024 2048; do
    mpirun -np $SLURM_NTASKS ./build/mpi_laplace $N 2000
done

