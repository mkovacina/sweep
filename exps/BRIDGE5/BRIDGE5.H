/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _bridge5_actions
#define _bridge5_actions

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/
/*--------------------------------- Macros ----------------------------------*/
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void		Sample PROTO((char *, int, Real)); */


/* additive placement routines */
void safe_str_trail PROTO((agent_struct *, int));
void stay_search_trail PROTO((agent_struct *, int));
void follow PROTO((agent_struct *, int));
void staysearch PROTO((agent_struct *, int));
void leader_escape PROTO ((agent_struct *, int));
void escape PROTO ((agent_struct *, int));

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _bridge5_actions */




