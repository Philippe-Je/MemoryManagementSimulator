#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>

const int TOTAL_PAGES = 100;
const int PAGE_SIZE = 160;
const int MEMORY_START = 2000;


struct Process {
    int id;
    int startAddress;
    int sizeInMB;
    int unusedSpace;
};

class MemoryManagementSimulator {
private:
    std::vector<int> memory;
    std::vector<Process> processes;
    int nextStartAddress;

public:
    MemoryManagementSimulator() : memory(TOTAL_PAGES, 0), nextStartAddress(MEMORY_START) {}

    void userMemoryAllocation() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 30);

        int processId = 1;
        while (true) {
            int numPages = dis(gen);
            int processSize = numPages * 80;
            int pagesRequired = std::ceil(static_cast<double>(processSize) / PAGE_SIZE);

            if (pagesRequired > availablePages()) {
                break;
            }

            int totalAllocatedSize = pagesRequired * PAGE_SIZE;
            int unusedSpace = totalAllocatedSize - processSize;

            Process process{processId, nextStartAddress, processSize, unusedSpace};
            processes.push_back(process);

            for (int i = 0; i < pagesRequired; ++i) {
                memory[findNextFreePage()] = processId;
            }

            nextStartAddress += totalAllocatedSize;
            processId++;
        }
    }

    void printSummaryReport() {
        std::cout << "Summary Report:\n";
        std::cout << std::setw(15) << "Process Id" << std::setw(30) << "Starting Memory Address"
                  << std::setw(30) << "Size of the Process MB" << std::setw(20) << "Unused Space MB\n";

        for (const auto& process : processes) {
            std::cout << std::setw(15) << process.id << std::setw(30) << process.startAddress
                      << std::setw(30) << process.sizeInMB << std::setw(20) << process.unusedSpace << "\n";
        }
    }

private:
    int availablePages() {
        return std::count(memory.begin(), memory.end(), 0);
    }

    int findNextFreePage() {
        for (int i = 0; i < TOTAL_PAGES; ++i) {
            if (memory[i] == 0) return i;
        }
        return -1;
    }
};

int main() {
    MemoryManagementSimulator simulator;
    simulator.userMemoryAllocation();
    simulator.printSummaryReport();
    return 0;
}
