#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

void calculate_prefix_sums(int x[], int n, int s[]) {
    s[0] = x[0];
    int r = 0;
    // We used inclusive scan to ensure that each thread gets the correct cumulative sum.
    #pragma omp parallel for reduction (inscan, +:r)
    for (int i = 1; i < n; i++) {
        r += x[i];
        #pragma omp scan inclusive(r)
        s[i] = r;
    }
}

void calculate_prefix_mins(int s[], int n, int m[]) {
    m[0] = s[0];
    int r = s[0];
    // We used inclusive scan here to ensure that each thread gets the minimum up to its index.
    #pragma omp parallel for reduction (inscan, min:r)
    for (int i = 1; i < n; i++) {
        r = s[i] < r ? s[i] : r;
        #pragma omp scan inclusive(r)
        m[i] = r;
    }
}

void calculate_sm(int s[], int m[], int n, int sm[]) {
    // We parallelized this loop as each calculation for sm[j] is independent.
    #pragma omp parallel for
    for (int j = 0; j < n; j++) {
        if (j == 0) {
            sm[j] = s[j];
        } else {
            sm[j] = s[j] - m[j - 1];
        }
    }
}

void calculate_indx_parallel(int s[], int m[], int n, int indx[]) {
    // We initialized indx in parallel for efficiency since it's a simple independent operation.
    #pragma omp parallel for
    for (int j = 0; j < n; j++) {
        indx[j] = -1;
    }

    // Finding indices where prefix minimums occur in parallel.
    // This is a nested loop, but the outer loop is parallelized with dynamic scheduling
    // to handle load imbalance and improve performance.
    #pragma omp parallel for schedule(dynamic)
    for (int j = 0; j < n; j++) {
        for (int i = 0; i <= j; i++) {
            if (s[i] == m[j]) {
                #pragma omp critical
                {
                    if (indx[j] == -1 || i < indx[j]) {
                        indx[j] = i; 
                    }
                }
                break; 
            }
        }
    }
}

int main() {
    const int n = 100000;

    int *x = malloc(n * sizeof(int));
    int *s = malloc(n * sizeof(int));
    int *m = malloc(n * sizeof(int));
    int *sm = malloc(n * sizeof(int));
    int *indx = malloc(n * sizeof(int));

    if (!x || !s || !m || !sm || !indx) {
        fprintf(stderr, "Memory allocation failed!\n");
        free(x);
        free(s);
        free(m);
        free(sm);
        free(indx);
        return 1; 
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        x[i] = (rand() % 31) - 15;
    }

    int thread_count;
    printf("Enter the number of threads: ");
    scanf("%d", &thread_count);
    omp_set_num_threads(thread_count);

    double total_time = 0;
    double start_time = omp_get_wtime();

    calculate_prefix_sums(x, n, s);
    calculate_prefix_mins(s, n, m);
    calculate_sm(s, m, n, sm);
    calculate_indx_parallel(s, m, n, indx);

    int MCS = INT_MIN;
    int end_idx = -1;
    for (int j = 0; j < n; j++) {
        if (sm[j] > MCS) {
            MCS = sm[j];
            end_idx = j;
        }
    }

    int start_idx = indx[end_idx] + 1;

    double end_time = omp_get_wtime();
    total_time = end_time - start_time;

    printf("The maximum consecutive sum (MCS) is: %d\n", MCS);
    printf("The indices of the subarray are: [%d, %d]\n", start_idx, end_idx);
    printf("Total time taken by the program: %.5f seconds\n", total_time);

    free(x);
    free(s);
    free(m);
    free(sm);
    free(indx);

    return 0;
}
