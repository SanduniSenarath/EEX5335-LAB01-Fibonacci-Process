#include <stdio.h>    // For printf, scanf, perror
#include <stdlib.h>   // For atoi, exit
#include <unistd.h>   // For fork, getpid, getppid
#include <sys/wait.h> // For wait

// Function to calculate and print Fibonacci sequence up to 'n'
// This would be called in the child process
void generateFibonacci(int n) {
    if (n < 0) {
        fprintf(stderr, "Fibonacci: Please enter a non-negative number.\n");
        return;
    }
    if (n == 0) {
        printf("Fibonacci Sequence (up to 0): 0\n");
        return;
    }
    if (n == 1) {
        printf("Fibonacci Sequence (up to 1): 0, 1\n");
        return;
    }

    int t1 = 0, t2 = 1;
    printf("Fibonacci Sequence (up to %d): %d, %d", n, t1, t2);

    for (int i = 2; i <= n; ++i) {
        int nextTerm = t1 + t2;
        printf(", %d", nextTerm);
        t1 = t2;
        t2 = nextTerm;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int n;
    pid_t pid;

    // 1. Command-line argument parsing and error checking
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive_integer>\n", argv[0]);
        return 1; // Indicate error
    }

    n = atoi(argv[1]); // Convert argument to integer

    if (n < 0) { // Lab specifically asks for non-negative, but "positive integer" also mentioned
                  // Assuming non-negative based on general Fibonacci input
        fprintf(stderr, "Error: Please enter a non-negative integer.\n");
        return 1;
    }

    // 2. Fork a new process
    pid = fork();

    // 3. Error checking for fork()
    if (pid < 0) {
        perror("fork failed"); // Print system error message
        return 1;
    }
    // 4. Child process block
    else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) generating Fibonacci sequence...\n", getpid());
        generateFibonacci(n); // Generate sequence in the child
        printf("Child process (PID: %d) finished.\n", getpid());
        exit(0); // Child exits successfully
    }
    // 5. Parent process block
    else {
        // Parent process
        printf("Parent process (PID: %d) created child with PID: %d. Waiting for child to complete...\n", getpid(), pid);

        // 6. Parent waits for the child to complete
        int status;
        pid_t terminated_pid = wait(&status); // Wait for any child

        // 7. Error checking for wait()
        if (terminated_pid == -1) {
            perror("wait failed");
            return 1;
        }

        if (WIFEXITED(status)) { // Check if child terminated normally
            printf("Child process (PID: %d) terminated normally with exit status: %d.\n", terminated_pid, WEXITSTATUS(status));
        } else {
            printf("Child process (PID: %d) terminated abnormally.\n", terminated_pid);
        }
        printf("Parent process (PID: %d) exiting.\n", getpid());
    }

    return 0; // Parent exits successfully
}