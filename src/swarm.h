/*
** Purpose:
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#pragma once

/*--------------- Includes Needed for Definitions Below --------------*/

#include "file.h"
#include "agent.h"
#include "support_grids.h"

/*---------------------------- Constants -----------------------------*/

/*------------------------------ Macros ------------------------------*/

/*------------------------------- Types ------------------------------*/

typedef struct swarm_unit 
{
	// so this is a list of units, and each unit contains an agent.
	// why?
	// what else could be put in this unit?
	// is the unit really just the agent, and if so can this be simplified?
	// and what if we instead just have a MAX_SIZE array?
  agent_ptr agent;
  
  struct swarm_unit *next_agt, *prev_agt;
} 
swarm_element_struct;

typedef struct 
{
	swarm_element_struct* head;
}
Swarm;

/*------------------------- Function Prototypes ----------------------*/

// PURPOSE
//          This function initializes the swarm of agents to the size 
//          specified by the swarm initialization file                
// INPUT
//          agent_grid
//            - Pointer to the agent grid
//          experimentFiles
//            - The set of files that define the experiment
// OUTPUT
//          swarm     
//            - Ptr to first agent in linked list of agents     
// RETURN
//          SUCCESS
//            - Indicates successful initialization of swarm    
//          FAILURE
//            - Indicates failure to initialize swarm           

int  initialize_swarm( Swarm* swarm, const ExperimentFiles* experimentFiles );
void update_swarm(Swarm* swarm, pri_func_ptr priority_grid[], s_grids_ptr all_support_grids);

/*------------------------------ Globals -----------------------------*/
/* GLOBAL  float  example;                                            */
