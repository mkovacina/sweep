/*----------------------- Includes Needed ----------------------------*/

#include "agent.h"
#include "constants.h"
#include "errors.h"
#include "actions.h"
#include "useful.h"

#include <stdlib.h>
#include <string.h>

/*------------------------ Externs Needed --------------------------- */

extern char* AGENT_FUNCTION_FILE_NAME;
extern char* AGENT_FILE_NAME;

/*------------------------- Local Macros -----------------------------*/

/*-------------------------- Local Types -----------------------------*/

typedef struct
{
  void (*function)();

  char *function_name;
} func_table;

/*------------------------ Local Constants ---------------------------*/

func_table function_table[] = 
{
  {   move_left,            "move_left"             },
  {   move_right,           "move_right"            },
  {   move_up,              "move_up"               },
  {   move_down,            "move_down"             },
  {   move_any,             "move_any"              },
  {   pick_any,             "pick_any"              },
  {   move_wander,          "move_wander"           },
  {   modify,               "modify"                },
  {   turn_left,            "turn_left"             },
  {   turn_right,           "turn_right"            },
  {   move_forward,         "move_forward"          },
  {   move_backward,        "move_backward"         },
  {   set_radius,           "set_radius"            },
  {   circle,               "circle"                },
  {   time_tag,             "time_tag"              },
  {   reset_field,          "reset_field"           },
  {   inc_field,            "inc_field"             },
  {   dec_field,            "dec_field"             },
  {   message1,             "message1"              },
  {   message2,             "message2"              },
  {   set_field_pri,        "set_field_pri"         },
  {   set_id,               "set_id"                }, 
  {   climb_gradient,       "climb_gradient"        },
  {   place,                "place"                 },
  {   pump_chem,            "pump_chem"             },
  {   deactivate,           "deactivate"            },
  {   safe_wander,          "safe_wander"           },
  {   no_op,                "no_op"                 },

  /* this line must be left here */
  #include "user_actions.tbl"

  {   0,                    ""                      }
};

/*---------------------- Local Function Prototypes -------------------*/
/* void sample (char *, int, Real);                                   */

void function_set        ( agent_ptr*, int, char* );
void grid_set            ( agent_ptr*, int, int, char* );

int getnumberfunctions ( char* );
int getnumbergrids     ( char* );

int copy_function ( function_struct*, function_struct* );
int copy_grid     ( update_grid_struct*, update_grid_struct* );

/*---------------------- Function Definitions ------------------------*/

int initialize_agent_function_table ( agent_function_table_struct *agent_function_table ) {
/* PURPOSE: This function will open the agent file and create the    */
/*          table of function lists for each agent specified by the  */
/*          agent file                                               */
/* INPUT:   agent_function_table  Ptr to agent function table        */
/* OUTPUT:  agent_function_table  Ptr to initialized function table  */
/* RETURN:  SUCCESS  Indicates successful initialization             */
/*          FAILURE  Indicates unsuccessful initialization           */

  FILE *agent_function_file;
  char buffer[MAX_BUFFER];
  int  i = 0, j = 0;
  
  /* Open agent file */
  agent_function_file = fopen( AGENT_FUNCTION_FILE_NAME, "r");
  if ( agent_function_file == NULL ) {
  
    error( "Can't open agent function file: ");
    error( AGENT_FUNCTION_FILE_NAME );
    error( "\n" );
    return FAILURE;
    
  }
  
  fgets ( buffer, MAX_BUFFER, agent_function_file );
  agent_function_table->number_agent_lists = atoi( buffer );
  
  agent_function_table->agent_list = malloc( sizeof( list_struct ) * 
                                     agent_function_table->number_agent_lists );
 
  /* Eat the next line */
  fgets ( buffer, MAX_BUFFER, agent_function_file );
 
  /* Loop until buffer is #### indicating end of file */
  while ( !strstr( buffer, "####" ) ) {
  
    /* Specifies the number of function lists */
    fgets( buffer, MAX_BUFFER, agent_function_file );
    agent_function_table->agent_list[i].list_size = atoi( buffer );
    agent_function_table->agent_list[i].list = malloc( sizeof( char* ) * 
                                               agent_function_table->agent_list[i].list_size );    

    /* Eat the next line, then get another line */
    fgets( buffer, MAX_BUFFER, agent_function_file );
    fgets( buffer, MAX_BUFFER, agent_function_file );
    
    /* Loop through every line and copy in strings for function names */
    while ( buffer[0] != '#' ) {
    
      agent_function_table->agent_list[i].list[j] = malloc( strlen( buffer ) );
      strcpy( agent_function_table->agent_list[i].list[j], buffer ); 
      fgets( buffer, MAX_BUFFER, agent_function_file );
      j++;
    
    }
    
    /* Eat delimeter between agent function lists */
    i++;
    j = 0;
    
  }
  
  fclose( agent_function_file );
  
  return SUCCESS;
  
}

int initialize_agent_table ( agent_table_struct* agent_table ) {
/* PURPOSE:  This function will create the table of user defined fields */
/* INPUT:    agent_table  Ptr to the agent table to initialize          */
/* OUTPUT:   agent_table  Ptr to the initialized agent table            */
/* RETURN:   SUCCESS      Indicates successful initialization           */
/*           FAILURE      Indicates unsuccessful initialization         */

  FILE *agent_file;
  char buffer[MAX_BUFFER];
  int  i = 0, j = 0;
  
  /* Open agent file */
  agent_file = fopen( AGENT_FILE_NAME, "r");
  if ( agent_file == NULL ) {
  
    error( "Can't open agent file: ");
    error( AGENT_FILE_NAME );
    error( "\n" );
    return FAILURE;
    
  }
  
  fgets ( buffer, MAX_BUFFER, agent_file );
  agent_table->number_field_lists = atoi( buffer );
  
  agent_table->agent_field_list = malloc( sizeof( agent_field_struct ) * 
                                  agent_table->number_field_lists );
 
  /* Eat the next line */
  fgets ( buffer, MAX_BUFFER, agent_file );
 
  /* Loop until buffer is #### indicating end of file */
  while ( !strstr( buffer, "####" ) ) {

    fgets( buffer, MAX_BUFFER, agent_file );
    agent_table->agent_field_list[i].number_fields = atoi( buffer );

    /* Eat the next line, then get another line */
    fgets( buffer, MAX_BUFFER, agent_file );
    fgets( buffer, MAX_BUFFER, agent_file );
    
    /* Loop through every line and copy in strings for function names */
    while ( (buffer[0] != '#') && (j < MAX_FIELDS) ) {
      agent_table->agent_field_list[i].field[j] = atof( buffer );
      j++;

      
      /** M.A.K.   06-12-2001 **/
      fgets( buffer, MAX_BUFFER, agent_file );
    }

    if (j == MAX_FIELDS) {
      error( "\nToo many user defined fields\n" );
      return FAILURE;

    }

    while ( j < MAX_FIELDS ) {
   
      agent_table->agent_field_list[i].field[j] = 0.0;
      j++;

    }
    
    /* Eat delimeter between agent function lists */
    // N.B.  this is actually done at the top of the loop

    i++;
    j = 0;
    
  }
  
  fclose( agent_file );

  return SUCCESS;


}

int initialize_agent ( agent_ptr *agent, list_struct agent_function_list,
                       agent_field_struct field_list,
                       fsa_struct *fsa, int priority_id, int support_id ) {
/* PURPOSE:  This function will initialize the agent in memory and    */
/*           set up its fsa                                           */
/* INPUT:    agent                Ptr to the agent to be initialized  */
/*           fsa                  Ptr to FSA for the agent            */
/*           agent_function_list  List of function for the agent      */
/*           priority_id          Priority ID for the agent           */
/* OUTPUT:   agent                Ptr to initialized agent            */
/*           support_id           Support ID for the agent            */
/* RETURN:   SUCCESS              Indicates successful initialization */
/*           FAILURE              Indicates unsuccessful init         */

  int i = 0, j;
  static int id_count = 1;

  /* Create memory for agent */
  (*agent) = malloc( sizeof( agent_struct ) );
  
  /* Create memory for fsa */
  (*agent)->fsa = malloc( sizeof( fsa_struct ) );
  
  /* Copy fsa */
  copy_fsa( fsa, (*agent)->fsa );

  /* Empty items */
  (*agent)->inventory = NULL;

  /* Make active */
  (*agent)->active = 1;
  
  /* Allocate space for state_functions */
  (*agent)->state_functions = malloc( sizeof( state_functions_struct ) * (*agent)->fsa->number_states );

  (*agent)->priority_id = priority_id;
  (*agent)->support_grid_id = support_id;
  (*agent)->id_number = id_count++;

  /* Copy user defined fields into agent */
  (*agent)->number_fields = field_list.number_fields;
  for (i = 0; i < MAX_FIELDS; i++) (*agent)->field[i] = field_list.field[i];

  (*agent)->chem = INIT_CHEM_AMOUNT;
  (*agent)->dir = -1;
  (*agent)->x_pos = -1;
  (*agent)->y_pos = -1;
  
  /* Trace though state description and find proper function in */
  /* function list                                              */
  i = 0;
  while ( i < (*agent)->fsa->number_states ) {
  
    /* Look in function list for correct description */
    for ( j = 0; j < agent_function_list.list_size; j++ ) {

      if ( strstr( agent_function_list.list[j], (*agent)->fsa->state[i].description) ) break;
      
    }
    
    /* If j is max size, indicate error */
    if ( j == agent_function_list.list_size ) {
    
      error( "State description, " );
      error( (*agent)->fsa->state[i].description );
      error( ", not found in agent file\n" );
      return FAILURE;
      
    }
    
    /* Found proper description set up functions and support grid */
    /* indicators                                                 */
    function_set( agent, i++, agent_function_list.list[j] );
    
  }
  
  return SUCCESS;

}

int free_agent_table ( agent_table_struct *agent_table ) {
/* PURPOSE:  Clears the agent table     */
/* INPUT:    agent_table Table to clear */
/* OUTPUT:   NONE                       */
/* RETURN:   SUCCESS Indicates success  */
/*           FAILURE Indicates failure  */

  free( agent_table->agent_field_list );

  return SUCCESS;

}

int free_agent_function_table ( agent_function_table_struct *agent_function_table ) {
/* PURPOSE:  Clears the agent function table     */
/* INPUT:    agent_table Table to clear          */
/* OUTPUT:   NONE                                */
/* RETURN:   SUCCESS Indicates success           */
/*           FAILURE Indicates failure           */

  int i,j;

  for ( i = 0; i < agent_function_table->number_agent_lists; i++ ) {

    for ( j = 0; j < agent_function_table->agent_list[i].list_size; j++ ) {

      free( agent_function_table->agent_list[i].list[j] );

    }

    free( agent_function_table->agent_list[i].list );

  }

  free( agent_function_table->agent_list );

  return SUCCESS; 

}

void function_set ( agent_ptr* agent, int state, 
                    char* func_description ) {
/* PURPOSE:  This function sets up a list of functions for a          */
/*           particular state.  All it does is it goes through its    */
/*           list of known action routines and checks to see if that  */
/*           string is contained in the state_description string      */
/* INPUT:    agent            Ptr to the agent to be set              */
/*           state            State functions are to be set for       */
/*           func_description String to parse for functions           */
/* OUTPUT:   agent            Ptr to the set agent                    */
/* RETURN:   NONE                                                     */

  int i = 0, x;
  
  /* Go up to list of action routines */
  for ( ; (*func_description) != ':'; func_description++ );
  func_description++;

  /* Get the total number of functions */
  (*agent)->state_functions[state].number_functions = getnumberfunctions(func_description);
  (*agent)->state_functions[state].function_list = malloc( sizeof( function_struct ) * (*agent)->state_functions[state].number_functions );
  
  /* Loop until final delimeter is found */
  while ( *(func_description) != '#' ) {

    for( x = 0; strncmp( "", function_table[ x ].function_name, strlen( function_table[ x ].function_name ) ); x++ )
      {
	if ( strncmp( func_description, function_table[ x ].function_name, strlen( function_table[ x ].function_name ) ) == 0 )
	  {
	    (*agent)->state_functions[state].function_list[i].function = function_table[ x ].function;
	    grid_set( agent, state, i, strstr( func_description, function_table[ x ].function_name ) );
	  }
      }

    i++;
    
    /* AT THIS POINT ANOTHER FUNCTION FOR CUSTOM ROUTINES */
    
    for ( ; *(func_description) != ';'; func_description++ );
    func_description++;
    
  }
}

void grid_set ( agent_ptr *agent, int state, int function_num, 
                char* grid_list ) {
/* PURPOSE:  Sets up the grids to be changed for a particular         */
/*           function call                                            */
/* INPUT:    agent         Ptr to the agent to be set                 */
/*           state         State function to be set for               */
/*           function_num  Number of function to set                  */
/*           grid_list     String of grids to parse                   */
/* OUTPUT:   agent         Ptr to set agent                           */
/* RETURN:   NONE                                                     */

  int j = 0;
  
  /* Figure out what grid is to update with particular function */
  for ( ; ( (*grid_list) != '-' ) && ( (*grid_list) != ';' ); grid_list++ );
 
  /* No Grids to Update */
  if ( (*grid_list) == ';' ) {
  
    (*agent)->state_functions[state].function_list[function_num].number_grids = 0;
    
  } else {
  
    /* Update past - */
    grid_list++;
    
    /* Get number of grids and allocate */
    (*agent)->state_functions[state].function_list[function_num].number_grids = 
      getnumbergrids( grid_list );

    (*agent)->state_functions[state].function_list[function_num].update_grids = 
      malloc( sizeof( update_grid_struct ) * 
      (*agent)->state_functions[state].function_list[function_num].number_grids );
    
    while ( (*grid_list) != ';' ) {
    
      if ( (*grid_list) == 'S' ) {
      
        /* Set update grid in array to priority id */
        (*agent)->state_functions[state].function_list[function_num].update_grids[j].grid = (*agent)->support_grid_id;
        
      } else if ( (*grid_list) == 'D' ) {
      
        (*agent)->state_functions[state].function_list[function_num].update_grids[j].grid = -1;
        
      } else {
      
        /* Position contains a number and build number */
        (*agent)->state_functions[state].function_list[function_num].update_grids[j].grid = atoi( grid_list );
        
      }
      
      /* Move to ( or , or ; */
      for ( ; ( (*grid_list) != '(' ) && ( (*grid_list) != ';' ) && ( (*grid_list) != ',' ); grid_list++ );
      
      if ( (*grid_list) == '(' ) {
      
        /* Move to character */
        grid_list++;
        
        (*agent)->state_functions[state].function_list[function_num].update_grids[j].grid_value = atof( grid_list );
        
        /* Move to ) character */
        for ( ; ( (*grid_list) != ')' ); grid_list++ );
        grid_list++;
      } 

      if ( (*grid_list) == ',' ) {
      
        grid_list++;
        
      }
      
      j++;
      
    }
    
  }
  
}

int delete_agent ( agent_ptr *agent ) {
/* PURPOSE:  This function deletes the agent from memory */
/* INPUT:    agent  Agent to delete                      */
/* OUTPUT:   NONE                                        */
/* RETURN:   SUCCESS  Indicates success                  */
/*           FAILURE  Indicates failure                  */

  int i,j;

  /* Loop through each function and delete update grids the function */
  for ( i = 0; i < (*agent)->fsa->number_states; i++ ) {

    for ( j = 0; j < (*agent)->state_functions[i].number_functions; j++ ) {

      /* CMH 7-2-99 added to handle functions w/o grids to update */
      if ( (*agent)->state_functions[i].function_list[j].number_grids > 0) {
	free( (*agent)->state_functions[i].function_list[j].update_grids );
      }

    }

    free( (*agent)->state_functions[i].function_list );

  }

  free( (*agent)->state_functions );

  /* Delete the fsa */
  if ( delete_fsa((*agent)->fsa) ) {

    error( "Could not delete fsa\n" );
    return FAILURE;

  }

  free( (*agent) );

  return SUCCESS;

}

void print_agent ( agent_ptr agent_to_print ) {
/* PURPOSE:  This function prints out the agent to screen */
/* INPUT:    agent_to_print  Ptr to agent to print out    */
/* OUTPUT:   NONE                                         */
/* RETURN:   NONE                                         */

  int i;
  
  printf( "\nAgent\n-----\n\n" );
  printf( "Priority ID = %d\n", agent_to_print->priority_id );
  printf( "Support Grid ID = %d\n\n", agent_to_print->support_grid_id );
  printf( "Functions per state:\n\n" );
  
  for ( i = 0; i < agent_to_print->fsa->number_states; i++ ) {
  
    printf( " State %d, Number Functions = %d\n", i, 
            agent_to_print->state_functions[i].number_functions );
    
  }
  
  printf( "\nUser Defined Fields:\n\n" );

  for ( i = 0; i < MAX_FIELDS; i++ ) {

    printf( "User %d: %f\n", i, agent_to_print->field[i] );

  }

  print_function_list( agent_to_print );
  print_fsa( agent_to_print->fsa );
 
}


void print_function_list ( agent_ptr agent_to_print ) {
/* PURPOSE:  This function prints out the function list to screen */
/* INPUT:    agent_to_print  Ptr to agent to print out            */
/* OUTPUT:   NONE                                                 */
/* RETURN:   NONE                                                 */

  int i, j, k, x;
  
  printf( "\n\nFunction List:\n");
  
  /* Loop through all functions assigned to agent */
  for ( i = 0; i < agent_to_print->fsa->number_states; i++ ) {
  
    printf( "\n\nState: %d", i );
    
    /* Loop through every function in function list */
    for ( j = 0; j < agent_to_print->state_functions[i].number_functions; j++ ) {
    
      printf( "\nFunction Number: %d", j );
      printf( "\nFunction: " );
      
    for( x = 0; strncmp( "", function_table[ x ].function_name, strlen( function_table[ x ].function_name ) ); x++ )
      {
	/* Figure out function name */
	if ( agent_to_print->state_functions[i].function_list[j].function == function_table[ x ].function )           
	  printf( function_table[ x ].function_name );
      }
      /* Loop through each grid to print */
      printf( "\nNumber of grids to update: %d", 
            agent_to_print->state_functions[i].function_list[j].number_grids );
      
      for ( k = 0; k < agent_to_print->state_functions[i].function_list[j].number_grids; k++ ) {
      
        printf( "\nGrid to update: %d", 
                agent_to_print->state_functions[i].function_list[j].update_grids[k].grid );
        
        printf( "\nValue to update grid with: %f", 
                agent_to_print->state_functions[i].function_list[j].update_grids[k].grid_value );
        
      }
      
    }
    
  }
  
}

int getnumberfunctions ( char* string ) {
/* PURPOSE:  This function will return the number of action routines */
/*           specified in a particular string                        */
/*           Format of line: a:*;*;*;#                               */
/*           Where * is a function name                              */
/*           number of ; is number of functions                      */
/* INPUT:    string  Ptr to string with action routines              */
/* OUTPUT:   NONE                                                    */
/* RETURN:   Number of functions                                     */

  char *tracer = string;
  
  int count = 0;
  
  while ( (*tracer) != '#' ) {
  
    if ( (*tracer) == ';' ) count++;
    
    tracer++;
    
  }
  
  return count;
  
}

int getnumbergrids ( char* string ) {
/* PURPOSE:  This function will return the number of grids           */
/*           specified in a particular string                        */
/*           Format of line: *,*,*;                                  */
/*           Where * is a grid                                       */
/*           number of ,'s + 1 is number of functions                */
/* INPUT:    string  Ptr to string with action routines              */
/* OUTPUT:   NONE                                                    */
/* RETURN:   Number of grids                                         */

  char *tracer = string;
  
  int count = 1;
  
  while ( (*tracer) != ';' ) {
  
    if ( (*tracer) == ',' ) count++;
    
    tracer++;
    
  }
  
  return count;
  
}

int copy_agent( agent_ptr source, agent_ptr *destination ) {
/* PURPOSE:  This will copy an agent pointed to by source to the */
/*           destination ptr                                     */
/* INPUT:    source       Agent to be copied                     */
/* OUTPUT:   destination  Destination of agent                   */
/* RETURN:   SUCCESS      Copy succeeded                         */
/*           FAILURE      Copy failed                            */
 
  int i,j;

  /* Copy all non pointer fields */
  (*destination)->x_pos = source->x_pos;
  (*destination)->y_pos = source->y_pos;
  (*destination)->id_number = source->id_number;
  (*destination)->dir = source->dir;
  (*destination)->priority_id = source->priority_id;
  (*destination)->support_grid_id = source->support_grid_id;

  (*destination)->active = source->active;
  
  /* GOING TO NEED FIX HERE, CALL TO USER DEFINED ROUTINE? */
  (*destination)->chem = source->chem;
  
  /* Allocate space for state functions, as large as number of states */
  (*destination)->state_functions = malloc( sizeof( state_functions_struct ) * 
                                         source->fsa->number_states );
  
  for( i = 0; i < source->fsa->number_states; i++ ) {
  
    (*destination)->state_functions[i].number_functions = source->state_functions[i].number_functions;
    (*destination)->state_functions[i].function_list = malloc( sizeof( function_struct ) * 
                                                            source->state_functions[i].number_functions );    

    for ( j = 0; j < source->state_functions[i].number_functions; j++ ) {
   
      if( copy_function( &(source->state_functions[i].function_list[j]),
                          &((*destination)->state_functions[i].function_list[j]) ) ) {
        error( "Cannot copy function\n" );
        return FAILURE;
                     
      }
      
    }
    
  }
  
  /* Allocate space for fsa and copy it */
  (*destination)->fsa = malloc( sizeof( fsa_struct ) );
  
  if ( (*destination)->fsa == NULL ) {
  
    error( "Out of memory\n" );
    return FAILURE;
    
  }
  
  if ( copy_fsa( source->fsa, (*destination)->fsa ) ) {
  
    error( "Cannot copy fsa\n" );
    return FAILURE;
    
  }

  /** M.A.K.   06-12-2001 **/
  for( i = 0; i < MAX_FIELDS; i++ )
    {
      (*destination)->field[i] = source->field[i];
    }
  
  return SUCCESS;
  
}

int copy_function( function_struct *source, function_struct *destination ) {
/* PURPOSE:  Copy function_struct from source to destination */
/*           Space was already allocated just copy struct    */
/* INPUT:    source      Source function_struct              */
/* OUTPUT:   destination Destination function_struct         */
/* RETURN:   SUCCESS     Indicates success                   */
/*           FAILURE     Indicates failure                   */

  int i;

  /* Copy function over */
  destination->function     = source->function;
  destination->number_grids = source->number_grids;
  
  /* Allocate space for grids */
  destination->update_grids = malloc( sizeof( update_grid_struct ) * destination->number_grids );
  
  if ( destination->update_grids == NULL ) {
  
    error( "Out of memory\n" );
    return FAILURE;
    
  }
  
  /* Copy each grid struct */
  for ( i = 0; i < destination->number_grids; i++ ) {
  
    if ( copy_grid( &(source->update_grids[i]), 
                      &(destination->update_grids[i]) ) ) {
    
      error( "Cannot copy grid\n" );
      return FAILURE;
      
    }
    
  }
  
  return SUCCESS;
  
}

int copy_grid( update_grid_struct *source, update_grid_struct *destination ) {
/* PURPOSE:  Copy a grid struct from source to destination */
/*           Space already allocated for destination       */
/* INPUT:    source      Grid struct to copy from          */
/* OUTPUT:   destination Grid struct to copy to            */
/* RETURN:   SUCCESS     Indicates success                 */
/*           FAILURE     Indicates failure                 */

  destination->grid       = source->grid;
  destination->grid_value = source->grid_value;
  
  return SUCCESS;
  
}
