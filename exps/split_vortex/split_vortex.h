/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _INCLUDED_SPLIT_VORTEX_H
#define _INCLUDED_SPLIT_VORTEX_H

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/
/*--------------------------------- Macros ----------------------------------*/
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void		Sample PROTO((char *, int, Real)); */

void split_wander_info   PROTO ((agent_struct *, int ));
void split_vortex_info   PROTO ((agent_struct *, int ));
void split_vortex_follow PROTO ((agent_struct *, int ));
void split_info          PROTO ((agent_struct *, int ));

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _INCLUDED_SPLIT_VORTEX_H */




