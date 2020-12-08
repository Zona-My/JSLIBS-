#ifndef btc_price_series_read_H
#define btc_price_series_read_H

/* Reading a BTC price series. */
/* Last edited on 2015-04-20 00:43:56 by stolfilocal */

#define _GNU_SOURCE
#include <stdio.h>

void btc_price_series_read(FILE* rd, int* ndP, char*** dtP, double **apP);
  /* Reads a daily price series {ap[0..nd-1]} and the respective dates {dt[0...nd-1]}
    from file {rd}, in the format described in the documentation above.  
    The number of days {nd} in the seris, and the arrays {dt,ap} (allocated by the procedure) are
    returned in {*ndP}, {*dtP}, and {*apP}. */

#endif
