/* See bbgoal.h */
/* Last edited on 2005-09-26 00:52:08 by stolfi */

#include <bbgoal.h>
#include <fbb_f1_ia.h>
#include <fbb_f2_ia.h>
#include <fbb_f3_ia.h>

#include <affirm.h>
#include <bool.h>

#include <string.h>

bbgoal_data_t bbgoal_from_tag(char *tag)
  {
    if (strcmp(tag, "f1_ia") == 0) { return fbb_f1_ia_get_data(); }
    if (strcmp(tag, "f2_ia") == 0) { return fbb_f2_ia_get_data(); }
    if (strcmp(tag, "f3_ia") == 0) { return fbb_f3_ia_get_data(); }
    demand(FALSE, "bad function tag");
    return fbb_f1_ia_get_data(); /* To pacify the compiler. */
  }
