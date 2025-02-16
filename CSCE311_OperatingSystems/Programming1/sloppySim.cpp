// Made by Damian Sclafani

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <functional>

// Global variables
std::mutex counterMutex;
int globalCounter = 0;

// Function to simulate work
void simulateWork(int workTime, int iterations, bool cpuBound, int sloppiness) {
    std::chrono::milliseconds sleepTime(workTime);

    for (int i = 0; i < iterations; ++i) {
        if (cpuBound) {
            // Simulate CPU-bound work
            for (int j = 0; j < 1e6; ++j) {
                // Busy wait or actual computation
            }
        } else {
            // Simulate I/O-bound work
            std::this_thread::sleep_for(sleepTime);
        }

        // Update global counter after Sloppiness iterations
        if (i % sloppiness == 0) {
            std::lock_guard<std::mutex> lock(counterMutex);
            globalCounter += sloppiness;
        }
    }
}

// Logging function
void logInfo(int nThreads, int sloppiness, int workTime, int workIterations, bool cpuBound) {
    // Log information
    std::cout << "Settings:" << std::endl;
    std::cout << "  Number of Threads: " << nThreads << std::endl;
    std::cout << "  Sloppiness: " << sloppiness << std::endl;
    std::cout << "  Work Time: " << workTime << " ms" << std::endl;
    std::cout << "  Work Iterations: " << workIterations << std::endl;
    std::cout << "  CPU Bound: " << (cpuBound ? "true" : "false") << std::endl;

    // Print final global counter
    std::cout << "Final Global Counter: " << globalCounter << std::endl;
}

int main(int argc, char* argv[]) {
    // Default values
    int nThreads = 2;
    int sloppiness = 10;
    int workTime = 10;
    int workIterations = 100;
    bool cpuBound = false;
    bool doLogging = false;

    // Parse command line arguments
    if (argc >= 3) {
        nThreads = std::atoi(argv[1]);
        sloppiness = std::atoi(argv[2]);
        workTime = std::atoi(argv[3]);

        if (argc >= 5) {
            workIterations = std::atoi(argv[4]);
            cpuBound = (std::string(argv[5]) == "true");
        }

        if (argc == 7) {
            doLogging = (std::string(argv[6]) == "true");
        }
    } else {
        std::cerr << "Invalid number of arguments. Please provide at least 3 arguments." << std::endl;
        return 1;
    }

    // Create threads
    std::vector<std::thread> threads;
    for (int i = 0; i < nThreads - 1; ++i) {
        //threads.emplace_back(simulateWork, workTime, workIterations, cpuBound);
        //threads.emplace_back(simulateWork, workTime, workIterations, cpuBound, sloppiness);
        threads.emplace_back(simulateWork, workTime, workIterations, cpuBound, std::ref(sloppiness));

    }

    // Main thread also does work
    simulateWork(workTime, workIterations, cpuBound, sloppiness);

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Log information if required
    if (doLogging) {
        logInfo(nThreads, sloppiness, workTime, workIterations, cpuBound);
    }

    return 0;
}
