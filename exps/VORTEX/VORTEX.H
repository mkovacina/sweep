/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _INCLUDED_VORTEX_H
#define _INCLUDED_VORTEX_H

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/
/*--------------------------------- Macros ----------------------------------*/
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void		Sample PROTO((char *, int, Real)); */


/* additive placement routines */
void wander_info     PROTO ((agent_struct *, int ));
void neighbor_info   PROTO ((agent_struct *, int ));
void vortex_end_info PROTO ((agent_struct *, int ));
void vortex_mid_info PROTO ((agent_struct *, int ));
void vortex_follow   PROTO ((agent_struct *, int ));

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _INCLUDED_VORTEX_H */




