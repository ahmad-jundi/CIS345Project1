#Create a variable for all the libraries used in code
libs = -lm -lpthread	

all: thr_atomic thr_reduce

#Compiles the .o file, checks for libraries, renames the file to []
thr_atomic: thr_atomic.o
	gcc thr_atomic.o $(libs) -o thr_atomic

#Compiles the .o file, checks for libraries, renames the file to []
thr_reduce: thr_reduce.o
	gcc thr_reduce.o $(libs) -o thr_reduce

#Compiles the .c file, changes it to .o 
thr_atomic.o: thr_atomic.c
	gcc -c thr_atomic.c

#Compiles the .c file, changes it to .o
thr_reduce.o: thr_reduce.c
	gcc -c thr_reduce.c