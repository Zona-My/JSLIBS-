#ifndef lsq_H
#define lsq_H

/* Fits a linear map of {R^nx} to {R^nf} by least squares, given sampling proc. */
/* Last edited on 2019-12-18 15:59:51 by jstolfi */

#define lsq_H_COPYRIGHT \
  "Copyright � 2006  by the State University of Campinas (UNICAMP)"

#include <stdint.h>
#include <bool.h>

typedef void lsq_gen_data_point_t
  ( int32_t k, 
    int32_t nx, 
    double Xk[], 
    int32_t nf, 
    double Fk[], 
    double *WkP
  );
  /* The type of a client-implemented procedure that yields one /data
    point/ for the purpose of least squares fitting. The data point
    consists of a set of /argument coordinates/ (independent variables)
    {Xk}, a corresponding set of /function samples/ (dependent
    variables) {Fk}, and a /reliability weight/ {Wk}.
    
    The parameter {k} is the sequential index of the data point,
    starting from 0. The procedure should store into {Xk[0..nx-1]} the
    argument coordinates of the data point, and into {Fk[0..nf-1]} the
    corresponding function samples. The procedure should also store into
    {*WkP} the (non-negative) weight {Wk} of the data point. */

int32_t lsq_fit
  ( int32_t nt,     /* Number of data points to generate. */
    int32_t nx,     /* Number of argument coordinates per point. */
    int32_t nf,     /* Number of function samples per data point. */
    lsq_gen_data_point_t *gen_data_point,
    double U[], /* (OUT) Fitted linear transformation matrix. */
    bool_t verbose
  );
  /* Finds the linear function {s} from {R^nx} to {R^nf} that best
    approximates some function {f} from {R^nx} to {R^nf}, over a given
    set of data points, by weighted least squares.
    
    The procedure calls {gen_data_point(k,nx,Xk,nf,Fk,&Wk)}, with {k}
    varying from 0 to {nt-1}, to generate the argument coordinates
    {Xk[0..nx-1]}, the function samples {Fk[0..nf-1]}, and the
    reliability weight {Wk} for each data point {k}. The weighted least
    squares method assumes that the function samples {Fk[0..nf-1]} are
    measurements of some ``true'' linear function {f} from {R^nx} to
    {R^nf}, whose actual (unknown) value at {Xk[0..nx-1]} is
    {Tk[0..nf-1]}, perturbed by the addition of {nf} measurement errors
    {Nk[0..nf-1]}, with independent Gaussian distributions with zero
    mean and the same variance {1/Wk}. 
    
    The procedure stores into argument {U} the {nx � nf} coefficient matrix of
    the linear function {s} that maximizes the likelihood of the data
    points. The parameter {U} should have {nx*nf} elements,
    and the matrix will be packed by rows; that is, conceptual 
    element {U[i,j]} is {U[i*nf + j]}.
    
    The approximation is defined as the vector-matrix product {s(x) = x*U}.
    Namely, column {j} of {U} is the coefficient vector of the
    linear combination of {x[0..nx-1]} that best approximates coordinate
    {j} of the function {f}, in the sense of minimizing the sum of
    squared errors {Wk*|Xk*U[*,j] - Fk[j]|^2} over all data points data
    points {Xk,Fk,Wk}.
    
    Note that only the relative values of the weights are relevant. A
    data point with zero weight is ignored. Moreover, for any integer
    {N}, a data point that has weight {Wk} counts the same as {N} data
    points with same arguments and function samples, but with weight
    {Wk/N}.
    
    !!! To implement: if {Wk} is set to {+INF} by {gen_data_point}, it means
    that the function {s} should interpolate {Fk} at {Xk}. !!!
    
    !!! Eliminate {nt} parameter and make {gen_data_point} return TRUE on EOF. !!!

    The procedure returns the rank {r} of the least-squares system. If {r} is less
    than {nx}, it means that the {Xk} vectors generated by {gen_data_point}
    belonged to a proper linear subspace of {R^{nx}}. In that case, the
    best-fit map is not unique.
    
    This procedure uses storage proportional to {nx*(nx+nf)},
    independently of the number of data points {nt}. Therefore, it may be
    preferrable to {lsq_fit_arrays} when the data points are too numerous to
    fit in memory at once. */

/* AUXILIARY PROCEDURES */

void lsq_compute_matrix_and_rhs
  ( int32_t nt, 
    int32_t nx, 
    int32_t nf, 
    lsq_gen_data_point_t *gen_data_point, 
    double A[], 
    double B[], 
    bool_t verbose
  );
  /* Calls {gen_data_point(k,nx,Xk,nf,Fk,&Wk)} for {k} in {0..nt-1}, thus
    obtaining {Xk[0..nx-1]}, {Fk[0..nf-1}, and {Wk}. Computes from all those values
    the moment matrix {A} ({nx � nx}) and the right-hand side matrix
    {B} ({nx � nf}). */

int32_t lsq_solve_system
  ( int32_t nx, 
    int32_t nf, 
    double A[], 
    double B[], 
    int32_t nc, 
    double R[], 
    double S[], 
    double U[], 
    double L[], 
    bool_t verbose
  );
  /* Computes the solution matrix {U} ({nx � nf}) of the least squares problem with moment matrix {A}
    ({nx � nx}) and right-hand side {B} ({nx � nf}).  
    
    If {nc > 0}, assumes that each colums of the solution is constrained by {nc} affine (first-degree)
    equations.  In that case, the constraints are supposed to be {R * U = S}, where
    {R} must be an {nc � nx} matrix and {S} an {nc � nf} matrix. In that  case,
    it returns into matrix {L} ({nc � nf}) the Lagrange multipliers associated with the
    constraints.
    
    If {nc} is zero, the parameters {R}, {S}, and {L} are ignored, and may be {NULL}.
    In that case, the procedure simply solves the system {A * U = B} for {U}. 
    
    The procedure returns the apparent rank of the systems, that is, the number
    of linearly independent rows in {A} and {R}. */
 
void lsq_debug_double_vec(int32_t nx, double x[], char *fmt);
void lsq_debug_int32_t_vec(int32_t nx, int32_t x[], char *fmt);
  /* These procedures print {x[0..nx-1]} to {stderr}, each with format {fmt},
    separated by spaces and bracketed by '[' and ']'. */

#endif
