#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

//Number of threads and limit of sum calculation
int m, n;

//Global array
double *partial_sums;

//Global Array
pthread_t *threads;

//Function to calculate sum of quadruple roots for each thread
void* quad_root_sum(void* thread_id) {
    //Calculate start and end indices for the thread
    int start = (int)thread_id * (n / m);
    int end = ((int)thread_id + 1) * (n / m);

    //Local sum for each thread
    double thread_sum = 0;

    //Calculate the sum of quadruple roots
    for (int i = start + 1; i <= end; i++) {
        thread_sum += sqrt(sqrt(i));
    }

    //Store the local sum in the array of partial sums
    partial_sums[(int)thread_id] = thread_sum;

    
}

int main(int argc, char* argv[]) {
    //Check for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s m n\n", argv[0]);
        return 1;
    }

    //Get the number of threads and limit of sum calculation from arguments
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    //Initializing partial sums array
    partial_sums = (double*)malloc(sizeof(double) * m);

    //Initializing threads array
    threads = (pthread_t*)malloc(sizeof(pthread_t) * m)

    //Create `m` threads
    for (long i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, quad_root_sum, (void*)i);     //void* makes it so no matter what type of value i is, it will take that
    }

    //Calling pthread_exit() allows other threads to continue execution
    pthread_exit(0);

}

//Note to self: To compile the code, make sure to use -lpthread and -lm to implement the pthread and math libraries
//So it would look like: gcc -o thr_reduce thr_reduce.c -lpthread -lm

//To run the code: ./thr_reduce [m] [n]