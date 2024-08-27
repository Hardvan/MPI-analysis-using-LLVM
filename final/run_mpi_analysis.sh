#!/bin/bash

# Input C file to compile to LLVM IR
FILENAME="mpi_example.c"

# Step 1: Compile $FILENAME to LLVM IR (input.ll)
start_time=$(date +%s%N)
clang -I/usr/include/lam -emit-llvm -S $FILENAME -o input.ll
end_time=$(date +%s%N)
elapsed=$((($end_time - $start_time) / 1000000))
echo "✅ Step 1: Compiled $FILENAME to LLVM IR (input.ll) in ${elapsed} ms"

# Step 2: Compile the MPIAnalysisPass.cpp to a shared object (MPIAnalysisPass.so)
start_time=$(date +%s%N)
clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp `llvm-config --cxxflags --ldflags --libs`
end_time=$(date +%s%N)
elapsed=$((($end_time - $start_time) / 1000000))
echo "✅ Step 2: Compiled MPIAnalysisPass.cpp to shared object (MPIAnalysisPass.so) in ${elapsed} ms"

# Step 3: Run the custom LLVM pass (mpi-analysis) on the input.ll
start_time=$(date +%s%N)
opt -load-pass-plugin=./MPIAnalysisPass.so -passes="mpi-analysis" < input.ll > /dev/null
end_time=$(date +%s%N)
elapsed=$((($end_time - $start_time) / 1000000))
echo "✅ Step 3: Ran mpi-analysis pass on input.ll in ${elapsed} ms"
