/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _emerge4_actions
#define _emerge4_actions

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/
/*--------------------------------- Macros ----------------------------------*/
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void		Sample PROTO((char *, int, Real)); */


/* additive placement routines */
void pick_up PROTO((agent_struct *, int));
void put_down PROTO((agent_struct *, int));
void locate_region PROTO((agent_struct *, int));
void findtarget PROTO((agent_struct *, int));
void staysearch PROTO((agent_struct *, int));
void object_trail PROTO((agent_struct *, int));
void follow PROTO((agent_struct *, int));

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _emerge4_actions */




