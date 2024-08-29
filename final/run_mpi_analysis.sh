#!/bin/bash

###########################################
# Run the script as follows:
# Verbose mode: ./run_mpi_analysis.sh -v
# Non-verbose mode: ./run_mpi_analysis.sh
###########################################

echo "🚀 Analyzing Uniform Participation of MPI Processes Using LLVM/Clang 🚀"
echo "" # Newline

# Enable verbose mode if -v is passed as an argument
VERBOSE=false
if [[ $1 == "-v" ]]; then
    VERBOSE=true
fi

# Input C file to compile to LLVM IR
FILENAME="mpi_example2.c"

# Step 1: Compile $FILENAME to LLVM IR (input.ll)
echo "🔍 Step 1: Compiling $FILENAME to LLVM IR (input.ll)"
if $VERBOSE; then
    echo "⛓️  Executing: clang -I/usr/include/lam -emit-llvm -S $FILENAME -o input.ll"
fi
start_time=$(date +%s%N)
clang -I/usr/include/lam -emit-llvm -S $FILENAME -o input.ll
end_time=$(date +%s%N)
elapsed=$((($end_time - $start_time) / 1000000))
echo "✅ Compiled in ${elapsed} ms"
echo "" # Newline

# Step 2: Compile the MPIAnalysisPass.cpp to a shared object (MPIAnalysisPass.so)
echo "🔍 Step 2: Compiling MPIAnalysisPass.cpp to shared object (MPIAnalysisPass.so)"
if $VERBOSE; then
    echo "⛓️  Executing: clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp \`llvm-config --cxxflags --ldflags --libs\`"
fi
start_time=$(date +%s%N)
clang++ -shared -fPIC -o MPIAnalysisPass.so MPIAnalysisPass.cpp `llvm-config --cxxflags --ldflags --libs`
end_time=$(date +%s%N)
elapsed=$((($end_time - $start_time) / 1000000))
echo "✅ Compiled in ${elapsed} ms"
echo "" # Newline

# Step 3: Run the custom LLVM pass (mpi-analysis) on the input.ll
echo "🔍 Step 3: Running mpi-analysis pass on input.ll"
if $VERBOSE; then
    echo "⛓️  Executing: opt -load-pass-plugin=./MPIAnalysisPass.so -passes=\"mpi-analysis\" < input.ll > /dev/null"
fi
start_time=$(date +%s%N)
opt -load-pass-plugin=./MPIAnalysisPass.so -passes="mpi-analysis" < input.ll > /dev/null
end_time=$(date +%s%N)
elapsed=$((($end_time - $start_time) / 1000000))
echo "✅ Ran mpi-analysis pass on input.ll in ${elapsed} ms"
