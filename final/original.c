#include <mpi.h>
#include <stdio.h>

/*
Process communication:
- Process 0 sends data to Process 1 with tag 0
- Process 1 receives data from Process 0 with tag 0
*/

// MPI_Send format: MPI_Send(buffer, count, datatype, destination, tag, communicator)
// MPI_Recv format: MPI_Recv(buffer, count, datatype, source, tag, communicator, status)

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        int data = 100;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        int data;
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: %d\n", data);
    }

    MPI_Finalize();
    return 0;
}

/*
OUTPUT:
🚀 Analyzing Uniform Participation of MPI Processes Using LLVM/Clang 🚀

🔍 Step 1: Compiling original.c to LLVM IR (input.ll)
⛓️  Executing: clang -I/usr/include/lam -emit-llvm -S original.c -o input.ll
✅ Compiled in 83 ms

🔍 Step 2: Compiling MPIAnalysisPass.cpp to shared object (MPIAnalysisPass.so)
⛓️  Executing: clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp `llvm-config --cxxflags --ldflags --libs`
✅ Compiled in 3448 ms

🔍 Step 3: Running mpi-analysis pass on input.ll
⛓️  Executing: opt -load-pass-plugin=./MPIAnalysisPass.so -passes="mpi-analysis" < input.ll > /dev/null
MPIAnalysisPass running on function: main
[INFO] Detected MPI MPI_Send: comm=MPI_COMM_WORLD, tag=0, rank=1
[INFO] Detected MPI MPI_Recv: comm=MPI_COMM_WORLD, tag=0, rank=0
[INFO] Analyzing Uniform Participation Patterns...
[INFO] Uniform Participation Detected in Comm MPI_COMM_WORLD with Tag 0 involving Ranks: 0 1
Uniform Participation Report:
------------------------------------
- Communicator: MPI_COMM_WORLD
- Tag: 0
- Participating Ranks: {0, 1}
This indicates that both MPI_Send and MPI_Recv operations with tag 0 in communicator
MPI_COMM_WORLD involve these ranks.

✅ Ran mpi-analysis pass on input.ll in 25 ms
*/
