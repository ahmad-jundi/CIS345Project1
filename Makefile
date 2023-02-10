libs = -lm -lpthread

all: thr_atomic thr_reduce

thr_atomic: thr_atomic.o
	gcc thr_atomic.o $(libs) -o thr_atomic

thr_reduce: thr_reduce.o
	gcc thr_reduce.o $(libs) -o thr_reduce

thr_atomic.o: thr_atomic.c
	gcc -c thr_atomic.c

thr_reduce.o: thr_reduce.c
	gcc -c thr_reduce.c