/* See rmxn.h. */
/* Last edited on 2012-12-15 09:48:00 by stolfilocal */

#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <rn.h>
#include <bool.h>
#include <jsmath.h>
#include <affirm.h>
#include <gauss_elim.h>

#include <rmxn.h>

void rmxn_zero(int m, int n, double *M)
  { int i, j, t;
    t = 0;
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
        { M[t] = 0.0; t++; }
  }

void rmxn_copy(int m, int n, double *A, double *M)
  { int mn = m*n;
    int ij;
    for (ij = 0; ij < mn; ij++) { M[ij] = A[ij]; }
  }

void rmxn_get_row(int m, int n, double *A, int i, double *r)
  { double *Ai = &(A[i*n]);
    int j;
    for (j = 0; j < n; j++) { r[j] = Ai[j]; }
  }
  
void rmxn_set_row(int m, int n, double *A, int i, double *r)
  { double *Ai = &(A[i*n]);
    int j;
    for (j = 0; j < n; j++) { Ai[j] = r[j]; }
  }

void rmxn_get_col(int m, int n, double *A, int j, double *r)
  { double *Aij = &(A[j]);
    int i;
    for (i = 0; i < m; i++) { r[i] = (*Aij); Aij += n; }
  }

void rmxn_set_col(int m, int n, double *A, int j, double *r)
  { double *Aij = &(A[j]);
    int i;
    for (i = 0; i < m; i++) { (*Aij) = r[i]; Aij += n; }
  }

void rmxn_ident(int m, int n, double *M)
  { int i, j, t;
    t = 0;
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
        { M[t] = (i == j ? 1.0 : 0.0); t++; }
  }

void rmxn_map_row (int m, int n, double *x, double *A, double *r)
  { int i, j;
    for (j = 0; j < n; j++)
      { double sum = 0.0, corr = 0.0; 
        int t = j;
        for (i = 0; i < m; i++) 
          { double term = x[i] * A[t];
            /* Kahan's summation: */
            double tcorr = term - corr;
            double newSum = sum + tcorr;
            corr = (newSum - sum) - tcorr;
            sum = newSum;
            t += n;
          }
        r[j] = sum;
      }
  }

void rmxn_map_col (int m, int n, double *A, double *x, double *r)
  { int i, j, t = 0;
    for (i = 0; i < m; i++)
      { double sum = 0.0, corr = 0.0; 
        for (j = 0; j < n; j++)
          { double term = A[t] * x[j]; 
            /* Kahan's summation: */
            double tcorr = term - corr;
            double newSum = sum + tcorr;
            corr = (newSum - sum) - tcorr;
            sum = newSum;
            t++;
          }
        r[i] = sum;
      }
  }

void rmxn_mul (int m, int p, int n, double *A, double *B, double *M)
  { int i, j, k, r = 0, v = 0;
    for (i = 0; i < m; i++)
      { for (j = 0; j < n; j++)
          { double sum = 0.0, corr = 0.0;
            int t = j;
            for (k = 0; k < p; k++)
              { double term = A[r+k]*B[t];
                /* Kahan's summation: */
                double tcorr = term - corr;
                double newSum = sum + tcorr;
                corr = (newSum - sum) - tcorr;
                sum = newSum;
                t+= n;
              }
            M[v] = sum; v++;
          }
        r += p;
      }
  }

void rmxn_mul_tr (int m, int n, int p, double *A, double *B, double *M)
  { int i, j, k;
    int v = 0;
    int r = 0;
    for (i = 0; i < m; i++)
      { int s = 0;
        for (j = 0; j < n; j++)
          { double sum = 0.0, corr = 0.0;
            for (k = 0; k < p; k++) 
              { double term = A[r+k]*B[s+k];
                /* Kahan's summation: */
                double tcorr = term - corr;
                double newSum = sum + tcorr;
                corr = (newSum - sum) - tcorr;
                sum = newSum;
              }
            M[v] = sum; v++;
            s += p;
          }
        r += p;
      }
  }

void rmxn_tr_mul (int p, int m, int n, double *A, double *B, double *M)
  { int i, j, k;
    int v = 0;
    for (i = 0; i < m; i++)
      { for (j = 0; j < n; j++)
          { double sum = 0.0, corr = 0.0;
            int r = i, s = j;
            for (k = 0; k < p; k++) 
              { double term = A[r]*B[s];
                /* Kahan's summation: */
                double tcorr = term - corr;
                double newSum = sum + tcorr;
                corr = (newSum - sum) - tcorr;
                sum = newSum;
                r += m; s += n;
              }
            M[v] = sum; v++;
          }
      }
  }

double rmxn_det (int n, double *A)
  { int n2 = n*n, t;
    double *C = (double *)notnull(malloc(n2*sizeof(double)), "no mem for C");
    double det = 1.0;
    for (t = 0; t < n2; t++) { C[t] = A[t]; }
    gsel_triangularize(n, n, C, FALSE, 0.0);
    for (t = 0; t < n2; t += n+1) { det *= C[t]; }
    free(C);
    return det;
  }

double rmxn_inv (int n, double *A, double *M)
  { int i, j;
    int nC = 2*n;
    int nA = n;
    int nM = n;
    double *C = (double *)notnull(malloc(n*nC*sizeof(double)), "no mem for C");
    /* Copy {A} into the left half of {C}, fill the right half with the identity: */
    for (i = 0; i < n; i++) 
      { double *Cij = &(C[nC*i]); 
        double *Aij = &(A[nA*i]);
        for (j = 0; j < nA; j++) { (*Cij) = (*Aij); Cij++; Aij++; }
        for (j = 0; j < nM; j++) { (*Cij) = (j == i ? 1.0 : 0.0); Cij++; }
      }
    gsel_triangularize(n, nC, C, FALSE, 0.0);
    double det = 1.0;
    for (i = 0; i < n; i++) { det *= C[nC*i + i]; }
    gsel_diagonalize(n, nC, C, 0);
    gsel_normalize(n, nC, C, 0);
    for (i = 0; i < n; i++) 
      { double *Cij = &(C[nC*i + n]); 
        double *Mij = &(M[nM*i]);
        for (j = 0; j < nM; j++) { (*Mij) = (*Cij); Mij++; Cij++; }
      }
    free(C);
    return det;
  }
  
double rmxn_inv_full (int n, double *A, double *M)
  { 
    int i, j, t;
    /* Copy {A} into {M}: */
    t = 0;
    for (i = 0; i < n; i++) { for (j = 0; j < n; j++) { M[t] = A[t]; t++; } }
 
    double det = 1.0; /* Accumulates the determinant. */
 
    int prow[n], pcol[n]; 
    int k = 0;
    
    while (k < n)
      { /* Process the remaining {n-k}x{n-k} submatrix starting at {A[k][k]}. */

        /* Find the element with largest abs value in the submatrix. */
        /* Store its value in {biga}, its indices in {prow[k],pcol[k]}: */
        prow[k] = k;
        pcol[k] = k;
        double biga = M[k*n+k];
        for (i = k; i < n; i++)
          { for (j = k; j < n; j++)
              { if (fabs (M[i*n+j]) > fabs (biga))
                  { biga = M[i*n+j]; prow[k] = i; pcol[k] = j; }
              }
          }
 
        /* Accumulate the determinant: */
        det *= biga; /* Product of pivots */

        /* If the matrix is all zeros, we break here: */
        if (biga == 0.0) { break; }
        
        /* Swap rows {k} and {prow[k]}: */
        i = prow[k];
        if (i > k)
          { for (j = 0; j < n; j++)
              { double hold = -M[k*n+j];
                M[k*n+j] = M[i*n+j];
                M[i*n+j] = hold;
              }
          }
 
        /* Swap columns {k} and {pcol[k]}: */
        j = pcol[k];
        if (j > k)
          { for (i = 0; i < n; i++)
              { double hold = -M[i*n+k];
                M[i*n+k] = M[i*n+j];
                M[i*n+j] = hold;
              }
          }
 
        /* Negate column {k} and divide by minus the pivot {biga}. */
        for (i = 0; i < n; i++) { if (i != k) { M[i*n+k] /= -biga; } }
 
        /* Reduce the matrix: */
        for (i = 0; i < n; i++)
          { if (i != k)
              { double hold = M[i*n+k];
                for (j = 0; j < n; j++)
                  { if (j != k) { M[i*n+j] += hold * M[k*n+j]; } }
              }
          }
 
        /* Divide row {k} by pivot {biga}: */
        for (j = 0; j < n; j++) { if (j != k) { M[k*n+j] /= biga; } }
 
        /* Set the diagonal element to its reciprocal: */
        M[k*n+k] = 1.0 / biga;
         
        /* Shrink: */
        k++;
      }
 
    /* Undo the row and column interchanges, transposed: */
    while(k > 0)
      { 
        /* Back up in {k}: */
        k--;
      
        /* Swap column {k} with column {prow[k]}: */
        i = prow[k];
        if (i > k)
          { for (j = 0; j < n; j++) 
              { double hold = M[j*n+k];
                M[j*n+k] = -M[j*n+i];
                M[j*n+i] = hold;
              }
          }
 
        /* Swap row {k} with row {pcol[k]}: */
        j = pcol[k];
        if (j > k)
          { for (i = 0; i < n; i++)
              { double hold = M[k*n+i];
                M[k*n+i] = -M[j*n+i];
                M[j*n+i] = hold;
              }
          }
      }

    /* Return the determinant: */
    return det;
  }


void rmxn_scale(int m, int n, double s, double *A, double *M) 
  { int i, j, k = 0;
    for (i = 0; i < m; i++)
      { for (j = 0; j < n; j++)
          { M[k] = s*A[k]; k++; }
      }
  }

void rmxn_mix(int m, int n, double s, double *A, double t, double *B, double *M)
  { int i, j, k = 0;
    for (i = 0; i < m; i++)
      { for (j = 0; j < n; j++)
          { M[k] = s*A[k] + t*B[k]; k++; }
      }
  }

void rmxn_rel_diff(int m, int n, double *A, double *B, double *M)
  { int i, j, k = 0;
    for (i = 0; i < m; i++)
      { for (j = 0; j < n; j++)
          { M[k] = rel_diff(A[k], B[k]); k++; }
      }
  }

double rmxn_norm_sqr(int m, int n, double *A)
  { double s = 0;
    int i, j, k = 0;
    for (i = 0; i < m; i++)
      { for (j = 0; j < n; j++)
          { double Aij = A[k]; k++; 
            s += Aij*Aij;
          }
      }
    return s;
  }

double rmxn_norm(int m, int n, double *A)
  { double s = 0;
    int i, j, k = 0;
    for (i = 0; i < m; i++)
      { for (j = 0; j < n; j++)
          { double Aij = A[k]; k++; 
            s += Aij*Aij;
          }
      }
    return sqrt(s);
  }

double rmxn_mod_norm_sqr(int n, double *A)
  {
    double s = 0.0;
    int i, j;
    int k = 0;
    for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
        { double Aij = A[k]; k++; 
          double Dij = Aij - (i == j ? 1 : 0);
          s += Dij*Dij;
        }
    return s; 
  }

double rmxn_max_abs_elem(int m, int n, double *A)
  { int i, j;
    double emax = 0.0;
    for (i = 0; i < m; i++)
      { for (j = 0; j < n; j++)
          { double Mij = fabs(A[i*n + j]);
            if (Mij > emax) { emax = Mij; }
          }
      }
    return emax;
  }

void rmxn_LT_inv_map_row(int n, double *y, double *L, double *r)
  { int i, j;
    for (j = n-1; j >= 0; j--)
      { double sum = y[j], corr = 0.0;
        for (i = j+1; i < n; i++)
          { double term = -r[i]*L[n*i + j];
            /* Kahan's summation: */
            double tcorr = term - corr;
            double newSum = sum + tcorr;
            corr = (newSum - sum) - tcorr;
            sum = newSum;
          }
        r[j] = sum/L[n*j + j];
      }
  }

void rmxn_LT_inv_map_col(int m, double *L, double *y, double *r)
  { int i, j;
    for (i = 0; i < m; i++)
      { double sum = y[i], corr = 0.0;
        for (j = 0; j < i; j++)
          { double term = -L[m*i + j]*r[j];
            /* Kahan's summation: */
            double tcorr = term - corr;
            double newSum = sum + tcorr;
            corr = (newSum - sum) - tcorr;
            sum = newSum;
          }
        r[i] = sum/L[m*i + i];
      }
  }

void rmxn_LT_pos_div(int m, int n, double *A, double *L, double *M)
  { int i, j, k;
    for (k = 0; k < m; k++)
      { for (j = n-1; j >= 0; j--)
          { double sum = A[n*k + j], corr = 0.0;
            for (i = j+1; i < n; i++)
              { double term = -M[n*k + i]*L[n*i + j];
                /* Kahan's summation: */
                double tcorr = term - corr;
                double newSum = sum + tcorr;
                corr = (newSum - sum) - tcorr;
                sum = newSum;
              }
            M[n*k + j] = sum/L[n*j + j];
          }
      }
  }

void rmxn_LT_pre_div(int m, int n, double *L, double *A, double *M)
  { int i, j, k;
    for (k = 0; k < n; k++)
      { for (i = 0; i < m; i++)
          { double sum = A[n*i + k], corr = 0.0;
            for (j = 0; j < i; j++)
              { double term = -L[m*i + j]*M[n*j + k];
                /* Kahan's summation: */
                double tcorr = term - corr;
                double newSum = sum + tcorr;
                corr = (newSum - sum) - tcorr;
                sum = newSum;
              }
            M[n*i + k] = sum/L[m*i + i];
          }
      }
  }

void rmxn_cholesky(int n, double *A, double *L)
  { 
    /* Andre-Louis Cholesky (spelled with a 'y'), born in France in
      1875, was a geodesist in the French military. He developed his
      computational procedure to solve geodetic problems, among which
      was, in the words of his eulogy, the "problem of levelling in
      Morocco." He died in battle in 1918. Benoit published the
      computational method in "Bulletin geodesique" in 1924. 
        -- David Pattison, Washington, DC

      Although the name is originally Polish or Russian, Cholesky 
      probably pronounced it himself in the French fashion, i.e.
      with an "sh" sound, as in "Chopin" and "Chostakovich"; and not
      with a "tsh" sound --- which in French would be spelled "Tch",
      as in "Tchaikovski" or "Tchekov". */
  
    int i, j, k;

    for (i = 0; i < n; i++)
      { double Lii;
        /* Compute {L[i,j] = X[i,j] - SUM{ L[i,k]*A[j,k] : k = 0..j-1 })/L[j,j]} */
        /* where X[i,j] = (j <= i ? A[i,j] : 0) */
        for (j = 0; j < i; j++)
          { double sum = 0.0, corr = 0.0;
            for (k = 0; k < j; k++)
              { double term = L[n*i + k]*L[n*j + k];
                /* Kahan's summation formula: */
                double tcorr = term - corr;
                double newSum = sum + tcorr;
                corr = (newSum - sum) - tcorr;
                sum = newSum;
              }
            { double Ljj = L[n*j + j];
              affirm(Ljj != 0.0, "zero element in diagonal");
              L[n*i + j] = (A[n*i + j] - sum)/Ljj;
            } 
          }
        
        /* Compute {L[i,i] = sqrt(A[i,i] - SUM{ L[i,j]^2 : j = 0..i-1 })} */
        { double sum = 0.0, corr = 0.0;
          for (j = 0; j < i; j++)
            { double w = L[n*i+j], term = w*w; 
              /* Kahan's summation formula: */
              double tcorr = term - corr;
              double newSum = sum + tcorr;
              corr = (newSum - sum) - tcorr;
              sum = newSum;
            }
          Lii = A[n*i+i] - sum;
        }
        affirm (Lii >= 0.0, "matrix is not positive definite?");
        L[n*i + i] = sqrt(Lii);
        for (j = i+1; j < n; j++) { L[n*i + j] = 0.0; }
      }
  }

void rmxn_print (FILE *f, int m, int n, double *A)
  { rmxn_gen_print(f, m, n, A, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }

void rmxn_gen_print
  ( FILE *f, int m, int n, double *A,
    char *fmt, 
    char *olp, char *osep, char *orp,
    char *ilp, char *isep, char *irp
  )
  {
    int i,j, t;
    if (olp == NULL) { olp = "(\n"; }
    if (osep == NULL) { osep = "\n"; }
    if (orp == NULL) { orp = "\n)"; }
    if (ilp == NULL) { ilp = "  ("; }
    if (isep == NULL) { isep = " "; }
    if (irp == NULL) { irp = ")"; }
    if (fmt == NULL) { fmt = "%16.8e"; }
    fputs(olp, f);
    t = 0;
    for (i = 0; i < m; i++)
      {
        if (i > 0) { fputs(osep, f); }
        fputs(ilp, f);
        for (j = 0; j < n; j++) 
          { if (j > 0) { fputs(isep, f); }
            fprintf(f, fmt, A[t]); t++;
          }
        fputs(irp, f);
      }
    fputs(orp, f);
    fflush(f);
  }  


double *rmxn_alloc(int m, int n)
  { void *p = malloc(m*n*sizeof(double));
    affirm(p != NULL, "no memory for rmxn_t");
    return (double *)p;
  }

