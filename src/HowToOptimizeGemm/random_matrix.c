#include <stdlib.h>

#define A( i, j ) a[ (i)*lda + (j) ]

void random_matrix( int m, int n, double *a, int lda )
{
  double drand48();
  for (int i=0; i<m; i++ ){
    for (int j=0; j<n; j++ ){
      A( i,j ) = 2.0 * drand48() - 1.0;
    }
  }
}
