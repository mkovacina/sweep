/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _emerge_actions
#define _emerge_actions

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
void hill_down PROTO((agent_struct *, int));
void hill_climb PROTO((agent_struct *, int));
void staysearch PROTO((agent_struct *, int));
void locate_obj PROTO((agent_struct *, int));
void safe_str PROTO((agent_struct *, int));

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _emerge_actions */




