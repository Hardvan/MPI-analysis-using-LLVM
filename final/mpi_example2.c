#include <mpi.h>
#include <stdio.h>

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
        // Send to Process 2
        MPI_Send(&data, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
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
        // MPI_Send(&data, 1, MPI_INT, 3, 1, custom_comm); // Process 2 sends data to Process 3 with tag 1 within custom_comm
    }
    // else if (rank == 3) // Process 3
    // {
    //     int data;                                                          // Buffer to receive data
    //     MPI_Recv(&data, 1, MPI_INT, 2, 1, custom_comm, MPI_STATUS_IGNORE); // Process 3 receives data from Process 2 with tag 1 within custom_comm
    //     printf("Process 3 received data: %d\n", data);
    // }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
