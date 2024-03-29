#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define N 5

extern void arrMulAdd(long long int n, double scalar, double* X, double* Y, double* Z);

// param: n - length of vector - long long int - RCX
// A - scalar - sd float - XMM1
// X - vector 1 - address(sd float) - R8
// Y - vector 2 - address(sd float) - R9
// Z - resultant vector - address(sd float) - RBP + 32

int main() {
	long long int n = N;
	double scalar = 1.5;
	double arr1[N] = {1.2,1.4,1.6,1.8,2.0};
	double arr2[N] = { 0.2,0.4,0.6,0.8,1.0 };
	double res[N] = { 0,0,0,0,0 };
	arrMulAdd(n,scalar,arr1,arr2,res);
	int i = 0;
	for (i = 0; i < N; i++) {
		printf("%.2f\n", res[i]);
	}
	return 0;
}
