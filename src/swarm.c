#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "trace.h"
#include "swarm.h"
#include "errors.h"
#include "agent.h"
#include "probe.h"
#include "constants.h"
#include "support_grids.h"
#include "useful.h"

/*------------------------ Externs Needed --------------------------- */

extern char                 *SWARM_FILE_NAME;
extern int                  RANDOM_NUMBER;
extern s_grids_ptr          all_support_grids;
extern pri_func_ptr         priority_grid[];
extern swarm_element_struct *swarm;
extern swarm_element_struct *master_swarm_types;

/*------------------------ Local Constants ---------------------------*/

/*------------------------- Local Macros -----------------------------*/

/*-------------------------- Local Types -----------------------------*/

/*---------------------- Local Function Prototypes -------------------*/
/* void sample (char *, int, Real);                                   */

int addntimes( agent_ptr, int );
int place_agents( swarm_element_struct*, int, char*, fgrid_ptr );
void random_position( agent_struct *agent );
void strip_white( char **str );

/*---------------------- Function Definitions ------------------------*/

int initialize_swarm ( fgrid_ptr agent_grid, char* agentFunctionFileName) 
{
  /* Swarm file */
  FILE  *swarm_file;
  char  buffer[MAX_BUFFER], *current_char;
  
  /* Current agent information */
  int current_type, current_priority, current_support_grid, current_size;
      
  /* Pointers to agents in swarm list */
  swarm_element_struct *first_agent;
      
  /* FSA and agent function tables */
  agent_table_struct          agent_table;
  fsa_table_struct            fsa_table;
  agent_function_table_struct agent_function_table;

  /* Agent */
  agent_ptr temp_agent = NULL;

  /* Get the FSA table */  
  if ( initialize_fsa_table(&fsa_table) ) {
  
    error("FSA Table not initialized.\n");
    return FAILURE;
  
  }

  FILE *agent_function_file = fopen( agentFunctionFileName, "r");

  if ( agent_function_file == NULL ) 
  {
    error( "Can't open agent function file: ");
    error( agentFunctionFileName );
    error( "\n" );
    return FAILURE;
  }

  /* Get the agent function table */
  if ( initialize_agent_function_table(&agent_function_table, agent_function_file) )
  {
    error( "Agent Function Table not initialized.\n" );
    return FAILURE;
  }

  fclose(agent_function_file);

  /* Get the agent table */
  if ( initialize_agent_table( &agent_table ) ) {

    error( "Agent Table not initialized.\n" );
    return FAILURE;

  }

  /* If agent table and FSA table do not have same number of elements */
  /* indicate error.                                                  */
  if ( fsa_table.number_fsa != agent_function_table.number_agent_lists ) {
  
    error( "Mismatch between number of fsa and number of agent function lists.\n" );
    return FAILURE;
    
  }

  /* Also check against agent table */
  if ( fsa_table.number_fsa != agent_table.number_field_lists ) {

    error( "Mismatch between number of agent field lists and number of agent types\n" );
    return FAILURE;

  }

  srand( RANDOM_NUMBER );
  
  /* Open the swarm file */
  swarm_file = fopen(SWARM_FILE_NAME, "r");
  if ( swarm_file == NULL ) {  
    error("Can't open file ");
    error(SWARM_FILE_NAME);
    error("\n");
    return FAILURE;
  }
  
  /* Skip past the comments */
  fgets( buffer, MAX_BUFFER, swarm_file );
  while ( buffer[0] == '#' ) fgets( buffer, MAX_BUFFER, swarm_file );
  
  /* Loop until ## encountered */
  while ( ( buffer[0] != '#' ) && ( buffer[1] != '#' ) ) {
  
    current_char = &(buffer[0]);
    
    /* Get amount of current type */
    for (; (*current_char < 48) || (*current_char > 57); current_char++);
    current_size = atoi( current_char );
    
    /* Get type of FSA to use */
    /* Find position of next number */
    
    for ( ; (*current_char >= 48) && (*current_char <= 57); current_char++ );
    for ( ; (*current_char < 48) || (*current_char > 57); current_char++ );
    
    current_type = atoi( current_char );
    current_type--;
    
    /* Check against one of the ranges in either table */
    if ( current_type > fsa_table.number_fsa ) {
    
      error( "Agent type not defined\n" );
      return FAILURE;
      
    }
    
    /* Get Priority ID for agent */
    for ( ; (*current_char >= 48) && (*current_char <= 57); current_char++ );
    for ( ; (*current_char < 48) || (*current_char > 57); current_char++ );
    
    current_priority = atoi( current_char );
    
    /* Get Support Grid ID for agent */
    for ( ; (*current_char >= 48) && (*current_char <= 57); current_char++ );
    for ( ; (*current_char < 48) || (*current_char > 57); current_char++ );
    
    current_support_grid = atoi( current_char );    

    /* Initialize agent */
    if ( initialize_agent( &(temp_agent), 
                           agent_function_table.agent_list[current_type],
                           agent_table.agent_field_list[current_type],
                           &(fsa_table.fsa[current_type]), 
                           current_priority, current_support_grid ) ) {
                            

      error( "Agent not initialized\n");
      return FAILURE;
      
    }

	if (IsTraceLevelVerbose())
	{
		TraceVerboseLine("Made agents\n");
		print_agent(temp_agent);
	}

    /* Trace up to last agent */
    first_agent = swarm;
    while ( first_agent->next_agt != NULL ) {

      first_agent = first_agent->next_agt; 

    }

    /* Add agent to swarm proper number of times */
    if ( addntimes( temp_agent, current_size ) ) {
    
      error( "Agents cannot be added to swarm\n" );
      return FAILURE;
      
    }

    /* Moves to first agent of next type, as long as size != 0 */
    //first_agent = first_agent->next_agt;

# if 0
    
    /* Add agent to master swarm index of types */
    if ( addntimes( temp_agent, 1, &master_swarm_types ) ) {
    
      error( "Agents cannot be added to master swarm types\n" );
      return FAILURE;
      
    }

# endif

    /* Get rid of temp_agent */
    delete_agent( &(temp_agent) );

    /* Move to first character of placement */
    for ( ; (*current_char >= 48) && (*current_char <= 57); current_char++ );
    for ( ; (*current_char == 9); current_char++ );

    if ( place_agents( first_agent, current_size, current_char, agent_grid ) ) {
 
      error ( "Cannot place agents\n" );
      return FAILURE;

    }
      
    /* Eat line */
    fgets( buffer, MAX_BUFFER, swarm_file );

  }
  fclose( swarm_file );
  
  return SUCCESS;
  
}

/* strips excess white spaces from a string */
/* it stops when a non-whitespace character is found */
void strip_white( char **str )
  {
    while( (**str) != '\0' && isspace( **str ) != 0 )
      {
	(*str)++;
      }
  }

/* give an agent a random position */
void random_position( agent_struct *agent )
{
  double randnum;

  randnum = ( double ) rand( ) / ( double ) RAND_MAX;
  agent->x_pos = ( int ) ( GGETC( 0 ) * randnum );
  
  randnum = ( double ) rand( ) / ( double ) RAND_MAX;
  agent->y_pos = ( int ) ( GGETR( 0 ) * randnum );

  SETAGT(agent->y_pos, agent->x_pos);
}


/* places agents in a line */
void linear_pos(agent_struct *agent, int startrow, int startcol, int dir){

  static int offset = 0;  
  int row, column;

  switch( dir){
  case 0: row = startrow - offset; break;
  case 1: column = startcol + offset; break;
  case 2: row = startrow + offset; break;
  case 3: column = startcol - offset; break;
  } 

  agent->x_pos = column;

  agent->y_pos = row;

  //  printf("Placing agent at %d %d\n", agent->y_pos, agent->x_pos);
  SETAGT(agent->y_pos, agent->x_pos);

  offset++;

}

/* give an agent a random position within an area */
void limited_random_pos( agent_struct *agent, int minrow, int mincol, 
			                      int maxrow, int maxcol)
{
  double randnum;
  int row_range  = maxrow - minrow + 1;
  int col_range  = maxcol - mincol + 1;

  randnum = ( double ) rand( ) / ( double ) RAND_MAX;
  agent->x_pos = ( int ) ( col_range * randnum ) + mincol;
  
  randnum = ( double ) rand( ) / ( double ) RAND_MAX;
  agent->y_pos = ( int ) ( row_range * randnum ) + minrow;

  //  printf("Placing agent at %d %d\n", agent->y_pos, agent->x_pos);
  SETAGT(agent->y_pos, agent->x_pos);
}


int place_agents ( swarm_element_struct *first_agent, 
                   int                  size,
                   char                 *place_string, 
		   fgrid_ptr            agent_grid ) {
/* PURPOSE: Places the agents based on contents of place_string  */
/* INPUT:   first_agent  First agent of current type             */
/*          size         Number of agents for current type       */
/*          place_string Indicates how to place agents and where */
/* OUTPUT:  swarm        Updated with properly placed agents     */
/* RETURN:  SUCCESS      Indicates success                       */
/*          FAILURE      Indicates failure                       */

  swarm_element_struct *trace = first_agent;
  int x_center, y_center, x_size, y_size, swarm_length,
      swarm_half_length, x_coord, y_coord, x_init, y_init, mincol, minrow, 
      maxcol, maxrow, dir;

  /* used by file initialization */
  FILE* fp;
  int x, test;
  char errorbuf[ 100 ], *filename, c;

  //  printf("Placing agents\n");
  //  printf("Method used is %s\n", place_string);


  /* Check for Random */
  if ( strstr( place_string, "Random" ) ) {
   
    while ( trace != NULL ) 
      {
	if ((trace->agent)->x_pos == -1 && (trace->agent)->y_pos == -1){
	   random_position( trace->agent );
	}

	trace = trace->next_agt;
      }

  
  /* Check for Random Region */
  } else if ( strstr( place_string, "RandRegion(" ) ) {

    for( ; *(place_string) != '('; place_string++ );
    place_string++;

    minrow = atoi( place_string );

    for( ; *(place_string) != ','; place_string++ );
    place_string++;

    mincol = atoi( place_string );

    for( ; *(place_string) != ','; place_string++ );
    place_string++;

    maxrow = atoi( place_string );

    for( ; *(place_string) != ','; place_string++ );
    place_string++;

    maxcol = atoi( place_string );

    while ( trace != NULL ) 
      {
	if ((trace->agent)->x_pos == -1 && (trace->agent)->y_pos == -1){
	   limited_random_pos( trace->agent, minrow, mincol, maxrow, maxcol );
	}

	trace = trace->next_agt;
      }
  
  /* Check for S(X,Y) */
  } else if ( strstr( place_string, "S(" ) ) {

    place_string++;
    place_string++;

    /* added to correctly position the beginning of the string -DWP 10/23/99 */
    for( ; *(place_string) != ','; place_string++ );
    for( ; *(place_string) != '('; place_string-- );
    place_string++;

    x_center = atoi( place_string );

    for( ; *(place_string) != ','; place_string++ );
    place_string++;
    y_center = atoi( place_string );

    x_size = GGETC( 0 );
    y_size = GGETR( 0 );

    swarm_length = ceil( sqrt( size ) );

    swarm_half_length = ceil( swarm_length / 2 );

    if ( ( ( x_center - swarm_half_length ) < 0 ) && 
         ( ( x_center + swarm_half_length ) > x_size ) ) {

      return FAILURE;
   
    }
  
    if ( ( ( y_center - swarm_half_length ) < 0 ) && 
         ( ( y_center + swarm_half_length ) > y_size ) ) {

      return FAILURE;

    }

    x_coord = x_center - swarm_half_length;
    y_coord = y_center - swarm_half_length;

    x_init = x_coord;
    y_init = y_coord;

    while ( trace != NULL ) {
      
	if ((trace->agent)->x_pos != -1 && (trace->agent)->y_pos != -1){
	   trace = trace->next_agt;
	   continue;
	}

      trace->agent->x_pos = x_coord++;
      trace->agent->y_pos = y_coord;
      SETAGT(trace->agent->y_pos, trace->agent->x_pos);

      if ( ( x_coord - x_init ) > swarm_length ) {

        x_coord = x_init;
        y_coord++;

      }

      if ( y_coord > ( y_center + swarm_half_length ) ) {

        return FAILURE;

      }

      trace = trace->next_agt;

    }

  /* Check for M(X,Y) */
  } else if ( strstr( place_string, "M(" ) ) {

    place_string++;
    place_string++;
    x_center = atoi( place_string );
   
    for( ; *(place_string) != ','; place_string++ );
    place_string++;
    y_center = atoi( place_string );

    x_size = GGETC( 0 );
    y_size = GGETR( 0 );

    swarm_length = ceil( sqrt( size ) );
   
    swarm_half_length = ceil( swarm_length / 2 );

    if ( ( ( x_center - swarm_half_length ) < 0 ) && 
         ( ( x_center + swarm_half_length ) > x_size ) ) {

      return FAILURE;
   
    }
  
    if ( ( ( y_center - swarm_half_length ) < 0 ) && 
         ( ( y_center + swarm_half_length ) > y_size ) ) {

      return FAILURE;

    }

    x_coord = x_center - swarm_half_length;
    y_coord = y_center - swarm_half_length;

    x_init = x_coord;
    y_init = y_coord;

    while ( trace != NULL ) {

      if ((trace->agent)->x_pos != -1 && (trace->agent)->y_pos != -1){
	 trace = trace->next_agt;
	 continue;
      }

      trace->agent->x_pos = x_coord++;
      trace->agent->y_pos = y_coord;
      SETAGT(trace->agent->y_pos, trace->agent->x_pos);

      if ( ( x_coord - x_init ) > swarm_length ) {

        x_coord = x_init;
        y_coord++;

      }

      if ( y_coord > ( y_center + swarm_half_length ) ) {

        x_coord = x_init;
        y_coord = y_init;

      }

    trace = trace->next_agt;

    }

    /* file initialization of agent positions */
  } else if ( strstr( place_string, "F(" ) ) {
    /* move past "F(" */
    place_string++;
    place_string++;

    strip_white( &place_string );

    /*
      strsep() is like strtok().
       It returns a null-terminated string
       which is the name of the initialization file
    */

    filename = strsep( &place_string, " )" );
    fp = fopen( filename, "r" );
    
    if ( fp == NULL )
      {
	sprintf( errorbuf, "Agent placement file %s not found.\n", filename );
	error( errorbuf);
	sprintf( errorbuf, "Please check %s.\n", SWARM_FILE_NAME );
	error( errorbuf );
	exit(1);
      } 

    for( x = 0; x < size && trace != NULL; x++, trace = trace->next_agt )
      {
	test = fscanf( fp, "%d%d", &trace->agent->x_pos, &trace->agent->y_pos );

	SETAGT( trace->agent->y_pos, trace->agent->x_pos );

	if ( test == EOF )
	  {
	    sprintf( errorbuf, "EOF reached in %s.\n", filename );
	    error( errorbuf  );
	    sprintf( errorbuf, "Random positions are now being assigned for agents %d to %d.\n", x, size );
	    error( errorbuf );
	  
	    for( ; x < size && trace != NULL; x++, trace = trace->next_agt )
	      {
		random_position( trace->agent );
	      }

	    /* break from main loop */
	    break;
	  }

	/* tests to see if both integer positions were read in */
	if ( test != 2 )
	  {
	    sprintf( errorbuf, "Error in %s at line %d, position %ld.\n", filename, x+1, ftell( fp ) );
	    error( errorbuf );
	    exit( 1 );
	  }
	
	/* make sure both integers were in range */
	if ( trace->agent->x_pos >= GGETC( 0 ) || trace->agent->y_pos >= GGETR( 0 ) ||
	     trace->agent->x_pos < 0           || trace->agent->y_pos < 0 )
	  {
	    
	    sprintf( errorbuf, "In file %s, agent on line# %d has an invalid position.\n", filename, x+1 );
	    
	    error( errorbuf );
	    error( "Assigning a random position for the agent.\n" );
	    
	    random_position( trace->agent );

	  }

	/* allows for comments after each line */

	while( ( c = fgetc( fp ) ) != EOF )
	  {
	    if ( c == '\n' )
	      {
		/* break from while loop */
		break;
	      }

	    if ( c == '#' )
	      {
		/* ignore the rest of the line */
		while( ( c = fgetc( fp ) ) != '\n' && c != EOF );

		/* break from while loop */		
		break;
	      }

	    if ( isspace( c ) == 0 )
	      {
		sprintf( errorbuf, "Error in %s at line %d, position %ld.\n", filename, x+1, ftell( fp ) );
		error( errorbuf );
		exit( 1 );
	      }
	  }
       
      }

  } else if ( strstr( place_string, "L(" ) ) {

    for( ; *(place_string) != '('; place_string++ );
    place_string++;

    minrow = atoi( place_string );

    for( ; *(place_string) != ','; place_string++ );
    place_string++;

    mincol = atoi( place_string );

    for( ; *(place_string) != ','; place_string++ );
    place_string++;

    dir = atoi( place_string );

    while ( trace != NULL ) 
      {
	if ((trace->agent)->x_pos == -1 && (trace->agent)->y_pos == -1){
	   linear_pos( trace->agent, minrow, mincol, dir); 
	}

	trace = trace->next_agt;
      }

  } 



  return SUCCESS;

}


void update_swarm ( ) {
/* PURPOSE: This function gets an agents particular view of world,    */
/*          transitions the agent's fsa, and calls the agent's        */
/*          functions associated with the particular state            */
/* INPUT:   swarm     Ptr to first agent in linked list of agents     */
/* OUTPUT:  NONE                                                      */
/* RETURN:  NONE                                                      */

  swarm_element_struct *tracer = swarm;
  
  int current_function = 0;
  
  char agent_view;

  /* Loop until tracer points to NULL */
  while ( tracer != NULL ) {
    if ( tracer->agent->active ) {

      /* THIS IS WHERE AGENT GETS ITS PARTICULAR VIEW OF THE WORLD */
      agent_view = priority_grid[tracer->agent->priority_id]
                    (all_support_grids, tracer->agent->y_pos, 
                     tracer->agent->x_pos);
      
      change_state(tracer->agent->fsa, agent_view);

      current_function = 0;

      while ( current_function < tracer->agent->state_functions[tracer->agent->fsa->current_state].number_functions ) {

	if( (*tracer->agent->state_functions[tracer->agent->fsa->current_state].function_list[current_function].function) == NULL )
	  {
	    printf("func #: %d     state: %d\n",current_function,tracer->agent->fsa->current_state);
	    print_function_list( tracer->agent ); 
	    error("\nFunction in agent action list not found");
	    exit(-1);
	  }

        (*tracer->agent->state_functions[tracer->agent->fsa->current_state].function_list[current_function].function)
          (tracer->agent,current_function);
        current_function++; 

      }

      probe_agents(tracer->agent);

    }

    tracer = tracer->next_agt;

  }

}

int remove_element( swarm_element_struct *swarm_element ) {
/* PURPOSE: Remove an element from the list and fix up the pointers */
/* INPUT:   swarm_element  Element to remove                        */
/* OUTPUT:  NONE                                                    */
/* RETURN:  SUCCESS Indicates success                               */
/*          FAILURE Indicates failure                               */

  /* If only agent in list, delete agent, make pointer null */
  if ( ( swarm_element->prev_agt == NULL ) && 
       ( swarm_element->next_agt == NULL ) ) {

    delete_agent( &(swarm_element->agent) );
    free( swarm_element );

  } else {

    delete_agent( &(swarm_element->agent) );
    swarm_element->next_agt->prev_agt = swarm_element->prev_agt;
    swarm_element->prev_agt->next_agt = swarm_element->next_agt;
    free( swarm_element );

  }

  return SUCCESS;

}

int addntimes( agent_ptr agent, int n  ) {
/* PURPOSE:  This function will add agent to the end of the swarm list */
/*           n times                                                   */
/* INPUT:    agent     Ptr to agent to add                             */
/*           n         Number of times to add agent                    */
/*           swarm     Swarm to add to                                 */
/* OUTPUT:   swarm     Update swarm                                    */
/* RETURN:   SUCCESS   Indicates success of addition                   */
/*           FAILURE   Indicates failre of addition                    */

  int i;
  static int id_count = 1;

  swarm_element_struct *trace_agent = swarm;

  /* If empty, add one and then recursively call self to add n-1 more */
  if ( swarm->agent == NULL ) {
  
//    /* Malloc space for first element */
//    trace_agent = malloc( sizeof( swarm_element_struct ) );
  
    if ( trace_agent == NULL ) {
    
      error( "Out of memory, cannot add to swarm\n" );
      return FAILURE;
      
    }

    /* Copy agent */    
    trace_agent->agent = malloc( sizeof( agent_struct ) );
  
    if ( trace_agent->agent == NULL ) {
    
      error( "Out of memory, cannot add to swarm\n" );
      return FAILURE;
      
    }
       
    copy_agent( agent, &(trace_agent->agent) );
    trace_agent->agent->id_number = id_count++;

    /* Fix up pointers */
    trace_agent->prev_agt = NULL;
    trace_agent->next_agt = NULL;
  
    /* Recursive call */
    //    addntimes( agent, n-1, list );
    n -= 1;

  }

  /* Trace through and find end of list */  
  while ( (trace_agent->next_agt) != NULL ) trace_agent = trace_agent->next_agt;

  for ( i = 0; i < n; i++ ) {
      
    /* Malloc space for next element in list */
    trace_agent->next_agt = malloc( sizeof( swarm_element_struct ) );

    if ( trace_agent->next_agt == NULL ) {
    
      error( "Out of memory, cannot add to swarm\n" );
      return FAILURE;
      
    }
      
    /* Copy agent */
    trace_agent->next_agt->agent = malloc( sizeof( agent_struct ) );
      
    if ( trace_agent->next_agt->agent == NULL ) {
    
      error( "Out of memory, cannot add to swarm\n" );
      return FAILURE;
      
    }
               
    copy_agent( agent, &(trace_agent->next_agt->agent) );
    (trace_agent->next_agt)->agent->id_number = id_count++;

    /* Fix previous, Null next */
    (trace_agent->next_agt)->prev_agt = trace_agent;
    (trace_agent->next_agt)->next_agt = NULL;
      
    /* Update trace to next */
    trace_agent = trace_agent->next_agt;
      
  }
  
  return SUCCESS;

}

