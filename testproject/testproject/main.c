#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#define N 268435456

extern void arrMulAdd(long long int n, double scalar, double* X, double* Y, double* Z);

// param: n - length of vector - long long int - RCX
// A - scalar - sd float - XMM1
// X - vector 1 - address(sd float) - R8
// Y - vector 2 - address(sd float) - R9
// Z - resultant vector - address(sd float) - RBP + 32

void sanityCheck(long long int n, double scalar, double* X, double* Y, double* Z) {
    for (int i = 0; i < n; i++) {
        Z[i] = scalar * X[i] + Y[i];
    }
}

int main() {
    struct timespec tspec_before, tspec_after;
    long long total_sec_c = 0, total_nsec_c = 0;
    long long total_sec_asm = 0, total_nsec_asm = 0;

    long long int n = N;
    double scalar = 0;
    double* arr1 = (double*)malloc(n * sizeof(double));
    double* arr2 = (double*)malloc(n * sizeof(double));
    double* funcASM = (double*)malloc(n * sizeof(double));
    double* funcC = (double*)malloc(n * sizeof(double));

    scalar = ((float)rand()) / ((float)rand());
    for (int i = 0; i < n; i++) {
        arr1[i] = ((float)rand()) / ((float)rand());
        arr2[i] = ((float)rand()) / ((float)rand());
        funcASM[i] = 0;
        funcC[i] = 0;
    }

    for (int iteration = 0; iteration < 30; iteration++) {
        timespec_get(&tspec_before, TIME_UTC);
        sanityCheck(n, scalar, arr1, arr2, funcC);
        timespec_get(&tspec_after, TIME_UTC);
        long sec_c = tspec_after.tv_sec - tspec_before.tv_sec;
        long nsec_c = tspec_after.tv_nsec - tspec_before.tv_nsec;
        if (nsec_c < 0) { // Normalize if nsec part is negative
            sec_c--;
            nsec_c += 1000000000L;
        }
        total_sec_c += sec_c;
        total_nsec_c += nsec_c;
        printf("Iteration %d - C Kernel Time: %ld.%09ld seconds\n", iteration + 1, sec_c, nsec_c);

        timespec_get(&tspec_before, TIME_UTC);
        arrMulAdd(n, scalar, arr1, arr2, funcASM);
        timespec_get(&tspec_after, TIME_UTC);
        long sec_asm = tspec_after.tv_sec - tspec_before.tv_sec;
        long nsec_asm = tspec_after.tv_nsec - tspec_before.tv_nsec;
        if (nsec_asm < 0) { // Normalize if nsec part is negative
            sec_asm--;
            nsec_asm += 1000000000L;
        }
        total_sec_asm += sec_asm;
        total_nsec_asm += nsec_asm;
        printf("Iteration %d - ASM Kernel Time: %ld.%09ld seconds\n", iteration + 1, sec_asm, nsec_asm);
    }



    double average_time_c = (total_sec_c + total_nsec_c / 1e9) / 30.0;
    double average_time_asm = (total_sec_asm + total_nsec_asm / 1e9) / 30.0;

    printf("Average C Kernel Time: %f seconds\n", average_time_c);
    printf("Average ASM Kernel Time: %f seconds\n", average_time_asm);

    free(arr1);
    free(arr2);
    free(funcASM);
    free(funcC);



    return 0;
}


// OUTPUT
        /* for (int i = 0; i < N; i++) {
        printf("C: %.2f ASM: %.2f RESULT: ", funcC[i], funcASM[i]);
        if (funcC[i] = funcASM[i]) printf("PASS\n");
        else printf("FAIL\n");
        } */
