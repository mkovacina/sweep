/*
** Purpose:
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _support_update
#define _support_update

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/


/*--------------------------------- Macros ----------------------------------*/

/* "N" macros are for accessing parts of nseq structs */
/* #define N_LEN(src,x,y) (src)->node[x][y].bvec.length */

/* Macros to read and write values in the support grids */

/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void         Sample PROTO((char *, int, Real)); */

s_grids_ptr init_support_grids PROTO((char []));
fgrid_ptr init_agent_grid PROTO(());

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /*_support_update */


