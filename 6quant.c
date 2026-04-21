#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    clock_t t;
    int m, n, k, i, j, l;
    printf("Enter m n k:\n");
    if (scanf("%d%d%d", &m, &n, &k) != 3 || m <= 0 || n <= 0 || k <= 0) {
        fprintf(stderr, "Dimensions must be positive integers.\n");
        return 1;
    }

    int *a, *b, *c;
    a = (int *)malloc(m * k * sizeof(int));
    b = (int *)malloc(k * n * sizeof(int));
    c = (int *)malloc(m * n * sizeof(int));
    if (!a || !b || !c) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(a); free(b); free(c);
        return 1;
    }

    srand(time(0));
    for (i = 0; i < m; i++)
        for (j = 0; j < k; j++)
            *(a + i*k + j) = rand();

    for (i = 0; i < k; i++)
        for (j = 0; j < n; j++)
            *(b + i*n + j) = rand();

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            *(c + i*n + j) = 0;

    t = clock();
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            for (l = 0; l < k; l++) {
                int p = 0, q = 0, r = 0;
                p = *(a+i*k+l) & 0b00000000000000000000000000111111;
                q = *(b+l*n+j) & 0b00000000000000000000000000111111;
                r = r | (p & q);
                p = *(a+i*k+l) & 0b00000000000000000000111111000000;
                q = *(b+l*n+j) & 0b00000000000000000000111111000000;
                r = r | (p & q);
                p = *(a+i*k+l) & 0b00000000000000111111000000000000;
                q = *(b+l*n+j) & 0b00000000000000111111000000000000;
                r = r | (p & q);
                p = *(a+i*k+l) & 0b00000000111111000000000000000000;
                q = *(b+l*n+j) & 0b00000000111111000000000000000000;
                r = r | (p & q);
                p = *(a+i*k+l) & 0b00111111000000000000000000000000;
                q = *(b+l*n+j) & 0b00111111000000000000000000000000;
                r = r | (p & q);
                *(c + i*n + j) |= r;
            }
        }
    }
    t = clock() - t;

    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("6-bit quantized multiply took %f seconds\n", time_taken);

    free(a); free(b); free(c);
    return 0;
}
