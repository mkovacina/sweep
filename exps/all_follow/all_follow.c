/* gives access to error() */
#include "errors.h"

/* give access to print_*_grid() */
#include "print_grid.h"

#define UP    -1
#define DOWN   1
#define RIGHT  2
#define LEFT  -2

#define MAX_STUCK_COUNT 7

void AF_safe_wander( agent_struct *agent, int action_index )
/*  "safe" wandering, will not allow multiple agents to occupy same location */
{
  char *function_name = "AF_safe_wander";
  int  num_grids      = 1;

  int dir, count, test_val; /* randomly selected direction */
  int row, col, rows, cols, state;

  function_struct    temp_func;
  update_grid_struct temp_grid;
  int                action_grid;

#ifdef AF_error
  puts( function_name );
#endif

  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != num_grids )
  {
    printf( "Insufficient number of grids supplied to %s().\n", function_name );
    printf( "%d grid is required for %s()\n", num_grids, function_name );
    exit( 1 );
  }

  temp_grid = temp_func.update_grids[0];

  /* determines which support grid contains pertinent information */
  action_grid = temp_grid.grid;
 
  rows = GGETR(0) - 1;
  cols = GGETC(0) - 1;

  dir = (int)(100 * ((double)rand()/(double)RAND_MAX));
  count = 0;
  
  switch( dir%4 )
  { 
    while( count < 4 )
    { 
      /* move up */
      case 0:  
	if ( count < 4 && row > 0 ) 
	{ 
	  test_val = GETAGT( row - 1, col );
	  if ( !test_val )
	  {
	    GSET( action_grid, row, col, UP );
	    move_up( agent, 0 ); 
	    return;
	  }
	}
	count++;
	  
      /* move right */
      case 1:  
	if ( count < 4 && col < cols ) 
	{
	  test_val = GETAGT( row, col + 1 );
	  if ( !test_val )
	  {
	    GSET( action_grid, row, col, RIGHT );
	    move_right( agent, 0 ); 
	    return;
	  }
	}
	count++;

      /* move down */
      case 2:  
	if ( count < 4 && row < rows ) 
	{
	  test_val = GETAGT( row + 1, col );
	  if ( !test_val )
	  {
	    GSET( action_grid, row, col, DOWN );
	    move_down( agent, 0 ); 
	    return;
	  }
	}
	count++;
      
      /* move left */  
      case 3:  
	if ( count < 4 && col > 0 ) 
	{
	  test_val = GETAGT( row, col - 1 );
	  if ( !test_val )
	  {
	    GSET( action_grid, row, col, LEFT );
	    move_left( agent, 0 ); 
	    return;
	  }
	}
	count++;
    }
  }

  /* count if stuck */
  if ( count >= 4 )
  {
    /* not ++ b/c field is of type float */
    agent->field[ 0 ] = agent->field[ 0 ] + 1;
  }
  else
  {
    agent->field[ 0 ] = 0;
  }

  if ( agent->field[ 0 ] > MAX_STUCK_COUNT )
  {
    GSET( action_grid, row, col, 0 );
  }
}


void AF_safe_follow(agent_struct *agent, int action_index)
{
  char *function_name = "AF_safe_follow";
  int  num_grids      = 1;

  int dir;
  int row, col, state;

  function_struct    temp_func;
  update_grid_struct temp_grid;
  int                action_grid;

#ifdef AF_error
  puts( function_name );
#endif

  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != num_grids )
  {
    printf( "Insufficient number of grids supplied to %s().\n", function_name );
    printf( "%d grid is required for %s()\n", num_grids, function_name );
    exit( 1 );
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  action_grid = temp_grid.grid;

  dir = GGET( action_grid, row, col );

  /* copy the direction to the new grid for next agent or for self if waiting */
  GSET( action_grid, row, col, dir );

  switch( dir )
  {
    case UP:      
      if( !GETAGT( row-1, col ) )
      {
	move_up( agent, 0 );
      }
      break;

     case RIGHT:
       if( !GETAGT( row, col+1 ) )
       {
	 move_right( agent, 0 );
       }  
       break;

     case DOWN:      
       if( !GETAGT( row+1, col ) )
       {
	 move_down( agent, 0 );
       }
       break;

     case LEFT:      
       if( !GETAGT( row, col-1 ) )
       {
	 move_left(agent,0);
       }
       break;

     default:     
       printf("Invalid direction found -- program aborting\n");
       abort();
       break;
  }

  /* count if stuck */
  if ( row == agent->y_pos && col == agent->x_pos )
  {
    /* not ++ b/c field is of type float */
    agent->field[ 0 ] = agent->field[ 0 ] + 1;
  }
  else
  {
    agent->field[ 0 ] = 0;
  }

  if ( agent->field[ 0 ] > MAX_STUCK_COUNT )
  {
    GSET( action_grid, row, col, 0 );
  }
}
