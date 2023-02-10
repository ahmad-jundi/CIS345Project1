#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Number of threads and limit of sum calculation
int x, y;

// Global sum of quadruple roots
double sum = 0;

// Mutex lock to access shared variable `sum`
pthread_mutex_t lock;

// Function to calculate sum of quadruple roots for each thread
void* quad_root_sum(void* arg) {
    // Get thread id from argument
    int thread_id = *((int*) arg);

    // Calculate start and end indices for the thread
    int start = thread_id * (y / x);
    int end = (thread_id + 1) * (y / x);

    // Local sum for each thread
    double thread_sum = 0;

    // Calculate the sum of quadruple roots
    for (int i = start + 1; i <= end; i++) {
        thread_sum += sqrt(sqrt(i));
    }

    // Lock the mutex before updating the shared sum
    pthread_mutex_lock(&lock);
    sum += thread_sum;
    pthread_mutex_unlock(&lock);

    // Print the local sum for each thread
    printf("thr %d: %f\n", thread_id, thread_sum);

    // Return null as function return type is `void*`
    return NULL;
}

int main(int argc, char* argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s x y\n", argv[0]);
        return 1;
    }

    // Get the number of threads and limit of sum calculation from arguments
    x = atoi(argv[1]);
    y = atoi(argv[2]);

    // Array to store thread IDs
    pthread_t threads[x];
    int thread_ids[x];

    // Initialize the mutex lock
    pthread_mutex_init(&lock, NULL);

    // Create `x` threads
    for (int i = 0; i < x; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, quad_root_sum, &thread_ids[i]);
    }

    // Wait for all threads to finish execution
    for (int i = 0; i < x; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final sum of quadruple roots
    printf("sum of quadruple roots: %f\n", sum);

    // Destroy the mutex lock
    pthread_mutex_destroy(&lock);

    // Return 0 as main function return type is `int`
    return 0;
}

//Note to self: To compile the code, make sure to use -lpthread and -lm to implement the pthread and math libraries
//So it would look like: gcc -o thr_atomic thr_atomic.c -lpthread -lm

//To run the code: ./thr_atomic [x] [y]