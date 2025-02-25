#include "MyClass.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>

void MyClass::runProgram() {
    int arr[ARRAY_SIZE];
    int fd[2];
    pid_t pid;

    // Seed the random number generator
    srand(time(0));

    // Fill the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = rand() % 100;  // Random numbers between 0 and 99
    }

    // Create a pipe
    if (pipe(fd) == -1) {
        std::cerr << "Pipe creation failed" << std::endl;
        return;
    }

    // Create a child process
    pid = fork();

    if (pid == -1) {
        std::cerr << "Fork failed" << std::endl;
        return;
    } else if (pid == 0) {
        // Child process
        close(fd[0]);  // Close unused read end

        // Find minimum in second half of the array
        int min_child = *std::min_element(arr + ARRAY_SIZE/2, arr + ARRAY_SIZE);

        // Write the minimum to the pipe
        write(fd[1], &min_child, sizeof(min_child));
        close(fd[1]);

        std::cout << "Child process (PID " << getpid() << ") found minimum: " << min_child << std::endl;
        exit(0);
    } else {
        // Parent process
        close(fd[1]);  // Close unused write end

        // Find minimum in first half of the array
        int min_parent = *std::min_element(arr, arr + ARRAY_SIZE/2);

        // Read child's minimum from the pipe
        int min_child;
        read(fd[0], &min_child, sizeof(min_child));
        close(fd[0]);

        std::cout << "Parent process (PID " << getpid() << ") found minimum: " << min_parent << std::endl;
        std::cout << "Child process found minimum: " << min_child << std::endl;

        // Find the overall minimum
        int min_overall = std::min(min_parent, min_child);
        std::cout << "Overall minimum of the array: " << min_overall << std::endl;

        // Wait for child to finish
        wait(NULL);
    }
}
