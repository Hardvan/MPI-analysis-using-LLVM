#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <set>
#include <vector>

using namespace llvm;

namespace
{
    // Structure to hold MPI communication details such as type, communicator, tag, and rank.
    struct MPICommunication
    {
        std::string type; // type of MPI call: "Send" or "Recv"
        std::string comm; // Communicator name, assumed as "MPI_COMM_WORLD" for simplicity
        int tag;          // Tag associated with the MPI call
        int rank;         // Rank involved in the MPI call
    };

    // Main analysis pass that analyzes MPI communication patterns in a function
    struct MPIAnalysisPass : public PassInfoMixin<MPIAnalysisPass>
    {
        std::vector<MPICommunication> mpiCalls; // Vector to store detected MPI calls in the function

        // Function that runs the analysis pass on the given function F
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM)
        {
            errs() << "MPIAnalysisPass running on function: " << F.getName() << "\n";

            for (auto &BB : F) // Each basic block in the function
            {
                for (auto &I : BB) // Each instruction in the basic block
                {
                    if (auto *call = dyn_cast<CallInst>(&I)) // Check if the instruction is a function call
                    {
                        // Get the called function and its name
                        if (Function *calledFunc = call->getCalledFunction())
                        {
                            StringRef funcName = calledFunc->getName();
                            // If the function is MPI_Send or MPI_Recv, analyze the call
                            if (funcName.equals("MPI_Send") || funcName.equals("MPI_Recv"))
                            {
                                analyzeMPICall(call, funcName);
                            }
                        }
                    }
                }
            }

            analyzeUniformParticipation();   // Analyze uniform participation patterns (generates report)
            return PreservedAnalyses::all(); // Indicate that all analyses are preserved
        }

        // Function to analyze an MPI call (either MPI_Send or MPI_Recv)
        void analyzeMPICall(CallInst *call, StringRef funcName)
        {
            MPICommunication mpiComm;      // Create an MPICommunication object to store MPI call details
            mpiComm.type = funcName.str(); // Store the type of MPI call (MPI_Send or MPI_Recv)

            mpiComm.comm = "MPI_COMM_WORLD"; // Assuming the communicator is always MPI_COMM_WORLD

            // Extract the tag, which is the 5th argument (4th index) in both MPI_Send & MPI_Recv
            if (auto *tagArg = dyn_cast<ConstantInt>(call->getArgOperand(4)))
            {
                mpiComm.tag = tagArg->getSExtValue();
            }

            // Extract the rank, which is the 4th argument (3rd index) in both MPI_Send & MPI_Recv
            int rankArgIndex = 3;
            if (auto *rankArg = dyn_cast<ConstantInt>(call->getArgOperand(rankArgIndex)))
            {
                mpiComm.rank = rankArg->getSExtValue();
            }

            // Store the analyzed MPI call in the mpiCalls vector
            mpiCalls.push_back(mpiComm);

            // Output the detected MPI call details for debugging
            errs() << "[INFO] Detected MPI " << mpiComm.type << ": comm=" << mpiComm.comm
                   << ", tag=" << mpiComm.tag << ", rank=" << mpiComm.rank << "\n";
        }

        // Function to analyze uniform participation patterns among MPI processes
        void analyzeUniformParticipation()
        {
            errs() << "\n[INFO] Analyzing Uniform Participation Patterns...\n\n";

            // Map to store ranks involved in each (comm, tag) pair
            std::map<std::pair<std::string, int>, std::set<int>> participationMap; // (comm, tag) -> {ranks}

            // Populate participationMap with ranks involved in each (comm, tag) pair
            for (const auto &call : mpiCalls)
            {
                participationMap[{call.comm, call.tag}].insert(call.rank);
            }

            // Iterate over the participationMap to identify and report uniform participation
            for (const auto &entry : participationMap)
            {
                // If more than one rank is involved in a (comm, tag) pair, report it as uniform participation
                if (entry.second.size() > 1)
                {
                    errs() << "[INFO] Uniform Participation Detected in Comm " << entry.first.first
                           << " with Tag " << entry.first.second << " involving Ranks: ";
                    for (int rank : entry.second) // Output the ranks involved in the uniform participation
                    {
                        errs() << rank << " ";
                    }
                    errs() << "\n";

                    // Output a detailed uniform participation report.
                    errs() << "Uniform Participation Report:\n";
                    errs() << "------------------------------------\n";
                    errs() << "- Communicator: " << entry.first.first << "\n";
                    errs() << "- Tag: " << entry.first.second << "\n";
                    errs() << "- Participating Ranks: {";
                    for (auto it = entry.second.begin(); it != entry.second.end(); it++)
                    {
                        if (it != entry.second.begin())
                            errs() << ", ";
                        errs() << *it;
                    }
                    errs() << "}\n";
                    errs() << "This indicates that both MPI_Send and MPI_Recv operations with tag "
                           << entry.first.second << " in communicator\n"
                           << entry.first.first
                           << " involve these ranks.\n\n";
                }
            }
        }

        // Indicates whether the pass is required to run again.
        static bool isRequired() { return true; }
    };
}

// LLVM pass registration function, enabling the pass to be used in LLVM's pass pipeline
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "MPIAnalysisPass", LLVM_VERSION_STRING,
        [](PassBuilder &PB)
        {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>)
                {
                    if (Name == "mpi-analysis") // Check if the requested pipeline name is "mpi-analysis"
                    {
                        FPM.addPass(MPIAnalysisPass()); // Add the MPIAnalysisPass to the pipeline
                        return true;
                    }
                    return false;
                });
        }};
}
