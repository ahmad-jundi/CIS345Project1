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

    for (int i = 1; i < m/2; i++)
    {
        //when in doubt print it out
        int partner = (int)thread_id ^ (1 << (i-1));
        if (thread_id > partner) {
            pthread_join(threads[partner], NULL);//Avoids a race condition, in other words, thread_id waits for partner to completely finish
            partial_sums[thread_id] += partial_sums[partner];
            printf("thread_id %ld partner %ld \n", (long)thread_id, (long)partner);
        }
        else {
            //partial_sums[thread_id] += partial_sums[partner]; Commented this to show Steve/Justin
            //printf("ELSE thread_id %ld partner %ld \n", (long)thread_id, (long)partner);
            return 0;
        }
    }
    printf("thread_id %ld \n", (long)thread_id);
    for (int i=0; i < m; i++) {
        printf("thr %d: %f\n", i, partial_sums[i]); //Fix this shit to look nice for tomorrow :) 
    }
}

int main(int argc, char *argv[])
{
    printf("test\n");
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
        pthread_create(&threads[i], NULL, quad_root_sum, (void *)i); // void* makes it so no matter what type of value i is, it will take that
    }

    // Calling pthread_exit() allows other threads to continue execution
    pthread_exit(0);
}

// Note to self: To compile the code, make sure to use -lpthread and -lm to implement the pthread and math libraries
// So it would look like: gcc -o thr_reduce thr_reduce.c -lpthread -lm

// To run the code: ./thr_reduce [m] [n]