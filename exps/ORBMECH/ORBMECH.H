/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _orb_mech_actions
#define _orb_mech_actions

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/
/*--------------------------------- Macros ----------------------------------*/
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void		Sample PROTO((char *, int, Real)); */


/* additive placement routines */
void x_setpos PROTO((agent_struct *, int));
void y_setpos PROTO((agent_struct *, int));
void z_setpos PROTO((agent_struct *, int));

void x_setvel PROTO((agent_struct *, int));
void y_setvel PROTO((agent_struct *, int));
void z_setvel PROTO((agent_struct *, int));

void x_thrust PROTO((agent_struct *, int));
void y_thrust PROTO((agent_struct *, int));
void z_thrust PROTO((agent_struct *, int));

void orbit_update PROTO((agent_struct *, int));

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _orb_mech_actions */




