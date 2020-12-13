/*
** Purpose:
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _agent_
#define _agent_

/*--------------- Includes Needed for Definitions Below --------------*/

#include "fsa.h"
#include "constants.h"

/*---------------------------- Constants -----------------------------*/

/*------------------------------ Macros ------------------------------*/

#define MAX_NUM_PARAMS 5

/*------------------------------- Types ------------------------------*/

typedef struct {

  int grid;
  
  int num_parameters;

  // allows for MAX_NUM_PARAMS in a action function call */
  float grid_value;  /* [ MAX_NUM_PARAMS ]; */
  
} update_grid_struct;

typedef struct {

  void (*function)();
  
  int number_grids;
  
  update_grid_struct* update_grids;
  
} function_struct;

typedef struct {

  function_struct *function_list;

  int number_functions;

} state_functions_struct;

typedef struct item_hold {

  float fields[MAX_FIELDS];

  struct item_hold *next_item, *prev_item;

} item_struct;

typedef struct {

  int x_pos, 
      y_pos, 
      id_number, 
      dir, 
      switch_dir,
      priority_id, 
      support_grid_id,
      active, 
      radius,
      number_fields;

float update_amt,
      count_amt;
  
  /* GOING TO NEED FIX HERE */
  float chem;

  /* Start of fix */
  float field[MAX_FIELDS];
  
  fsa_struct *fsa;
  
  state_functions_struct *state_functions;

  item_struct *inventory;
  
  int target[2][10];  /*Added 8-8-00, holds target preference list */

} agent_struct;

typedef struct {

  char **list;

  int list_size;

} list_struct; 

typedef struct {

  int number_agent_lists;

  list_struct *agent_list;
  
} agent_function_table_struct;

typedef struct {

  int number_fields;

  float field[MAX_FIELDS];

} agent_field_struct;

typedef struct {

  int number_field_lists;

  agent_field_struct *agent_field_list;

} agent_table_struct;

typedef agent_struct* agent_ptr;


/*------------------------- Function Prototypes ----------------------*/
/* void sample (char *, int, Real);                                   */


// PURPOSE: This function will open the agent file and create the    
//          table of function lists for each agent specified by the  
//          agent file                                               
// INPUT:   
//          agent_function_table  
//            - Pointer to agent function table        
//          agent_function_file_ptr
//            - File pointer to the agent file
// OUTPUT:  
//          agent_function_table
//            - Pointer to initialized function table  
// RETURN:  
//          SUCCESS
//            - Indicates successful initialization             
//          FAILURE
//            - Indicates unsuccessful initialization           
int initialize_agent_function_table ( agent_function_table_struct*, FILE* );
int initialize_agent_table          ( const agent_table_struct*, FILE* );
int initialize_agent                ( agent_ptr*, list_struct,
                                      agent_field_struct,
                                      fsa_struct*, int, int );
int copy_agent                      ( agent_ptr, agent_ptr* );
int delete_agent                    ( agent_ptr* );
int free_agent_table                ( agent_table_struct* );
int free_agent_function_table       ( agent_function_table_struct* );

void print_function_list ( agent_ptr );
void print_agent         ( agent_ptr );

/*------------------------------ Globals -----------------------------*/
/* GLOBAL  float  example;                                            */


/* NOTHING GOES BEYOND THIS LINE !!!! */
#endif /* _agent_ */

