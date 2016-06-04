
#include <stdio.h>
#include <DSMClient.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_VALUE 10000

// Merge Sort related functions

/* Returns true if x should be before y */
bool asc_order_comparer(int x, int y) { return x <= y; }
bool desc_order_comparer(int x, int y) { return x > y; }

void merge(int *A, int p, int q, int r, bool (*comparison_predicate)(int,int), int sentinel)
{
    int i, j, k;
    int n1 = (q - p + 1);
    int n2 = (r - q);
    int* L = malloc(sizeof(int) * (n1 + 1));
    int* R = malloc(sizeof(int) * (n2 + 1));
    for (i = 0; i < n1; i++)
        L[i] = A[p + i];
    for (j = 0; j < n2; j++)
        R[j] = A[q + j + 1];
    L[n1] = sentinel;
    R[n2] = sentinel;
    i = 0;
    j = 0;
    for (k = p; k <= r; k++) {
        if (comparison_predicate(L[i], R[j])) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
    }
    free(L);
    free(R);
}

void msort_internal(int *A, int p, int r, bool (*comparison_predicate)(int,int), int sentinel)
{
    if (p < r) {
        int q = ((p + r) / 2);
        msort_internal(A, p, q, comparison_predicate, sentinel);
        msort_internal(A, q + 1, r, comparison_predicate, sentinel);
        merge(A, p, q, r, comparison_predicate, sentinel);
    }
}

void msort(int *A, int N, bool (*comparison_predicate)(int,int), int sentinel)
{
    msort_internal(A, 0, N-1, comparison_predicate, sentinel);
}


// Shear Sort related functions

// For even rows, elements are sorted in ascending order. For odd rows, elements are sorted in descending order.
void sort_rows_alternate_direction(int **A, int M)
{
    int i;
    for (i = 0; i < M; i++) {
        if (i % 2 == 0)
            msort(A[i], M, asc_order_comparer, MAX_VALUE);
        else
            msort(A[i], M, desc_order_comparer, -1);
    }
}

// The elements of each column are copied to a one dimensional array so they can be processed by msort(). Each thread has its own auxiliar array for doing this.
void sort_columns(int **A, int M)
{
    int i, j;
    int* columnCopy = malloc(sizeof(int) * M);
    for (j = 0; j < M; j++) {
        for (i = 0; i < M; i++)
            columnCopy[i] = A[i][j];
        msort(columnCopy, M, asc_order_comparer, MAX_VALUE);
        for (i = 0; i < M; i++)
            A[i][j] = columnCopy[i];
    }
    free(columnCopy);
}

// Function called from shear_main.cpp.
void shear_sort(int **A, int M)
{
    int i;
    for (i = 0; i < log2(M * M); i++) {
        sort_rows_alternate_direction(A, M);
        sort_columns(A, M);
    }
}

int main (int argc, char *argv[])
{
    puts("Hello from application_3");
    return 0;
}