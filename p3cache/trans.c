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
    int i, j, k, l;
    int a, b, c, d, e, f, g, h; // Buffer variables
    // use blocking for all
    // 32x32, use variables as buffer and transpose
    if(N==32 && M==32){
        for (i=0; i<32; i+=8) {
            for (j=0; j<32; j+=8) {
                    for (k=0; k<8; k++) {
                        a=A[i+k][j]; // storing temps
                        b=A[i+k][j+1]; // divide into 16 8x8 matrices
                        c=A[i+k][j+2]; // this part takes care of
                        d=A[i+k][j+3]; // upper left and lower right
                        e=A[i+k][j+4];
                        f=A[i+k][j+5];
                        g=A[i+k][j+6];
                        h=A[i+k][j+7];
                        B[j][i+k]=a;
                        B[j+1][i+k]=b;
                        B[j+2][i+k]=c;
                        B[j+3][i+k]=d;
                        B[j+4][i+k]=e;
                        B[j+5][i+k]=f;
                        B[j+6][i+k]=g;
                        B[j+7][i+k]=h;
                    }
                }
            }
        }
    if(M==32 && N==64){
        for (i=0; i<64; i+=8) {
            for (j=0; j<32; j+=8) {
                if (i == j) {
                    for (k=0; k<8; k+=4) { // divide into 8x8
                        for (l=0; l<8; l+=4) {
                            a=A[i+k][j+l];
                            b=A[i+k][j+l+1];
                            c=A[i+k+1][j+l];
                            d=A[i+k+1][j+l+1];
                            e=A[i+k+2][j+l];
                            f=A[i+k+2][j+l+1];
                            g=A[i+k+3][j+l];
                            h=A[i+k+3][j+l+1];
                            B[j+l][i+k]=a;
                            B[j+l+1][i+k]=b;
                            B[j+l][i+k+1]=c;
                            B[j+l+1][i+k+1]=d;
                            B[j+l][i+k+2]=e;
                            B[j+l+1][i+k+2]=f;
                            B[j+l][i+k+3]=g;
                            B[j+l+1][i+k+3]=h;
                            a=A[i+k][j+l+2];
                            b=A[i+k][j+l+3];
                            c=A[i+k+1][j+l+2];
                            d=A[i+k+1][j+l+3];
                            e=A[i+k+2][j+l+2];
                            f=A[i+k+2][j+l+3];
                            g=A[i+k+3][j+l+2];
                            h=A[i+k+3][j+l+3];
                            B[j+l+2][i+k]=a;
                            B[j+l+3][i+k]=b;
                            B[j+l+2][i+k+1]=c;
                            B[j+l+3][i+k+1]=d;
                            B[j+l+2][i+k+2]=e;
                            B[j+l+3][i+k+2]=f;
                            B[j+l+2][i+k+3]=g;
                            B[j+l+3][i+k+3]=h;
                        }
                    }
                } else {
                    for (k=0; k<4;k++) {
                        for (l=0;l<4;l++) {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                    a=A[i][j+4];
                    b=A[i][j+5];
                    c=A[i][j+6];
                    d=A[i][j+7];
                    e=A[i+1][j+4];
                    f=A[i+1][j+5];
                    g=A[i+1][j+6];
                    h=A[i+1][j+7];
                    for (k=4; k<8;k++) {
                        for (l=0; l<4;l++) {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                    for (k=4; k<8;k++) {
                        for (l=4; l<8;l++) {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                    B[j+4][i]=a;
                    B[j+5][i]=b;
                    B[j+6][i]=c;
                    B[j+7][i]=d;
                    B[j+4][i+1]=e;
                    B[j+5][i+1]=f;
                    B[j+6][i+1]=g;
                    B[j+7][i+1]=h;
                    for (k=2; k<4;k++) {
                        for (l=4; l<8;l++) {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                }
            }
        }
    }
    if(M==64){ // similar to 32x64
        for (i=0; i<64; i+=8) {
            for (j=0; j<64; j+=8) {
                if (i == j) {
                    for (k=0; k<8; k+=4) { // divide into 8x8
                        for (l=0; l<8; l+=4) {
                            a=A[i+k][j+l];
                            b=A[i+k][j+l+1];
                            c=A[i+k+1][j+l];
                            d=A[i+k+1][j+l+1];
                            e=A[i+k+2][j+l];
                            f=A[i+k+2][j+l+1];
                            g=A[i+k+3][j+l];
                            h=A[i+k+3][j+l+1];
                            B[j+l][i+k]=a;
                            B[j+l+1][i+k]=b;
                            B[j+l][i+k+1]=c;
                            B[j+l+1][i+k+1]=d;
                            B[j+l][i+k+2]=e;
                            B[j+l+1][i+k+2]=f;
                            B[j+l][i+k+3]=g;
                            B[j+l+1][i+k+3]=h;
                            a=A[i+k][j+l+2];
                            b=A[i+k][j+l+3];
                            c=A[i+k+1][j+l+2];
                            d=A[i+k+1][j+l+3];
                            e=A[i+k+2][j+l+2];
                            f=A[i+k+2][j+l+3];
                            g=A[i+k+3][j+l+2];
                            h=A[i+k+3][j+l+3];
                            B[j+l+2][i+k]=a;
                            B[j+l+3][i+k]=b;
                            B[j+l+2][i+k+1]=c;
                            B[j+l+3][i+k+1]=d;
                            B[j+l+2][i+k+2]=e;
                            B[j+l+3][i+k+2]=f;
                            B[j+l+2][i+k+3]=g;
                            B[j+l+3][i+k+3]=h;
                        }
                    }
                } else {
                    for (k=0; k<4;k++) {
                        for (l=0;l<4;l++) {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                    a=A[i][j+4];
                    b=A[i][j+5];
                    c=A[i][j+6];
                    d=A[i][j+7];
                    e=A[i+1][j+4];
                    f=A[i+1][j+5];
                    g=A[i+1][j+6];
                    h=A[i+1][j+7];
                    for (k=4; k<8;k++) {
                        for (l=0; l<4;l++) {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                    for (k=4; k<8;k++) {
                        for (l=4; l<8;l++) {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                    B[j+4][i]=a;
                    B[j+5][i]=b;
                    B[j+6][i]=c;
                    B[j+7][i]=d;
                    B[j+4][i+1]=e;
                    B[j+5][i+1]=f;
                    B[j+6][i+1]=g;
                    B[j+7][i+1]=h;
                    for (k=2; k<4;k++) {
                        for (l=4; l<8;l++) {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                }
            }
        }
    }    if(M==61){
        for (i=0; i<64; i+=8) {
            for (j=0; j<56; j+=8) {
                for (k=0; k<8; k++) {
                    a=A[i+k][j];
                    b=A[i+k][j+1];
                    c=A[i+k][j+2];
                    d=A[i+k][j+3];
                    e=A[i+k][j+4];
                    f=A[i+k][j+5];
                    g=A[i+k][j+6];
                    h=A[i+k][j+7];
                    B[j][i+k]=a;
                    B[j+1][i+k]=b;
                    B[j+2][i+k]=c;
                    B[j+3][i+k]=d;
                    B[j+4][i+k]=e;
                    B[j+5][i+k]=f;
                    B[j+6][i+k]=g;
                    B[j+7][i+k]=h;
                }
            }
        }
        for (j=0; j<56; j+=8) {
            for (i=64;i<67;i++) {
                a=A[i][j];
                b=A[i][j+1];
                c=A[i][j+2];
                d=A[i][j+3];
                e=A[i][j+4];
                f=A[i][j+5];
                g=A[i][j+6];
                h=A[i][j+7];
                B[j][i]=a;
                B[j+1][i]=b;
                B[j+2][i]=c;
                B[j+3][i]=d;
                B[j+4][i]=e;
                B[j+5][i]=f;
                B[j+6][i]=g;
                B[j+7][i]=h;
            }
        }
        for (i=0; i<67; i++) {
            a=A[i][56];
            b=A[i][57];
            c=A[i][58];
            d=A[i][59];
            e=A[i][60];
            B[56][i]=a;
            B[57][i]=b;
            B[58][i]=c;
            B[59][i]=d;
            B[60][i]=e;
        }
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
    
    for (i = 0; i<N; i++) {
        for (j = 0; j<M; j++) {
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
    
    
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    
    for (i = 0; i<N; i++) {
        for (j = 0; j<M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
