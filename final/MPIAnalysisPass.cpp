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
        std::string type; // "Send" or "Recv" to represent the type of MPI call.
        std::string comm; // Communicator name, assumed as "MPI_COMM_WORLD" for simplicity.
        int tag;          // Tag associated with the MPI call.
        int rank;         // Rank involved in the MPI call.
    };

    // Main analysis pass that analyzes MPI communication patterns in a function.
    struct MPIAnalysisPass : public PassInfoMixin<MPIAnalysisPass>
    {
        std::vector<MPICommunication> mpiCalls; // Vector to store detected MPI calls in the function.

        // Function that runs the analysis pass on the given function F.
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM)
        {
            errs() << "MPIAnalysisPass running on function: " << F.getName() << "\n";

            // Iterate over each basic block in the function.
            for (auto &BB : F)
            {
                // Iterate over each instruction in the basic block.
                for (auto &I : BB)
                {
                    // Check if the instruction is a function call.
                    if (auto *call = dyn_cast<CallInst>(&I))
                    {
                        // Get the called function and its name.
                        if (Function *calledFunc = call->getCalledFunction())
                        {
                            StringRef funcName = calledFunc->getName();
                            // If the function is MPI_Send or MPI_Recv, analyze the call.
                            if (funcName.equals("MPI_Send") || funcName.equals("MPI_Recv"))
                            {
                                analyzeMPICall(call, funcName);
                            }
                        }
                    }
                }
            }

            // Analyze uniform participation patterns among MPI processes.
            analyzeUniformParticipation();
            return PreservedAnalyses::all(); // Indicate that all analyses are preserved.
        }

        // Function to analyze an MPI call (either MPI_Send or MPI_Recv).
        void analyzeMPICall(CallInst *call, StringRef funcName)
        {
            MPICommunication mpiComm;
            mpiComm.type = funcName.str(); // Store the type of MPI call.

            // Assuming the communicator is always MPI_COMM_WORLD.
            mpiComm.comm = "MPI_COMM_WORLD";

            // Extract the tag, which is the 5th argument in both MPI_Send and MPI_Recv.
            if (auto *tagArg = dyn_cast<ConstantInt>(call->getArgOperand(4)))
            {
                mpiComm.tag = tagArg->getSExtValue();
            }

            // Extract the rank, which is the 4th argument in both MPI_Send and MPI_Recv.
            int rankArgIndex = 3;
            if (auto *rankArg = dyn_cast<ConstantInt>(call->getArgOperand(rankArgIndex)))
            {
                mpiComm.rank = rankArg->getSExtValue();
            }

            // Store the analyzed MPI call in the mpiCalls vector.
            mpiCalls.push_back(mpiComm);

            // Output the detected MPI call details for debugging.
            errs() << "[INFO] Detected MPI " << mpiComm.type << ": comm=" << mpiComm.comm
                   << ", tag=" << mpiComm.tag << ", rank=" << mpiComm.rank << "\n";
        }

        // Function to analyze uniform participation patterns among MPI processes.
        void analyzeUniformParticipation()
        {
            errs() << "[INFO] Analyzing Uniform Participation Patterns...\n";

            std::map<std::pair<std::string, int>, std::set<int>> participationMap;

            // Populate participationMap with ranks involved in each (comm, tag) pair.
            for (const auto &call : mpiCalls)
            {
                participationMap[{call.comm, call.tag}].insert(call.rank);
            }

            // Iterate over the participationMap to identify and report uniform participation.
            for (const auto &entry : participationMap)
            {
                // If more than one rank is involved in a (comm, tag) pair, report it.
                if (entry.second.size() > 1)
                {
                    errs() << "[INFO] Uniform Participation Detected in Comm " << entry.first.first
                           << " with Tag " << entry.first.second << " involving Ranks: ";
                    for (int rank : entry.second)
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
                    for (auto it = entry.second.begin(); it != entry.second.end(); ++it)
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

// LLVM pass registration function, enabling the pass to be used in LLVM's pass pipeline.
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
                    if (Name == "mpi-analysis")
                    {
                        FPM.addPass(MPIAnalysisPass()); // Add the MPIAnalysisPass to the pipeline.
                        return true;
                    }
                    return false;
                });
        }};
}
