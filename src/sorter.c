#include <mpi.h>
#include "sorter.h"

void bitonic_sort(int* arr, int n, int rank_from, int n_process, int order,
  int world_rank, int world_size) {
	if (n <= 1) return;

	if (n_process > 1) {
		if (world_rank < (rank_from + n_process/2)) {
			bitonic_sort(arr,
                   n/2,
                   rank_from,
                   n_process/2,
                   SORT_ORDER_ASC,
                   world_rank,
                   world_size);

			MPI_Recv(arr + n/2,
							 n/2,
							 MPI_INT,
							 world_rank + n_process/2,
							 SORT_TAG,
							 MPI_COMM_WORLD,
							 MPI_STATUS_IGNORE);

			MPI_Send(arr,
						   n/2,
							 MPI_INT,
							 world_rank + n_process/2,
							 SORT_TAG,
							 MPI_COMM_WORLD);

			if ((n_process % 2 == 1) && (world_rank == rank_from)) {
				MPI_Send(arr,
							   n/2,
								 MPI_INT,
								 rank_from + n_process - 1,
								 SORT_TAG,
								 MPI_COMM_WORLD);
			}
		}
		else {
			bitonic_sort(arr + n/2,
									 n/2,
									 rank_from + n_process/2,
									 (n_process+1)/2,
									 SORT_ORDER_DESC,
                   world_rank,
                   world_size);

			if ((n_process % 2 == 0) || (world_rank < (rank_from + n_process - 1))) {
				MPI_Send(arr + n/2,
								 n/2,
								 MPI_INT,
								 world_rank - n_process/2,
								 SORT_TAG,
								 MPI_COMM_WORLD);

				MPI_Recv(arr,
								 n/2,
								 MPI_INT,
								 world_rank - n_process/2,
								 SORT_TAG,
								 MPI_COMM_WORLD,
								 MPI_STATUS_IGNORE);
			}
			else {
				MPI_Recv(arr,
								 n/2,
								 MPI_INT,
								 n_process,
								 SORT_TAG,
								 MPI_COMM_WORLD,
								 MPI_STATUS_IGNORE);
			}
		}
	}
	else {
		bitonic_sort(arr,
                 n/2,
                 rank_from,
                 1,
                 SORT_ORDER_ASC,
                 world_rank,
                 world_size);

		bitonic_sort(arr + n/2,
                 n/2,
                 rank_from,
                 1,
                 SORT_ORDER_DESC,
                 world_rank,
                 world_size);
	}
	_bitonic_merge(arr, n, rank_from, n_process, order, world_rank, world_size);
}

void _bitonic_merge(int* arr, int n, int rank_from, int n_process, int order,
  int world_rank, int world_size) {

	if (n <= 1) return;

	for (int i = 0; i < n/2; i++) {
		if (order == SORT_ORDER_ASC) {
			if (arr[i] > arr[i + n/2]) {
				int tmp = arr[i];
				arr[i] = arr[i + n/2];
				arr[i + n/2] = tmp;
			}
		}
    else {
      if (arr[i] < arr[i + n/2]) {
        int tmp = arr[i];
        arr[i] = arr[i + n/2];
        arr[i + n/2] = tmp;
      }
    }
	}

	if (n_process > 1) {
		if (world_rank < (rank_from + n_process/2)) {
			_bitonic_merge(arr,
                     n/2,
                     rank_from,
                     n_process / 2,
                     order,
                     world_rank,
                     world_size);

			MPI_Recv(arr + n/2,
						   n/2,
							 MPI_INT,
							 world_rank + n_process/2,
							 MERGE_TAG,
							 MPI_COMM_WORLD,
							 MPI_STATUS_IGNORE);

			MPI_Send(arr,
				 			 n/2,
							 MPI_INT,
							 world_rank + n_process/2,
							 MERGE_TAG,
							 MPI_COMM_WORLD);

			if ((n_process % 2 == 1) && (world_rank == rank_from)) {
				MPI_Send(arr,
								 n/2,
								 MPI_INT,
								 rank_from + n_process - 1,
								 MERGE_TAG,
								 MPI_COMM_WORLD);
			}
		}
		else {
			_bitonic_merge(arr + n/2,
                     n/2,
                     rank_from + n_process/2,
                     (n_process + 1)/2,
                     order,
                     world_rank,
                     world_size);

			if ((n_process % 2 == 0) || (world_rank < (rank_from + n_process - 1))) {
				MPI_Send(arr + n/2,
								 n/2,
								 MPI_INT,
								 world_rank - n_process/2,
								 MERGE_TAG,
								 MPI_COMM_WORLD);

				MPI_Recv(arr,
							   n/2,
								 MPI_INT,
								 world_rank - n_process/2,
								 MERGE_TAG,
								 MPI_COMM_WORLD,
								 MPI_STATUS_IGNORE);
			}
			else {
				MPI_Recv(arr,
								 n/2,
								 MPI_INT,
								 rank_from,
								 MERGE_TAG,
								 MPI_COMM_WORLD,
								 MPI_STATUS_IGNORE);
			}
		}

	}
	else {
		_bitonic_merge(arr, n/2, rank_from, 1, order, world_rank, world_size);
		_bitonic_merge(arr + n/2, n/2, rank_from, 1, order, world_rank, world_size);
	}
}
