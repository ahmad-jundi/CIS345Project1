#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Number of threads and limit of sum calculation
int m, n; //m, n

// Global sum of quadruple roots
double sum = 0;

// Initialize mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function to calculate sum of quadruple roots for each thread
void* quad_root_sum(void* thread_id) {
    // Calculate start and end indices for the thread
    int start = (int)thread_id * (n / m); //n,m
    int end = ((int)thread_id + 1) * (n / m);//n,m

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
    printf("thr %ld: %f\n", thread_id, thread_sum);

    // Return null as function return type is `void*`
    return NULL;
}

int main(int argc, char* argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s m n\n", argv[0]);
        return 1;
    }

    // Get the number of threads and limit of sum calculation from arguments
    //atoi = argument to integer
    m = atoi(argv[1]); //m
    n = atoi(argv[2]); //n

    // Array to store thread IDs
    pthread_t threads[m];


    // Create `x` threads
    for (long i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, quad_root_sum, (void*)i);   //void* makes it so no matter what type of value i is, it will take that
    }

    // Wait for all threads to finish execution
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final sum of quadruple roots
    printf("sum of quadruple roots: %f\n", sum);

    // Return 0 as main function return type is `int`
    return 0;
}

//Note to self: To compile the code, make sure to use -lpthread and -lm to implement the pthread and math libraries
//So it would look like: gcc -o thr_atomic thr_atomic.c -lpthread -lm

//To run the code: ./thr_atomic [m] [n]