/*
** Purpose:	
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _actions
#define _actions

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"

/*-------------------------------- Constants --------------------------------*/
/*--------------------------------- Macros ----------------------------------*/
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/* void		Sample PROTO((char *, int, Real)); */


/* absolute movement action routines */
void move_up PROTO((agent_struct *, int));
void move_down PROTO((agent_struct *, int));
void move_left PROTO((agent_struct *, int));
void move_right PROTO((agent_struct *, int));
void move_any PROTO((agent_struct *, int));
void pick_any PROTO((agent_struct *, int));
void move_wander PROTO((agent_struct *, int));
void safe_wander PROTO((agent_struct *, int));

/* relative movement action rotuiens */
void turn_right PROTO((agent_struct *, int));
void turn_left PROTO((agent_struct *, int));
void move_forward PROTO((agent_struct *, int));
void move_backward PROTO((agent_struct *, int));

void set_radius PROTO((agent_struct *, int));
void circle PROTO((agent_struct *, int));

void reset_field PROTO((agent_struct *, int));
void inc_field PROTO((agent_struct *, int));
void dec_field PROTO((agent_struct *, int));

void set_id PROTO((agent_struct *, int));
void message1 PROTO((agent_struct *, int));
void message2 PROTO((agent_struct *, int));
void set_field_pri PROTO((agent_struct *, int));

/* specialized action routines */
void climb_gradient PROTO((agent_struct *, int));

/* general action routines */
void time_tag PROTO((agent_struct *, int));
void place PROTO((agent_struct *, int));
void modify PROTO((agent_struct *, int));
void pump_chem PROTO((agent_struct *, int));
void no_op PROTO((agent_struct *, int));
void deactivate PROTO((agent_struct *, int));

/* user actions (experiment dependent) */
#include "user_actions.h"

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /* _actions */




