/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _INCLUDED_TRACKSEARCH_H
#define _INCLUDED_TRACKSEARCH_H

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/
/*--------------------------------- Macros ----------------------------------*/
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void		Sample PROTO((char *, int, Real)); */

/* the Initialize Function */

void initialize PROTO((agent_struct *, int));

/* the read_msg Function */

void read_msg PROTO((agent_struct *, int));

/* the broadcast Function */

void broadcast PROTO((agent_struct *, int));

/* the point to point Function */

void ptop PROTO((agent_struct *, int));

/* the best_get Function */

void best_get PROTO((agent_struct *, int));

/* the measuring Function */

void measure PROTO((agent_struct *, int));

/* tar_move Function, homes in on target */

void tar_move PROTO((agent_struct *, int));



/* additive placement routines */

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _INCLUDED_TRACKSEARCH_H */




