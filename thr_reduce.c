#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

//Number of threads and limit of sum calculation
int m, n;

//Global sum of quadruple roots (ASK NADDIE WHY THIS IS NEEDED)
double sum = 0;

//This line is declaring an array of size 256 that will store the partial sums of each thread.
//The number 256 is arbitrary, meaning it does not have to be that number
double partial_sums[256];

//Initialize mutex (ASK NADDIE WHY THIS IS NEEDED)
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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

    //Return null as function return type is `void*`
    return NULL;
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


    //Create `m` threads
    pthread_t threads[m];
    for (long i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, quad_root_sum, (void*)i);     //void* makes it so no matter what type of value i is, it will take that
    }

    //Wait for all threads to complete
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }

    // Parallel Reduction
    int active_threads = m;         //Variable for total number of threads, m.
    while (active_threads > 1) {                //Execute as long as active threads is greater than 1
        int other_thread = active_threads / 2;          //other_thread is equal to active_threads / 2 to determine the number of partner threads each active thread has in that specific step
        for (int i = 0; i < other_thread; i++) {        //for loop executes as long as i is less than other_thread, and increases by 1 for each loop
            int j = i + other_thread;                   //j variable used to access the other_thread's partial sum in the partial_sums[] array
            partial_sums[i] += partial_sums[j];         //Combines the two partial sums and stores the results back to partial_sums[i]
        }
        active_threads = other_thread;                  //When the loop finishes, active_threads is set to the value of other_thread, which will be half of the number of active threads, and will continue until only one active thread is left
    }
    //Results in the final sum being stored in 'partial_sums[0]' 
    //This value represents the sum of the quadruple roots of the first n positive integers

    // Print the final sum
    printf("Sum of quadruple roots: %f\n", partial_sums[0]);

    return 0;
}

//Note to self: To compile the code, make sure to use -lpthread and -lm to implement the pthread and math libraries
//So it would look like: gcc -o thr_reduce thr_reduce.c -lpthread -lm

//To run the code: ./thr_reduce [m] [n]