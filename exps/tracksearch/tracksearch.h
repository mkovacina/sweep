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

/* the Bounce Function */

void bounce PROTO((agent_struct *, int));

/* the Location Function */

void location PROTO((agent_struct *, int));

/* the Offset Function */

void offset PROTO((agent_struct *, int));

/* the Target Function */

void target PROTO((agent_struct *, int));

/* the Scan Function */

void scan PROTO((agent_struct *, int));


/* additive placement routines */

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _INCLUDED_TRACKSEARCH_H */




