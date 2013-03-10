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
  Four grids are supplied to wander_info().
  
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
      error( "Four grids are required for split_wander_info()\n" );
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
