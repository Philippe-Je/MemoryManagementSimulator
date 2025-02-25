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


    srand(time(0));

    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = rand() % 100;  // Random numbers between 0 and 99
    }

    if (pipe(fd) == -1) {
        std::cerr << "Pipe creation failed" << std::endl;
        return;
    }

    pid = fork();

    if (pid == -1) {
        std::cerr << "Fork failed" << std::endl;
        return;
    } else if (pid == 0) {
        close(fd[0]);  // Close unused read end

        int min_child = *std::min_element(arr + ARRAY_SIZE/2, arr + ARRAY_SIZE);
        write(fd[1], &min_child, sizeof(min_child));
        close(fd[1]);

        std::cout << "Child process (PID " << getpid() << ") found minimum: " << min_child << std::endl;
        exit(0);
    } else {
        close(fd[1]);  // Close unused write end
        int min_parent = *std::min_element(arr, arr + ARRAY_SIZE/2);
        int min_child;
        read(fd[0], &min_child, sizeof(min_child));
        close(fd[0]);

        std::cout << "Parent process (PID " << getpid() << ") found minimum: " << min_parent << std::endl;
        std::cout << "Child process found minimum: " << min_child << std::endl;
        int min_overall = std::min(min_parent, min_child);
        std::cout << "Overall minimum of the array: " << min_overall << std::endl;


        wait(NULL);
    }
}
