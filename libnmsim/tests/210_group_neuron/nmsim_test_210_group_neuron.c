#define PROG_NAME "nmsim_test_210_group_neuron"
#define PROG_DESC "basic tests of {limnmism} neuron population attributes"
#define PROG_VERS "1.0"

/* Last edited on 2019-03-14 17:23:55 by jstolfi */ 

#define PROG_COPYRIGHT \
  "Copyright © 2019  State University of Campinas (UNICAMP)"
  
#define PROG_AUTH \
  "Created by J. Stolfi on 2019-01-06"
  
#define PROG_HIST
  
#define PROG_HELP \
  "  " PROG_NAME ""

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <bool.h>
#include <argparser.h>
#include <jsfile.h>
#include <jsrandom.h>
#include <affirm.h>

#include <nmsim_group_neuron.h>

void nmsim_group_neuron_test_write(char *fname, nmsim_group_neuron_ix_t ing, nmsim_group_neuron_t *ngrp);
  /* Writes the neuron population attributes record {ngrp} to file {fname},
    assuming index {ing}. */

void nmsim_group_neuron_test_read(char *fname, nmsim_group_neuron_ix_t ing, nmsim_group_neuron_t *ngrp);
  /* Reads a neuron population index and attributes record from file {fname}, and compares
    them to {ing} and {ngrp}. */

int main(int argc, char **argv);

/* IMPLEMENTATIONS: */

int main(int argc, char **argv)
  { 
    nmsim_class_neuron_ix_t inc_max = 418;
    nmsim_elem_neuron_count_t nne_g_min = 10;
    nmsim_elem_neuron_count_t nne_g_max = 20;
    nmsim_group_neuron_t ngrp = nmsim_group_neuron_throw(inc_max, nne_g_min, nne_g_max);
    nmsim_group_neuron_show(stderr, "neuron group = ", &ngrp, "\n");
    char *fname = "out/test_neuron_group.txt";
    nmsim_group_neuron_ix_t ing = 17;
    nmsim_group_neuron_test_write(fname, ing, &ngrp);
    nmsim_group_neuron_test_read(fname, ing, &ngrp);
    return 0;
  }
  
void nmsim_group_neuron_test_write(char *fname, nmsim_group_neuron_ix_t ing, nmsim_group_neuron_t *ngrp)
  { FILE *wr = open_write(fname, TRUE);
    nmsim_group_neuron_write(wr, ing, ngrp);
    fprintf(wr, "\n");
    fclose(wr);
  }

void nmsim_group_neuron_test_read(char *fname, nmsim_group_neuron_ix_t ing, nmsim_group_neuron_t *ngrp)
  {
    FILE *rd = open_read(fname, TRUE);
    nmsim_class_neuron_ix_t inc_max = ngrp->inc;
    nmsim_group_neuron_t ngrp_read = nmsim_group_neuron_read(rd, ing, inc_max);
    fclose(rd);
    
    nmsim_group_neuron_compare(&ngrp_read, ngrp);
  }

