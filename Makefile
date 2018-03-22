MPICC = mpicc
CC = cc
LIBS =
CFLAGS = -O
OBJECTS = bitonic_sort.o sorter.o

all: bitonic_sort.o sorter.o bitonic_sort

bitonic_sort:   $(OBJECTS)
								$(MPICC)  -o bitonic_sort  $(OBJECTS) $(LIBS)

bitonic_sort.o: src/bitonic_sort.c
								$(CC) -c src/bitonic_sort.c $(CFLAGS)

sorter.o: src/sorter.c
				  $(CC) -c src/sorter.c $()

clean:
				rm -f bitonic_sort $(OBJECTS)

run:
	mpirun -np 8 ./bitonic_sort 1000
