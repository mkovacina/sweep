/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _bridge4_actions
#define _bridge4_actions

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/
/*--------------------------------- Macros ----------------------------------*/
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void		Sample PROTO((char *, int, Real)); */


/* additive placement routines */
void leader_wander        PROTO((agent_struct *, int));
void leader_recruit       PROTO((agent_struct *, int));
void follower_halt        PROTO((agent_struct *, int));
void follower_follow      PROTO((agent_struct *, int));
void follower_wander_info PROTO((agent_struct *, int));


/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _bridge4_actions */




