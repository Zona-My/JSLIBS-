/* rn.h --- operations on points and vectors of R^n */
/* Last edited on 2021-08-20 16:07:37 by stolfi */
/* 
  Based on VectorN.mg, created  95-02-27 by J. Stolfi.
*/

#ifndef rn_H
#define rn_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>

#include <bool.h>

void rn_zero (int32_t n, double *r);
  /* Sets {r} to the zero vector. */
  
void rn_all (int32_t n, double x, double *r);
  /* Sets all coordinates of {r} to the value {x}. */
  
void rn_axis (int32_t n, int32_t i, double *r);
  /* Sets {r} to the {i}th vector of the canonical basis. */

void rn_copy (int32_t n, double *a, double *r);
  /* Copies {a} into {r}; i.e. sets {r[i] = a[i]} for {i} in {0..n-1}. */
  
void rn_add (int32_t n, double *a, double *b, double *r);
  /* Sets {r = a + b}. */

void rn_sub (int32_t n, double *a, double *b, double *r);
  /* Sets {r = a - b}. */

void rn_neg (int32_t n, double *a, double *r);
  /* Sets {r} to {-a}. */

void rn_scale (int32_t n, double s, double *a, double *r);
  /* Sets {r := s * a}. */

void rn_shift (int32_t n, double s, double *a, double *r);
  /* Sets {r[i] := s + a[i]} for {i} in {0..n-1}. */

void rn_mix (int32_t n, double s, double *a, double t, double *b, double *r);
  /* Sets {r := s * a + t * b}. */

void rn_mix_in (int32_t n, double s, double *a, double *r);
  /* Sets {r := r + s * a}. */

void rn_weigh (int32_t n, double *a, double *w, double *r);
  /* Sets {r[i] := a[i] * w[i]}. */

void rn_unweigh (int32_t n, double *a, double *w, double *r);
  /* Sets {r[i] := a[i] / w[i]}. */
  
void rn_rot_axis (int32_t n, double *a, int32_t i, int32_t j, double ang, double *r);
  /* Sets {r} to {a} after a rotation that moves axis {i} towards 
    axis {j} by {ang} radians, leaving all other coordinates unchanged. */

double rn_sum (int32_t n, double *a);
  /* Returns the sum of all elements {a[0..n-1]}. */

double rn_norm (int32_t n, double *a);
  /* Returns the Euclidean length of {a}. */

double rn_norm_sqr (int32_t n, double *a);
  /* Returns the square of the Euclidean length of {a}. */

double rn_L_inf_norm (int32_t n, double *a);
  /* Returns the L-infinity norm of {a} (max absolute coordinate). */

double rn_dist (int32_t n, double *a, double *b);
  /* Returns the Euclidean distance between {a} and {b}. */

double rn_dist_sqr (int32_t n, double *a, double *b);
  /* Returns the square of the Euclidean distance between {a} and {b}. */

double rn_L_inf_dist (int32_t n, double *a, double *b);
  /* Returns the L-infinity distance between {a} and {b} 
    (max absolute diff). */
 
double rn_dir (int32_t n, double *a, double *r);
  /* Sets {r} to {a} normalized to unit Euclidean length; 
    returns the original length. */

double rn_L_inf_dir (int32_t n, double *a, double *r);
  /* Sets {r} to the vector {a/rn_L_inf_norm(a)}; 
    returns the original norm. */

double rn_dot (int32_t n, double *a, double *b);
  /* Dot product of vectors {a} and {b}. */

double rn_cos (int32_t n, double *a, double *b);
  /* Cosine of angle between vectors {a} and {b}. */

double rn_sin (int32_t n, double *a, double *b);
  /* Absolute sine of angle between vectors {a} and {b}. */

double rn_angle (int32_t n, double *a, double *b);
  /* Angle between vectors {a} and {b}, in radians. */

void rn_cross (int32_t n, double **a, double *r);
  /* Sets {r} to the `cross product' of the {n-1} given {n}-vectors
    {a[0..n-2]}; namely, a vector perpendicular to {a[0..n-2]}, whose
    length is the {n}-dimensional measure of the parallelotope defined
    by those vectors. */
  
double rn_det (int32_t n, double **a);
  /* Returns the determinant of the {n} x {n} matrix whose rows 
    are tne {n}-vectors {a[0..n-1]}. */

double rn_decomp (int32_t n, double *a, double *u, double *para, double *perp);
  /* Sets {para} and {perp} (when not NULL) to the components of 
    {a} that are parallel and perpendicular to to {u}, respectively.
    Namely, {para = c*u} and {perp = a - c*u}, where 
    {c = rn_dot(n,a,u)/rn_dot(n,u,u)}. Also returns {c}. */

double rn_mirror (int32_t n, double *a, double *u, double *r);
  /* Stores into {r} the vector {a} mirrored in the direction
    of the unit vector {u}, namely {a - 2*dot(a,u)*u}.
    Also returns the value of {dot(a,u)}. */

void rn_throw_cube (int32_t n, double *r);
  /* Sets {r} to a uniformly random point of the {n}-cube {[-1 _ +1]^n}. */
 
void rn_throw_dir (int32_t n, double *r);
  /* Sets {r} to a random direction of {R^n}; that is, a 
    uniformly random point on {S^{n-1}}, the {(n-1)}-dimensional 
    unit sphere of {R^n}. */

void rn_throw_ball (int32_t n, double *r);
  /* Sets {r} to a uniformly random point of the unit {n}-ball. */

void rn_throw_normal (int32_t n, double *r);
  /* Sets each coordinate {r[i]} to an independent Gaussian random
    number with zero mean and unit standard deviation. */

double rn_abs_rel_diff(int32_t n, double *a, double *b, double abs_tol, double rel_tol);
  /* Computes the maximum difference between each pair {a[i],b[i]},
    divided by {abs_tol} or {rel_tol} times the largest of the two
    elements. See {abs_rel_diff} in {jsmath.h} for details. */

void rn_print (FILE *f, int32_t n, double *a);
  /* Prints {a} on file {f}, with some default format. */

void rn_gen_print
  ( FILE *f, int32_t n, double *a, 
    char *fmt, 
    char *lp, char *sep, char *rp
  );
  /* Prints {a} on file {f}, formatting each coordinate with {fmt}. 
    The strings {lp}, {sep}, and {rp} are printed respectively before,
    between, and after all the coordinates of {a}.  When NULL, they default 
    to "%16.8e", "(", " ", and ")", respectively. */

/* HEAP ALLOCATION */

double *rn_alloc(int32_t n);
  /* Allocates {n} {double}s on the heap; bombs out if no mem. */

/* SPHERE INTEGRALS */
   
double rn_ball_vol(double r, int32_t d);
  /* Returns the volume of the {d}-ball with radius {r}, namely
    {V(d) = r^d * Pi^(d/2)/((d/2)!)} */

double rn_ball_cap_vol_frac_pos(int32_t d, double u);
  /* Returns the fraction {S(d,u)} of the volume of the unit {d}-ball
    that is contained in the slice between {x=-1} and {x=u},
    for {u} in {[-1,+1]}. */

double rn_ball_cap_vol_frac_ang(int32_t d, double w);
  /* Returns the fraction of the volume of the unit {d}-ball contained
    in the slice between {x=0} and {x=sin(w)}, for {w} in
    {[-PI/2,PI/2]}; namely, 
    
      {F(d,w) = (V(d-1)/V(d))*integral((cos(z))^d, z \in 0 _ w)}
      
    where {V(d)} is the volume of the unit {d}-ball. */

/* DERIVED TYPES */

typedef bool_t rn_pred_t(int32_t n, double *a);
  /* Type of a function that returns a {bool_t} value from an {double} array. */

typedef double rn_double_func_t(int32_t n, double *a);
  /* Type of a function that returns a {double} value from an {double} array. */

#endif
