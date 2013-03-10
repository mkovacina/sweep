/* gives access to error() */
#include "errors.h"

/* give access to print_*_grid() */
#include "print_grid.h"

#define _KILL_AMBIGUITY

#define UP    -1
#define DOWN   1
#define RIGHT  1
#define LEFT  -1

#define INITIAL_FACTOR 2


/*
  Two grids are supplied to wander_info().
  
  grid #0: hortizontal neighbor information
  grid #1: vertical    neighbor information
*/
void split_wander_info( agent_struct *agent, int action_index )
{
  int                row, col, state;
  function_struct    temp_func;
  update_grid_struct temp_grid;

#ifdef split_vortex_error
  printf("\nsplit_wander_info");
#endif

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 2 )
    {
      error( "Insufficient number of grids supplied to split_wander_info().\n" );
      error( "Two grids are required for split_wander_info()\n" );
      exit(1);
    }

  /* if there is no neighbor info, then put down neighbor info */
  if ( GGET( temp_func.update_grids[0].grid, row, col ) == 0 &&
       GGET( temp_func.update_grids[1].grid, row, col ) == 0    )
    {
      /* do not user cummulative additions to the grid */

      /* horizontal neighbor info */
      temp_grid = temp_func.update_grids[0];      
      GSET(temp_grid.grid, row, col+1, RIGHT * INITIAL_FACTOR );
      GSET(temp_grid.grid, row, col-1, LEFT  * INITIAL_FACTOR );

      /* vertical neighbor info */
      temp_grid = temp_func.update_grids[1];
      GSET(temp_grid.grid, row+1, col, DOWN * INITIAL_FACTOR );
      GSET(temp_grid.grid, row-1, col, UP   * INITIAL_FACTOR );
    }
}               


/*
  One grid is required by split_info()

  grid #0: neighbor information grid
*/
void split_info( agent_struct *agent, int action_index )
{
  int                row, col, state;
  function_struct    temp_func;
  update_grid_struct temp_grid;

#ifdef vortex_error
  printf("\nsplit_info");
#endif

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
    {
      error( "Insufficient number of grids supplied to h_split_info().\n" );
      error( "One grid is required for h_split_info()\n" );
      exit(1);
    }

  temp_grid = temp_func.update_grids[ 0 ];

  switch( (int)(temp_func.update_grids[0].grid_value) )
  {
  case 1:
    GSET( temp_grid.grid, row, col+1, RIGHT );
    break;
  case 2:
    GSET( temp_grid.grid, row, col-1, LEFT );
    break;
  case 3:
    GSET( temp_grid.grid, row-1, col, UP );
    break;
  case 4:
    GSET( temp_grid.grid, row+1, col, DOWN );
    break;
  case 5:
    GSET( temp_grid.grid, row, col+1, RIGHT );
    GSET( temp_grid.grid, row, col-1, LEFT  );
    break;
  case 6:
    GSET( temp_grid.grid, row-1, col, UP   );
    GSET( temp_grid.grid, row+1, col, DOWN );
    break;
  default:
    error( "Parameter error in split_vortex_info()\n" );
    printf( "param: %f\n",temp_func.update_grids[1].grid_value );
    exit(1);
    break;
  }
}

#define ORTHOGONAL_FACTOR 2
/*
  Two grids are required by split_vortex_info()

  grid #0: horizontal vortex info
  grid #1: vertical   vortex info
*/
void split_vortex_info( agent_struct *agent, int action_index )
{
  int                row, col, state;
  function_struct    temp_func;

#ifdef vortex_error
  printf("\nsplit_vortex_info");
#endif

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 2 )
    {
      error( "Insufficient number of grids supplied to split_vortex_info().\n" );
      error( "Two grids are required for split_vortex_info()\n" );
      exit(1);
    }


  /* check the parameter to determine which info orientation should be used */
  if( temp_func.update_grids[0].grid_value == 1 )
  {
    GSET( temp_func.update_grids[0].grid, row-1, col, RIGHT * ORTHOGONAL_FACTOR );
    GSET( temp_func.update_grids[0].grid, row+1, col, RIGHT * ORTHOGONAL_FACTOR );
    GSET( temp_func.update_grids[1].grid, row-1, col+1, DOWN );
    GSET( temp_func.update_grids[1].grid, row+1, col+1, UP   );
  }
  else
  if( temp_func.update_grids[0].grid_value == 2 )
  {
    GSET( temp_func.update_grids[0].grid, row-1, col, LEFT * ORTHOGONAL_FACTOR );
    GSET( temp_func.update_grids[0].grid, row+1, col, LEFT * ORTHOGONAL_FACTOR );
    GSET( temp_func.update_grids[1].grid, row-1, col-1, DOWN );
    GSET( temp_func.update_grids[1].grid, row+1, col-1, UP   );
  }
  else
  if( temp_func.update_grids[0].grid_value == 3 )
  {
    GSET( temp_func.update_grids[0].grid, row-1, col-1, RIGHT );
    GSET( temp_func.update_grids[0].grid, row-1, col+1, LEFT  );
    GSET( temp_func.update_grids[1].grid, row, col-1, UP * ORTHOGONAL_FACTOR );
    GSET( temp_func.update_grids[1].grid, row, col+1, UP * ORTHOGONAL_FACTOR );
  }
  else
  if( temp_func.update_grids[0].grid_value == 4 )
  {
    GSET( temp_func.update_grids[0].grid, row+1, col-1, RIGHT );
    GSET( temp_func.update_grids[0].grid, row+1, col+1, LEFT  );
    GSET( temp_func.update_grids[1].grid, row, col-1, DOWN * ORTHOGONAL_FACTOR );
    GSET( temp_func.update_grids[1].grid, row, col+1, DOWN * ORTHOGONAL_FACTOR );
  }
  else
  {
    error( "Parameter error in split_vortex_info()\n" );
    printf( "param: %f\n",temp_func.update_grids[1].grid_value );
    exit(1);
  }
}               


/*
  This is the implementation of vortex follow intended for a simulation
  consisting of 2 direction grids, hortizonal and vertical.
*/
void split_vortex_follow(agent_struct *agent, int action_index)
{ 
  int             row, col, state;
  function_struct temp_func;

#ifdef split_vortex_error
  printf("\nsplit_vortex_follow");
#endif

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* Since this is the 2 direction grid implementation, only 2 grids are wanted */
  if ( temp_func.number_grids != 2 )
    {
      error( "Too many grids in function vortex_follow\n" );
    }
  
  
    if( fabs( GGET( temp_func.update_grids[0].grid, row, col ) ) == fabs( GGET( temp_func.update_grids[1].grid, row, col ) ) )
    {
      error("uh-oh: ambiguity\n");
      printf("grid val: %f\n", fabs( (double)GGET( temp_func.update_grids[0].grid, row, col ) ));
      printf("x: %d    y: %d\n",row,col);
      print_support_grid( 0 );
      print_support_grid( 1 );
      print_support_grid( 2 );
      exit(1);
    }
  

  /* Grid 1 is the vertical grid   Grid 0 is the horizontal grid */
  if( abs( GGET( temp_func.update_grids[1].grid, row, col ) ) > abs( GGET( temp_func.update_grids[0].grid, row, col ) ) )
    {
      /* UP is negative */
      if ( GGET( temp_func.update_grids[1].grid, row, col ) < 0 )
	{
	  move_up( agent, action_index );
	}
      
      /* DOWN is positive */
      if ( GGET( temp_func.update_grids[1].grid, row, col ) > 0 )
	{
	  move_down( agent, action_index );
	}
      
      /* 0 -> ambiguous direction, which should never happen */
      if ( GGET( temp_func.update_grids[1].grid, row, col ) == 0 )     
	{
	  error("0 recieved in vertical grid in vortex_follow\n");

          #ifdef _KILL_AMBIGUITY
	        grid_dump();
	        exit(1);
          #endif
	}
    }
  else
    {
      /* LEFT is negative */
      if ( GGET( temp_func.update_grids[0].grid, row, col ) < 0 )
	{
	  move_left( agent, action_index );
	}
      
      /* RIGHT is positive */
      if ( GGET( temp_func.update_grids[0].grid, row, col ) > 0 )
	{
	  move_right( agent, action_index );
	}
      
      /* 0 -> ambiguous direction, which should never happen */
      if ( GGET( temp_func.update_grids[0].grid, row, col ) == 0 )     
	{
	  error("0 recieved in horizontal grid in vortex_follow\n");

	  #ifdef _KILL_AMBIGUITY
    	        grid_dump();
	        exit(1);
	  #endif
	}
    }
}             
#undef _KILL_AMBIGUITY





