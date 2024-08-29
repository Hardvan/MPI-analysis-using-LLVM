#include <mpi.h>
#include <stdio.h>

/*
Process communication:
- Process 0 sends data to Process 1 with tag 0
- Process 0 sends data to Process 2 with tag 0
- Process 1 receives data from Process 0 with tag 0
- Process 2 receives data from Process 0 with tag 0
*/

// MPI_Send format: MPI_Send(buffer, count, datatype, destination, tag, communicator)
// MPI_Recv format: MPI_Recv(buffer, count, datatype, source, tag, communicator, status)

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the rank of the process in MPI_COMM_WORLD
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // --------------------------------------------
    // Communication in MPI_COMM_WORLD (Default Communicator)
    // --------------------------------------------

    if (rank == 0) // Process 0
    {
        int data = 100;                                    // Data to send
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); // Process 0 sends data to Process 1 with tag 0
        MPI_Send(&data, 1, MPI_INT, 2, 0, MPI_COMM_WORLD); // Process 0 sends data to Process 2 with tag 0
    }
    else if (rank == 1) // Process 1
    {
        int data;                                                             // Buffer to receive data
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Process 1 receives data from Process 0 with tag 0
        printf("Process 1 received data: %d\n", data);
    }
    else if (rank == 2) // Process 2
    {
        int data; // Buffer to receive data
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}

/*
🚀 Analyzing Uniform Participation of MPI Processes Using LLVM/Clang 🚀

🔍 Step 1: Compiling mpi_example2.c to LLVM IR (input.ll)
⛓️  Executing: clang -I/usr/include/lam -emit-llvm -S mpi_example2.c -o input.ll
✅ Compiled in 36 ms

🔍 Step 2: Compiling MPIAnalysisPass.cpp to shared object (MPIAnalysisPass.so)
⛓️  Executing: clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp `llvm-config --cxxflags --ldflags --libs`
✅ Compiled in 3472 ms

🔍 Step 3: Running mpi-analysis pass on input.ll
⛓️  Executing: opt -load-pass-plugin=./MPIAnalysisPass.so -passes="mpi-analysis" < input.ll > /dev/null
MPIAnalysisPass running on function: main
[INFO] Detected MPI MPI_Send: comm=MPI_COMM_WORLD, tag=0, rank=1
[INFO] Detected MPI MPI_Send: comm=MPI_COMM_WORLD, tag=0, rank=2
[INFO] Detected MPI MPI_Recv: comm=MPI_COMM_WORLD, tag=0, rank=0
[INFO] Detected MPI MPI_Recv: comm=MPI_COMM_WORLD, tag=0, rank=0
[INFO] Analyzing Uniform Participation Patterns...
[INFO] Uniform Participation Detected in Comm MPI_COMM_WORLD with Tag 0 involving Ranks: 0 1 2
Uniform Participation Report:
------------------------------------
- Communicator: MPI_COMM_WORLD
- Tag: 0
- Participating Ranks: {0, 1, 2}
This indicates that both MPI_Send and MPI_Recv operations with tag 0 in communicator
MPI_COMM_WORLD involve these ranks.

✅ Ran mpi-analysis pass on input.ll in 18 ms
*/
