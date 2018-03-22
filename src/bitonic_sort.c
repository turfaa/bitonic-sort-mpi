#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
#include "sorter.h"

#define ROOT_RANK 0

int find_nearest_power_of_two(int n);
void rng(int * arr, int n);
void fill_array(int * arr, int from, int to, int filler);
int test(int *arr, int n, int order);

int world_rank, world_size;;

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	if (argc != 2) {
		if (world_rank == ROOT_RANK) {
			printf("Usage: %s n\n\n", argv[0]);
		}
		MPI_Finalize();
		exit(1);
	}

	int n_original = atoi(argv[1]);
	int n_arr = find_nearest_power_of_two(n_original);

	int* arr = (int*) malloc(n_arr * sizeof(int));

	if (world_rank == ROOT_RANK) {
		rng(arr, n_original);
		fill_array(arr, n_original, n_arr - 1, INT_MAX);
	}

	MPI_Bcast(arr, n_arr, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

	struct timeval startwtime, endwtime;

	if (world_rank == ROOT_RANK) gettimeofday (&startwtime, NULL);
	bitonic_sort(arr,
							 n_arr,
							 0,
							 world_size,
							 SORT_ORDER_ASC,
							 world_rank,
							 world_size);

	MPI_Barrier(MPI_COMM_WORLD);

	if (world_rank == ROOT_RANK) {
		gettimeofday(&endwtime, NULL);

		int seq_time = endwtime.tv_usec - startwtime.tv_usec
									 + (endwtime.tv_sec - startwtime.tv_sec) * 1000000;

		if (test(arr, n_original, SORT_ORDER_ASC)) {
			printf("berhasil, waktu %d microseconds\n", seq_time);
		}
		else {
			printf("gagal, waktu %d microseconds\n", seq_time);
		}
	}

	MPI_Finalize();
}

int test(int * arr, int n, int order) {
	for(int i = 0; i < n - 1; i++) {
		if (order == SORT_ORDER_ASC) {
			if (arr[i] > arr[i+1]) return 0;
		}
		else {
			if (arr[i] < arr[i+1]) return 0;
		}
	}

	return 1;
}

void rng(int* arr, int n) {
	srand(13515133);
	for (int i = 0; i < n; i++)
		arr[i] = rand();
}

void fill_array(int* arr, int from, int to, int filler) {
	for (int i = from; i <= to; i++) arr[i] = INT_MAX;
}

int find_nearest_power_of_two(int n) {
	int i = 1;
	while (i < n) i *= 2;
	return i;
}
