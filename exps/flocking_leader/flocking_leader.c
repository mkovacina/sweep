/* gives access to error() */
#include "errors.h"

/* give access to print_*_grid() */
#include "print_grid.h"

#define UP     1
#define DOWN   2
#define RIGHT  3
#define LEFT   4

#define MAX_STUCK_COUNT 4

#define PROXIMITY_LEVEL 4
#define BUFFER_AREA 3


void AF_put( int action_grid, int row, int col, int value );


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
	    move_left( agent, 0 ); 
	    return;
	  }
	}
	count++;
    }
  }
}

void AF_safe_follow(agent_struct *agent, int action_index)
{
  char *function_name = "AF_safe_follow";
  int  num_grids      = 1;

  int proximity;
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

  proximity = GGET( action_grid, row, col );

  /* copy the direction to the new grid for next agent or for self if waiting */
  GSET( action_grid, row, col, proximity );

  switch( proximity )
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
       printf("ERROR# 983\n");
       abort();
       break;
  }

  if ( row == agent->y_pos && col == agent->x_pos )
    {
      agent->field[0] = agent->field[0]+1;
    }
  else
    {
      agent->field[0] = 0;
    }

  if ( agent->field[0] > MAX_STUCK_COUNT )
    {
      AF_safe_wander( agent, action_index );
    }
}

void AF_set_proximity(agent_struct *agent, int action_index)
{
  char *function_name = "AF_set_proximity";
  int  num_grids      = 1;

  int proximity;
  int row, col, state;
  int r, c, radius;

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

  proximity = PROXIMITY_LEVEL;

  /* vain attempt for circularity */

  for( radius = PROXIMITY_LEVEL+BUFFER_AREA; radius > BUFFER_AREA; radius-- )
    {
      /* TOP: moves left to right */
      for( c = -radius; c < radius; c++ )
	{
	  AF_put( action_grid, row-radius, col+c, RIGHT );
	}

      /* BOTTOM: moves left to right */
      for( c = -radius+1; c < radius+1; c++ )
	{
	  AF_put( action_grid, row+radius, col+c, LEFT );
	}

      /* RIGHT: moves up to down */
      for( r = -radius; r < radius; r++ )
	{
	  AF_put( action_grid, row+r, col+radius, DOWN );
	}

      /* LEFT: moves up to down */
      for( r = -radius+1; r < radius+1; r++ )
	{
	  AF_put( action_grid, row+r, col-radius, UP );
	}

      if ( GGET( action_grid, row-radius+1, col-radius ) == 0 )
	{
	  AF_put( action_grid, row-radius+1, col-radius, RIGHT );
	}
    }
}


void AF_put( int action_grid, int row, int col, int value )
{
  if ( 0 <= row && row < GGETR( action_grid ) )
    {
      if ( 0 <= col && col < GGETC( action_grid ) )
	{
	  GSET( action_grid, row, col, value );
	}
    }
}
