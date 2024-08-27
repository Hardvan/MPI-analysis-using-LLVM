#!/bin/bash

# Step 1: Compile mpi_example.c to LLVM IR (input.ll)
clang -I/usr/include/lam -emit-llvm -S mpi_example2.c -o input.ll
echo "✅ Step 1: Compiled mpi_example2.c to LLVM IR (input.ll)"

# Step 2: Compile the MPIAnalysisPass.cpp to a shared object (MPIAnalysisPass.so)
clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp `llvm-config --cxxflags --ldflags --libs`
echo "✅ Step 2: Compiled MPIAnalysisPass.cpp to shared object (MPIAnalysisPass.so)"

# Step 3: Run the custom LLVM pass (mpi-analysis) on the input.ll
opt -load-pass-plugin=./MPIAnalysisPass.so -passes="mpi-analysis" < input.ll > /dev/null
echo "✅ Step 3: Ran mpi-analysis pass on input.ll"
