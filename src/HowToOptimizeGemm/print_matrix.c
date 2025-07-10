#include <stdio.h>

#define A( i, j ) a[ (j)*lda + (i) ]

void print_matrix( int m, int n, double *a, int lda )
{
  for (int i=0; i<m; i++ ){
    for (int j=0; j<n; j++ ){
      printf("%le ", A( i,j ) );
    }
    printf("\n");
  }
  printf("\n");
}

