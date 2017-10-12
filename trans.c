/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);


char trans32_desc[] = "32 X 32 Matrix";
void trans32(int M, int N, int A[N][M], int B[M][N]){
  int i, j, x, y, tmp;
  for (i = 0; i < 32; i += 8){
    for (j = 0; j < 32; j += 8){
      for (x = i; x < i+8; x++){
	for (y = j; y < j+8; y++){
	  if (x != y){
	    B[y][x] = A[x][y];
	  }
	  else{
	    tmp = A[x][y];
	  }
	}
	if (i == j){
	  B[x][x] = tmp;
	}
      }
    }
  }
}



char trans64_desc[] = "64 X 64 Matrix";
void trans64(int M, int N, int A[N][M], int B[M][N]){
  int i, j, x, y, tmp;
  for (i = 0; i < 64; i += 4){
    for (j = 0; j < 64; j += 8){
      for (x = i; x < i+4; x++){
	for (y = j; y < j+8; y++){
	  if (x != y)
	    B[y][x] = A[x][y];
	  else
	    tmp = A[x][y];
	}
	if (i == j){
	  B[x][x] = tmp;
	}
      }
    }
  }
}



char transRand_desc[] = "61 X 67 Matrix (or other random dimensions)";
void transRand(int M, int N, int A[N][M], int B[M][N]){
  int i, j, x, y, tmp;
  for (i = 0; i < 67; i += 16){
    for (j = 0; j < 61; i += 16){
      for (x = i; (x < i+16)&&(i<61); x++){
	for (y = j; (y < j+16)&&(y<67); y++){
	  if (i != j){
	    B[y][x] = A[x][y];
	  }
	  else{
	    tmp = A[x][y];
	  }
	}
	if (i == j){
	  B[x][x] = tmp;
	}
      }
    }
  }
}



/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  if (M == 32 && N == 32){
    trans32(M, N, A, B);
  }

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}



/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
    // registerTransFunction(trans32, trans32_desc);
    //registerTransFunction(trans64, trans64_desc);
    registerTransFunction(transRand, transRand_desc);

   

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

