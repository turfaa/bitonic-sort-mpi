#ifndef _SORTER_H
#define _SORTER_H

#define SORT_TAG 0
#define MERGE_TAG 1

#define SORT_ORDER_ASC 0
#define SORT_ORDER_DESC 1

void bitonic_sort(int * arr,
                  int n,
                  int rank_from,
                  int n_process,
                  int order,
                  int world_rank,
                  int world_size);

void _bitonic_merge(int * arr,
                    int n,
                    int rank_from,
                    int n_process,
                    int order,
                    int world_rank,
                    int world_size);

#endif
