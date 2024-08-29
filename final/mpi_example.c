#include <mpi.h>
#include <stdio.h>

/*
Process communication:
- Process 0 sends data to Process 1 with tag 0
- Process 1 receives data from Process 0 with tag 0

- Process 2 sends data to Process 3 with tag 1
- Process 3 receives data from Process 2 with tag 1

- Process 4 sends data to Process 5 with tag 2
- Process 5 receives data from Process 4 with tag 2

- Process 6 sends data to Process 7 with tag 3
- Process 7 receives data from Process 6 with tag 3
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
    }
    else if (rank == 1) // Process 1
    {
        int data;                                                             // Buffer to receive data
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Process 1 receives data from Process 0 with tag 0
        printf("Process 1 received data: %d\n", data);
    }

    // --------------------------------------------
    // Custom Communicator (custom_comm)
    // --------------------------------------------
    // Split MPI_COMM_WORLD into two groups: even ranks & odd ranks
    // Each group forms a new communicator

    MPI_Comm custom_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &custom_comm);

    if (rank == 2) // Process 2
    {
        int data = 200;                                 // Data to send
        MPI_Send(&data, 1, MPI_INT, 3, 1, custom_comm); // Process 2 sends data to Process 3 with tag 1 within custom_comm
    }
    else if (rank == 3) // Process 3
    {
        int data;                                                          // Buffer to receive data
        MPI_Recv(&data, 1, MPI_INT, 2, 1, custom_comm, MPI_STATUS_IGNORE); // Process 3 receives data from Process 2 with tag 1 within custom_comm
        printf("Process 3 received data: %d\n", data);
    }

    // --------------------------------------------
    // Adding More Processes with MPI_COMM_WORLD
    // --------------------------------------------

    if (rank == 4) // Process 4
    {
        int data = 300;                                    // Data to send
        MPI_Send(&data, 1, MPI_INT, 5, 2, MPI_COMM_WORLD); // Process 4 sends data to Process 5 with tag 2
    }
    else if (rank == 5) // Process 5
    {
        int data;                                                             // Buffer to receive data
        MPI_Recv(&data, 1, MPI_INT, 4, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Process 5 receives data from Process 4 with tag 2
        printf("Process 5 received data: %d\n", data);
    }

    // --------------------------------------------
    // Additional Custom Communicator (custom_comm2)
    // --------------------------------------------
    // Split MPI_COMM_WORLD into three groups based on rank mod 3
    // Each group forms a new communicator

    MPI_Comm custom_comm2;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 3, rank, &custom_comm2);

    if (rank == 6) // Process 6
    {
        int data = 400;                                  // Data to send
        MPI_Send(&data, 1, MPI_INT, 7, 3, custom_comm2); // Process 6 sends data to Process 7 with tag 3 within custom_comm2
    }
    else if (rank == 7) // Process 7
    {
        int data;                                                           // Buffer to receive data
        MPI_Recv(&data, 1, MPI_INT, 6, 3, custom_comm2, MPI_STATUS_IGNORE); // Process 7 receives data from Process 6 with tag 3 within custom_comm2
        printf("Process 7 received data: %d\n", data);
    }

    // Free the communicators before finalizing
    MPI_Comm_free(&custom_comm);
    MPI_Comm_free(&custom_comm2);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}

/*
OUTPUT:
🚀 Analyzing Uniform Participation of MPI Processes Using LLVM/Clang 🚀

🔍 Step 1: Compiling mpi_example.c to LLVM IR (input.ll)
⛓️  Executing: clang -I/usr/include/lam -emit-llvm -S mpi_example.c -o input.ll
✅ Compiled in 40 ms

🔍 Step 2: Compiling MPIAnalysisPass.cpp to shared object (MPIAnalysisPass.so)
⛓️  Executing: clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp `llvm-config --cxxflags --ldflags --libs`
✅ Compiled in 3531 ms

🔍 Step 3: Running mpi-analysis pass on input.ll
⛓️  Executing: opt -load-pass-plugin=./MPIAnalysisPass.so -passes="mpi-analysis" < input.ll > /dev/null
MPIAnalysisPass running on function: main
[INFO] Detected MPI MPI_Send: comm=MPI_COMM_WORLD, tag=0, rank=1
[INFO] Detected MPI MPI_Recv: comm=MPI_COMM_WORLD, tag=0, rank=0
[INFO] Detected MPI MPI_Send: comm=MPI_COMM_WORLD, tag=1, rank=3
[INFO] Detected MPI MPI_Recv: comm=MPI_COMM_WORLD, tag=1, rank=2
[INFO] Detected MPI MPI_Send: comm=MPI_COMM_WORLD, tag=2, rank=5
[INFO] Detected MPI MPI_Recv: comm=MPI_COMM_WORLD, tag=2, rank=4
[INFO] Detected MPI MPI_Send: comm=MPI_COMM_WORLD, tag=3, rank=7
[INFO] Detected MPI MPI_Recv: comm=MPI_COMM_WORLD, tag=3, rank=6
[INFO] Analyzing Uniform Participation Patterns...
[INFO] Uniform Participation Detected in Comm MPI_COMM_WORLD with Tag 0 involving Ranks: 0 1
Uniform Participation Report:
------------------------------------
- Communicator: MPI_COMM_WORLD
- Tag: 0
- Participating Ranks: {0, 1}
This indicates that both MPI_Send and MPI_Recv operations with tag 0 in communicator
MPI_COMM_WORLD involve these ranks.

[INFO] Uniform Participation Detected in Comm MPI_COMM_WORLD with Tag 1 involving Ranks: 2 3
Uniform Participation Report:
------------------------------------
- Communicator: MPI_COMM_WORLD
- Tag: 1
- Participating Ranks: {2, 3}
This indicates that both MPI_Send and MPI_Recv operations with tag 1 in communicator
MPI_COMM_WORLD involve these ranks.

[INFO] Uniform Participation Detected in Comm MPI_COMM_WORLD with Tag 2 involving Ranks: 4 5
Uniform Participation Report:
------------------------------------
- Communicator: MPI_COMM_WORLD
- Tag: 2
- Participating Ranks: {4, 5}
This indicates that both MPI_Send and MPI_Recv operations with tag 2 in communicator
MPI_COMM_WORLD involve these ranks.

[INFO] Uniform Participation Detected in Comm MPI_COMM_WORLD with Tag 3 involving Ranks: 6 7
Uniform Participation Report:
------------------------------------
- Communicator: MPI_COMM_WORLD
- Tag: 3
- Participating Ranks: {6, 7}
This indicates that both MPI_Send and MPI_Recv operations with tag 3 in communicator
MPI_COMM_WORLD involve these ranks.

✅ Ran mpi-analysis pass on input.ll in 17 ms
*/
