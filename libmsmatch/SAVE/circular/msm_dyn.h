#ifndef msm_dyn_H
#define msm_dyn_H

/* Dynamic programming tableaus for incremental optimum pairing. */
/* Last edited on 2008-01-11 17:30:31 by stolfi */

#define msm_dyn_H_COPYRIGHT \
  "Copyright � 2005  by the State University of Campinas (UNICAMP)" \
  " and the Federal Fluminense University (UFF)"

#include <msm_basic.h>
#include <msm_rung.h>
#include <msm_pairing.h>

#include <vec.h>
#include <stdint.h>

typedef struct msm_dyn_entry_t
  { double score;    /* Score of optimum path that ends at {g}. */
    msm_rung_t prev;  /* Previous rung in optimum path to {g}, or {msm_rung_none}. */
  } msm_dyn_entry_t;
  /* Entry of a tableau for dynamic programming, corresponding to some
    rung (index pair) {g}. */

vec_typedef(msm_dyn_entry_vec_t,msm_dyn_entry_vec,msm_dyn_entry_t);
  /* A vector of {msm_dyn_entry_t} variables. */

typedef struct msm_dyn_tableau_t
  { int rMin;              /* Minimum R-coord of all entries. */
    int rMax;              /* Maximum R-coord of all entries. */
    int nr;                /* Number of valid R-coord values. */
    int_vec_t sMin;        /* The minimum S-coord for each R-coord {r} is {sMin[r-rMin]}. */
    int_vec_t sMax;        /* The maximum S-coord for each R-coord {r} is {sMax[r-rMin]}. */
    int ns;                /* Max number of S-coord values for any {r}. */
    msm_dyn_entry_vec_t ev; /* Tableau entries. */
  } msm_dyn_tableau_t;
  /* A tableau for incremental dynamic programming. 

    Each entry of a tableau {T} corresponds to a distinct rung between
    two sequences. Let's define the /R-coordinate/ of a rung
    {g=(ix,iy)} as being the sum {r = ix+iy}; and let the
    /S-coordinate/ of {g} be the difference {s = ix-iy}. The rung {g}
    is represented in {T} if and only if {rMin <= r <= rMax}, and
    {sMin[r-rMin] <= s <= sMax[r-rMin]}. The corresponding entry is
    stored in {ev[dr*ns + ds]}, where {dr = r - rMin}, and 
    {ds = (s-sMin[dr])/2}. */
    
msm_dyn_tableau_t msm_dyn_tableau_new(void);
  /* Returns a tableau with zero space, that is, {rMax < rMin}, {nr = ns = 0}. */
    
void msm_dyn_tableau_resize(msm_dyn_tableau_t *tb, int rMin, int rMax, int maxds);
  /* Resizes the tableau {*tb} for a new set of rungs.
    The parameters {rMin} and {rMax} must be the minimum and
    maximum R-coordinate {r = ix+iy} of any rung {g = (ix,iy)} that needs to be
    represented in the tableau. The parameter {maxds} must be the maximum
    difference between the S-coordinates {s = ix-iy}
    of any two valid rungs with the same R-coordinate.
    
    The function sets {tb.rMin = rMin}, {tb.rMax = rMax}, and {tb.ns =
    maxds + 1}. It also recomputes {tb.nr = rMax - rMin + 1} and reallocates
    the vectors {tb.sMin}, {tb.sMax}, and {tb.ev} as needed to contain
    all elements. The bounds {tb.sMin[dr]} and {tb.sMax[dr]} are set
    to an empty interval, for all {dr} in {0..tb.nr-1}. */
    
void msm_dyn_tableau_set_s_range(msm_dyn_tableau_t *tb, int r, int sMin, int sMax);
  /* Sets the values of {sMin[dr]} and {sMax[dr]} to the given values,
    where {dr = r - rMin}. Initializes all tableau elements with R-coordinate
    {r} and S-coordinates in the range {sMin..sMax} to have score {-INF} and
    previous rung {msm_rung_none}. Requires {r} to be in {tb.rMin..tb.rMax},
    and also {sMax - sMin < tb.ns}. */
    
void msm_dyn_tableau_get_s_range(msm_dyn_tableau_t *tb, int r, int *sMin, int *sMax);
  /* Sets {*sMin} and {*sMax} to the current values of {sMin[dr]} and
    {sMax[dr]}, where {dr = r - rMin}. Returns an empty interval for
    any {r} outside of {tb.rMin..tb.rMax}, but possibly also for other
    cases. */

void msm_dyn_tableau_get_X_Y_ranges(msm_dyn_tableau_t *tb, int *xMin, int *xMax, int *yMin, int *yMax);
  /* Sets {*xMin} and {*xMax} to the minimum and maximum X coordinates of any
    entry in {tb}.  Ditto for {*yMin}, {*yMax}, and the Y coordinate.
    Returns empty intervals if and only if the tableau has no entries. */

msm_dyn_entry_t *msm_dyn_tableau_get_entry_address(msm_dyn_tableau_t *tb, msm_rung_t g);
  /* Gets the address of the entry of the element of {tb} that
    corresponds to the rung {g}. Assumes that the fields {sMin[dr]}
    and {sMax[dr]} have been properly set. Returns NULL if the
    requested element is not in the tableau. */

void msm_dyn_tableau_free(msm_dyn_tableau_t *tb);
  /* Frees the internal storage of tableau {*tb} (but not the 
    descriptor {*tb} itself), and resets it to the empty status. */

#endif
