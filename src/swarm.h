/*
** Purpose:
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#pragma once

/*--------------- Includes Needed for Definitions Below --------------*/

#include "agent.h"
#include "support_grids.h"

/*---------------------------- Constants -----------------------------*/

/*------------------------------ Macros ------------------------------*/

/*------------------------------- Types ------------------------------*/

typedef struct swarm_element_hold {

  agent_ptr agent;
  
  struct swarm_element_hold *next_agt, *prev_agt;

} swarm_element_struct;

/*------------------------- Function Prototypes ----------------------*/
/* void Sample (char *, int, Real);                                   */

// PURPOSE
//          This function initializes the swarm of agents to the size 
//          specified by the swarm initialization file                
// INPUT
//          agent_grid
//            - Pointer to the agent grid
//          agentFunctionFileName
//            - The name of the file to read the agent information from.
// OUTPUT
//          swarm     
//            - Ptr to first agent in linked list of agents     
// RETURN
//          SUCCESS
//            - Indicates successful initialization of swarm    
//          FAILURE
//            - Indicates failure to initialize swarm           

int  initialize_swarm (  fgrid_ptr agent_grid, char* agentFunctionFileName );
void update_swarm     (  );

/*------------------------------ Globals -----------------------------*/
/* GLOBAL  float  example;                                            */

/* NOTHING GOES BEYOND THIS LINE !!!! */