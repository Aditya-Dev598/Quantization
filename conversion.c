#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define FIXED_BIT 5

/* Masks for extracting 6-bit packed values from a 32-bit integer */
#define MASK0  0x0000003F
#define MASK1  0x00000FC0
#define MASK2  0x0003F000
#define MASK3  0x00FC0000
#define MASK4  0x3F000000

/* Convert a float in [-1, 1) to a 6-bit fixed-point integer (1 sign + 5 frac) */
int8_t float2fix(float n)
{
    int8_t int_part = 0, frac_part = 0;
    int i;
    float t;

    if (n == -1.0) {
        return 1 << FIXED_BIT;
    }

    if (n > 0) {
        int_part = ((int)floor(n)) << FIXED_BIT;
    } else {
        int_part = 1 << FIXED_BIT;
    }

    n = fabs(n) - floor(fabs(n));

    t = 0.5;
    for (i = 0; i < FIXED_BIT; i++) {
        if ((n - t) >= 0) {
            n -= t;
            frac_part += (1 << (FIXED_BIT - 1 - i));
        }
        t = t / 2;
    }

    return int_part + frac_part;
}

/* Convert a 6-bit fixed-point integer back to float */
float fix2float(int8_t n)
{
    int8_t int_part = n & (1 << FIXED_BIT);
    int8_t frac_part = n & (MASK0 >> 0);  /* lower FIXED_BIT bits */
    float ans = 0;

    if (int_part == (1 << FIXED_BIT) && frac_part == 0) {
        return -1.0;
    }

    for (int i = FIXED_BIT - 1; i >= 0; i--) {
        if (frac_part > 0) {
            int m = frac_part & 0x01;
            if (m == 1) {
                ans += 1.0f / (float)(pow(2, i + 1));
            }
            frac_part = frac_part >> 1;
        }
    }

    if (int_part == (1 << FIXED_BIT)) {
        ans = ans * (-1);
    }

    return ans;
}

/* Extract the idx-th packed 6-bit value from a 32-bit integer */
static inline int8_t unpack6(int packed, int idx)
{
    return (packed >> (idx * 6)) & 0x3F;
}

int main()
{
    clock_t t, s, r;

    srand(time(0));

    /* All representable values in the 6-bit fixed-point scheme */
    float input[] = {
        -1.0,    -0.96875, -0.9375,  -0.875,   -0.84375, -0.8125,
        -0.78125, -0.75,   -0.71875, -0.6875,  -0.65625, -0.625,
        -0.59375, -0.5625, -0.53125, -0.5,     -0.46875, -0.4375,
        -0.40625, -0.375,  -0.34375, -0.3125,  -0.28125, -0.25,
        -0.21875, -0.1875, -0.15625, -0.125,   -0.09375, -0.0625,
        -0.03125,  0.0,     0.03125,  0.0625,   0.09375,  0.125,
         0.15625,  0.1875,  0.21875,  0.25,     0.28125,  0.3125,
         0.34375,  0.375,   0.40625,  0.4375,   0.46875,  0.5,
         0.53125,  0.5625,  0.59375,  0.625,    0.65625,  0.6875,
         0.71875,  0.75,    0.78125,  0.8125,   0.84375,  0.875,
         0.9375,   0.96875
    };
    const int input_size = 62;

    int m = 0, n = 0, k = 0, M = 0, K = 0, N = 0;
    printf("Enter the dimensions of the compressed matrices m,n,k:\n");
    if (scanf("%d%d%d", &m, &n, &k) != 3 || m <= 0 || n <= 0 || k <= 0) {
        fprintf(stderr, "Invalid compressed matrix dimensions.\n");
        return 1;
    }

    int *A, *B, *C;
    A = (int *)malloc(m * k * sizeof(int));
    B = (int *)malloc(n * k * sizeof(int));
    C = (int *)malloc(m * n * sizeof(int));
    if (!A || !B || !C) {
        fprintf(stderr, "Memory allocation failed for compressed matrices.\n");
        free(A); free(B); free(C);
        return 1;
    }

    printf("Enter the dimensions of the original matrices M,N,K:\n");
    if (scanf("%d%d%d", &M, &N, &K) != 3 || M <= 0 || N <= 0 || K <= 0) {
        fprintf(stderr, "Invalid original matrix dimensions.\n");
        free(A); free(B); free(C);
        return 1;
    }

    float *D, *E, *F;
    D = (float *)malloc(M * K * sizeof(float));
    E = (float *)malloc(N * K * sizeof(float));
    F = (float *)malloc(M * N * sizeof(float));
    if (!D || !E || !F) {
        fprintf(stderr, "Memory allocation failed for float matrices.\n");
        free(A); free(B); free(C); free(D); free(E); free(F);
        return 1;
    }

    int i, j;
    int num = 0;

    printf("Initializing the compressed matrix A...\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < k; j++) {
            *(A + i*k + j) = 0;
            num = float2fix(input[rand() % input_size]);
            *(A + i*k + j) = num;
            num = float2fix(input[rand() % input_size]);
            *(A + i*k + j) += (num << 6);
            num = float2fix(input[rand() % input_size]);
            *(A + i*k + j) += (num << 12);
            num = float2fix(input[rand() % input_size]);
            *(A + i*k + j) += (num << 18);
            num = float2fix(input[rand() % input_size]);
            *(A + i*k + j) += (num << 24);
        }
    }

    printf("Retrieving compressed values from A into full-size matrix D...\n");
    int count = 1, p = 0, q = 0;
    t = clock();
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            *(D + i*K + j) = fix2float(unpack6(*(A + p*k + q), count - 1));
            count++;
            if (count == 6) {
                count = 1;
                q++;
            }
            if (q == k) {
                p++;
                q = 0;
            }
        }
    }
    t = clock() - t;

    printf("Initializing the compressed matrix B...\n");
    for (i = 0; i < k; i++) {
        for (j = 0; j < n; j++) {
            *(B + i*n + j) = 0;
            num = float2fix(input[rand() % input_size]);
            *(B + i*n + j) += num;
            num = float2fix(input[rand() % input_size]);
            *(B + i*n + j) += (num << 6);
            num = float2fix(input[rand() % input_size]);
            *(B + i*n + j) += (num << 12);
            num = float2fix(input[rand() % input_size]);
            *(B + i*n + j) += (num << 18);
            num = float2fix(input[rand() % input_size]);
            *(B + i*n + j) += (num << 24);
        }
    }

    printf("Retrieving compressed values from B into full-size matrix E...\n");
    count = 1; p = 0; q = 0;
    s = clock();
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            *(E + i*N + j) = fix2float(unpack6(*(B + p*n + q), count - 1));
            count++;
            if (count == 6) {
                count = 1;
                q++;
            }
            if (q == n) {
                p++;
                q = 0;
            }
        }
    }
    s = clock() - s;

    double time_taken = ((double)(t + s)) / CLOCKS_PER_SEC;
    printf("\nTime to retrieve and store in original matrices: %f seconds\n", time_taken);

    /* Initialize result matrix F to zero before accumulation */
    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++)
            *(F + i*N + j) = 0.0f;

    r = clock();
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            for (int l = 0; l < K; l++) {
                *(F + i*N + j) += (*(D + i*K + l)) * (*(E + l*N + j));
            }
        }
    }
    r = clock() - r;

    time_taken = ((double)r) / CLOCKS_PER_SEC;
    printf("Time for multiplication: %f seconds\n", time_taken);

    double total = ((double)(t + s + r)) / CLOCKS_PER_SEC;
    printf("Total time: %f seconds\n", total);

    free(A); free(B); free(C);
    free(D); free(E); free(F);

    return 0;
}
