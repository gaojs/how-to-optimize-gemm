#define A( i, j ) a[ (j)*lda + (i) ]
#define B( i, j ) b[ (j)*ldb + (i) ]

void copy_matrix( int m, int n, double *a, int lda, double *b, int ldb )
{
  for (int i=0; i<m; i++ ){
    for (int j=0; j<n; j++ ){
      B( i,j ) = A( i,j );
    }
  }
}

