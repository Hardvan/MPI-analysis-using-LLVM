# Analyzing Uniform Participation of MPI Processes Using LLVM/Clang

This project aims to analyze the uniform participation patterns of MPI processes in C/C++ code using LLVM/Clang. By leveraging the LLVM framework, we developed a custom analysis pass that identifies MPI communication calls & detects patterns of uniform participation among MPI processes.

## Team Members

1. Hardik Pawar
2. Karan Sathish
3. Abhishek Yadav
4. Harshit Dhoot

## Features

- **MPI Communication Analysis**: Detects & analyzes MPI_Send and MPI_Recv calls.
- **Uniform Participation Detection**: Identifies uniform participation patterns across MPI processes.
- **Automated shell script execution**: Simplifies the process of running the analysis by using a single command.
- **Detailed Reporting**: Provides comprehensive reports on detected communication patterns.
- **LLVM/Clang Integration**: Utilizes LLVM's powerful analysis and transformation capabilities.

## Explanation of Each Code File

- **[MPIAnalysisPass.cpp](./final/MPIAnalysisPass.cpp)**: An LLVM pass that analyzes MPI communication patterns in the provided IR code. It detects `MPI_Send` & `MPI_Recv` calls, extracts relevant information, and analyzes uniform participation patterns among the MPI processes.

- **Input C Files**:

  1. **[original.c](./final/original.c)**: The original MPI C code as given in the problem statement. The program consists of the following process communication:

     - Process 0 sends data to Process 1 with tag 0
     - Process 1 receives data from Process 0 with tag 0

  2. **[mpi_example.c](./final/mpi_example.c)**: A simple MPI program demonstrating point-to-point communication between processes using both the default communicator (`MPI_COMM_WORLD`) and custom communicators. The program performs consists of the following process communication:

     - Process 0 sends data to Process 1 using tag 0.
     - Process 4 sends data to Process 5 using tag 2.
     - Process 2 sends data to Process 3 using tag 1.
     - Process 6 sends data to Process 7 using tag 3.

  3. **[mpi_example2.c](./final/mpi_example2.c)**: An extended version of the `mpi_example.c` program with additional communication patterns. The program consists of the following process communication:

     - Process 0 sends data to Process 1 with tag 0
     - Process 0 sends data to Process 2 with tag 0
     - Process 1 receives data from Process 0 with tag 0
     - Process 2 receives data from Process 0 with tag 0

- **[run_mpi_analysis.sh](./final/run_mpi_analysis.sh)**: A script that automates the process of compiling the MPI program to LLVM IR, building the LLVM analysis pass, and running the analysis pass on the generated LLVM IR. This script streamlines the process, making it easy to execute the entire workflow with a single command.

- **[input.ll](./final/input.ll)**: LLVM IR (Intermediate Representation) of the mpi_example.c code, generated by Clang. This file serves as the input for the LLVM pass.

## Steps to Run the Analysis

### One-Time Setup Instructions

1. **Check MPI Compiler Location:**

   ```sh
   mpicc -show
   ```

2. **Generate LLVM IR from MPI C Code:**

   ```sh
   clang -I/usr/include/lam -emit-llvm -S mpi_example.c -o input.ll
   ```

   This command generates the LLVM IR file `input.ll` from the provided MPI C code in `mpi_example.c`.

   Note: Use the `find` command to locate the MPI header files if the above command fails.

   ```sh
   find /usr -name mpi.h
   ```

   Paste the path to the MPI header files in the clang command:

   ```sh
   clang -I<mpi_header_path> -emit-llvm -S mpi_example.c -o input.ll
   ```

### Execution Instructions

1. **Compile the MPI Analysis Pass:**

   ```sh
   clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp `llvm-config --cxxflags --ldflags --libs`
   ```

   This command compiles the MPIAnalysisPass.cpp file into a shared object file (MPIAnalysisPass.so) that can be loaded as an LLVM pass.

2. **Run the Analysis Pass:**

   ```sh
   opt -load-pass-plugin=./MPIAnalysisPass.so -passes="mpi-analysis" < input.ll > /dev/null
   ```

   This command runs the MPI analysis pass on the LLVM IR file `input.ll` and generates the analysis report. The `MPIAnalysisPass.so` shared object file is loaded as a plugin, and the `mpi-analysis` pass is executed on the input LLVM IR.

## Output Example

Output after performing the analysis on the `mpi_example.c` program:

```text
🚀 Analyzing Uniform Participation of MPI Processes Using LLVM/Clang 🚀

🔍 Step 1: Compiling mpi_example.c to LLVM IR (input.ll)
⛓️  Executing: clang -I/usr/include/lam -emit-llvm -S mpi_example.c -o input.ll
✅ Compiled in 83 ms

🔍 Step 2: Compiling MPIAnalysisPass.cpp to shared object (MPIAnalysisPass.so)
⛓️  Executing: clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp `llvm-config --cxxflags --ldflags --libs`
✅ Compiled in 4022 ms

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

✅ Ran mpi-analysis pass on input.ll in 26 ms
```

## Explanation of `comm`, `tag`, `rank`

In MPI (Message Passing Interface), `comm`, `tag`, and `rank` are key concepts used to manage and control the communication between processes in a parallel application. Here's a description of each:

### 1. `comm` (Communicator)

- **Definition**: A communicator in MPI is an object that defines a _group of processes_ that can communicate with each other. It essentially encapsulates the _communication context_.
- **Common Communicators**:

  - `MPI_COMM_WORLD`: The default communicator that includes all the processes in the MPI program.
  - User-defined communicators: You can create custom communicators to group specific processes for more specialized communication.

- **Usage**: Communicators are passed as arguments to most MPI functions to specify the group of processes involved in the communication.
- **Example**:

  ```c
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Send(buffer, count, datatype, dest, tag, comm);
  ```

### 2. `tag`

- **Definition**: A tag is an integer identifier used to distinguish between _different messages_. It allows processes to filter incoming messages based on their tag value.
- **Usage**: Tags are used in both sending and receiving messages. The sender assigns a tag to a message, and the receiver can specify which tag to receive.
- **Example**:

  ```c
  int tag = 99;
  MPI_Send(buffer, count, datatype, dest, tag, comm);
  MPI_Recv(buffer, count, datatype, source, tag, comm, &status);
  ```

### 3. `rank`

- **Definition**: A rank is a _unique identifier_ assigned to each process within a communicator. It is used to specify the source or destination of a message. It is like a _process ID_.
- **Usage**: The rank of a process is used to identify it among the group of processes in a communicator. Each process can query its own rank using `MPI_Comm_rank`.
- **Example**:

  ```c
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
      // Code for process with rank 0
  } else if (rank == 1) {
      // Code for process with rank 1
  } else {
      // Code for other processes
  }
  ```

### Summary in Context

When sending a message from one process to another using MPI, you specify the communicator (`comm`), the `rank` of the destination process, and a `tag` to identify the message.

For example, the `MPI_Send` function would send data from the calling process to another process identified by its rank within the given communicator, using a specified tag. The receiving process would use `MPI_Recv` with the same communicator and tag to correctly receive and identify the message.

## Prerequisites

- **LLVM/Clang:** Ensure you have LLVM and Clang installed.
- **MPI Library:** Ensure you have an MPI library installed, such as OpenMPI or MPICH.
