#include <mpi.h>
#include <stdio.h>

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

    // --------------------------------------------
    // Custom Communicator (custom_comm)
    // --------------------------------------------
    // Split MPI_COMM_WORLD into two groups: even ranks & odd ranks
    // Each group forms a new communicator

    MPI_Comm custom_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &custom_comm);

    if (rank == 2) // Process 2
    {
        int data = 200; // Data to send
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
