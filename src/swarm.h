/*
** Purpose:
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _swarm_
#define _swarm_

/*--------------- Includes Needed for Definitions Below --------------*/

#include "agent.h"

/*---------------------------- Constants -----------------------------*/

/*------------------------------ Macros ------------------------------*/

/*------------------------------- Types ------------------------------*/

typedef struct swarm_element_hold {

  agent_ptr agent;
  
  struct swarm_element_hold *next_agt, *prev_agt;

} swarm_element_struct;

/*------------------------- Function Prototypes ----------------------*/
/* void Sample (char *, int, Real);                                   */

int  initialize_swarm (  );
void update_swarm     (  );

/*------------------------------ Globals -----------------------------*/
/* GLOBAL  float  example;                                            */

/* NOTHING GOES BEYOND THIS LINE !!!! */
#endif /* _swarm_ */
