#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

// Number of threads and limit of sum calculation
int m, n;

// Global array
double *partial_sums;

// Global Array
pthread_t *threads;

// Function to calculate sum of quadruple roots for each thread
void *quad_root_sum(void *thread_temp)
{
    // Calculate start and end indices for the thread
    long thread_id = (long)thread_temp;
    int start = (long)thread_id * (n / m);
    int end = ((long)thread_id + 1) * (n / m);
    // Local sum for each thread
    double thread_sum = 0;
    // Calculate the sum of quadruple roots
    for (int i = start + 1; i <= end; i++)
    {
        thread_sum += sqrt(sqrt(i));
    }

    // Store the local sum in the array of partial sums
    partial_sums[(int)thread_id] = thread_sum;

    //Reducing the partial sums
    for (int i = 1; i < m/2; i++)
    {
        //Determine the partner thread
        int partner = (int)thread_id ^ (1 << (i-1));
        //Check if the current thread is responsible for reducing its partners sum
        if (thread_id > partner) {

            //Avoids a race condition, in other words, thread_id waits for partner to completely finish
            pthread_join(threads[partner], NULL);

            //Add partner's sum to current thread's partial sum
            partial_sums[thread_id] += partial_sums[partner];

            //Debugging statement to help us, commented out as it is no longer needed
            //printf("thread_id %ld partner %ld \n", (long)thread_id, (long)partner);
        }
        else {
            //Exit loop if current thread is not responsible for reducing its partners sum
            return 0;
        }
    }
    //Another debugging statement to show if the last active thread is 7, commented it out again
    //printf("thread_id %ld \n", (long)thread_id);
    for (int i=0; i < m; i++) {
        printf("thr %d: %f\n", i, partial_sums[i]);
    }
}

int main(int argc, char *argv[])
{
    // Get the number of threads and limit of sum calculation from arguments
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    // Initializing partial sums array
    partial_sums = (double *)malloc(sizeof(double) * m);

    // Initializing threads array
    threads = (pthread_t *)malloc(sizeof(pthread_t) * m);

        // Create `m` threads
        for (long i = 0; i < m; i++)
    {
        //Each thread executes quad_root_sum function with 'i' as argument
        pthread_create(&threads[i], NULL, quad_root_sum, (void *)i);
    }

    // Calling pthread_exit() allows other threads to continue execution
    pthread_exit(0);
}

// Note to self: To compile the code, make sure to use -lpthread and -lm to implement the pthread and math libraries
// So it would look like: gcc -o thr_reduce thr_reduce.c -lpthread -lm

// To run the code: ./thr_reduce [m] [n]