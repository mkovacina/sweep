/* gives access to error() */
#include "errors.h"

/* give access to print_*_grid() */
#include "print_grid.h"

#define RECRUIT_VALUE 255

#define UP    -1
#define DOWN   1
#define RIGHT  2
#define LEFT  -2


void leader_recruit(agent_struct *agent, int action_index)
{
  int row, col, state, dir;

  function_struct temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("leader_recruit\n");
#endif


  /* acquire the position of the current agent */
  row = agent->x_pos;
  col = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to leader_recruit().\n" );
    error( "One grid is required for leader_recruit()\n" );
    exit(1);
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  dir = GGET( temp_grid.grid, col, row );

  switch( dir )
  {
    case UP:
      GSET( temp_grid.grid, col - 1, row, DOWN  );

      temp_grid = temp_func.update_grids[0];
      GSET( temp_grid.grid, col - 1, row, 0 );      
      break;

    case RIGHT:
      GSET( temp_grid.grid, col, row + 1, LEFT );

      temp_grid = temp_func.update_grids[0];
      GSET( temp_grid.grid, col, row + 1, 0 );
      break;

    case DOWN:
      GSET( temp_grid.grid, col + 1, row, UP );

      temp_grid = temp_func.update_grids[0];
      GSET( temp_grid.grid, col + 1, row, 0 );
      break;

    case LEFT:
      GSET( temp_grid.grid, col, row - 1, RIGHT );

      temp_grid = temp_func.update_grids[0];
      GSET( temp_grid.grid, col, row - 1, 0 );
      break;

    default:
      puts("Uh-oh");printf("dir = %d\n",dir);grid_dump();/*abort()*/;break; 
  }
}


void leader_safe_wander( agent_struct *agent, int action_index )
/*  "safe" wandering, will not allow multiple agents to occupy same location */
{

  int dir, count, test_val; /* randomly selected direction */
  int row, col, rows, cols, state, action_grid;

  function_struct    temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("leader_safe_wander\n");
#endif

  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to leader_wander().\n" );
    error( "One grid is required for leader_wander()\n" );
    exit(1);
  }

  temp_grid = temp_func.update_grids[0];

  if ( GGET( temp_grid.grid, row, col ) != 0 )
  {
    leader_recruit( agent, action_index );
  }

  /* determines which support grid contains pertinent information */
  action_grid = temp_func.update_grids[0].grid;
 
  rows = GGETR(0);
  cols = GGETC(0);

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
	    GSET( temp_grid.grid, row, col, UP );
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
	    GSET( temp_grid.grid, row, col, RIGHT );
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
	    GSET( temp_grid.grid, row, col, DOWN );
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
	    GSET( temp_grid.grid, row, col, LEFT );
	    move_left( agent, 0 ); 
	    return;
	  }
	}
	count++;
    }
  }
}


/* 
   One grid is supplied

   grid #0: message grid
*/
void leader_place_info( agent_struct *agent, int action_index )
{
  int row, col, state;

  function_struct temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("leader_place_info\n");
#endif


  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to leader_place_info().\n" );
    error( "One grid is required for leader_place_info()\n" );
    exit(1);
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /*
     must look at GCURR b/c the "leader"'s new position is not guaranteed to be
     the same as it was in the past.
  */
  if ( GCURR( temp_grid.grid, row, col ) != RECRUIT_VALUE )
  {
    GSET( temp_grid.grid, row + 1, col, UP    );
    GSET( temp_grid.grid, row, col - 1, RIGHT );
    GSET( temp_grid.grid, row, col + 1, LEFT  );
    GSET( temp_grid.grid, row - 1, col, DOWN  );
  }
  else
  {
    GSET( temp_grid.grid, row, col, GGET( temp_grid.grid, row, col ) );
  }
}


/* 
   One grid is supplied

   grid #0: message grid
*/
void follower_safe_follow(agent_struct *agent, int action_index)
{
  int dir;
  int row, col, state;

  function_struct temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("follower_safe_follow\n");
#endif


  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to follower_safe_follow().\n" );
    error( "One grid is required for follower_safe_follow()\n" );
    exit(1);
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  dir = GGET( temp_grid.grid, row, col );

  /* copy the direction to the new grid for next agent or for self if waiting */
  GSET( temp_grid.grid, row, col, dir );

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
}



/* 
   One grid is supplied

   grid #0: message grid
*/
void follower_place_info( agent_struct *agent, int action_index )
{
  int row, col, state, dir;

  function_struct temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("follower_place_info\n");
#endif


  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to follower_place_info().\n" );
    error( "One grid is required for follower_place_info()\n" );
    exit(1);
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  dir = GGET( temp_grid.grid, row, col );

  GSET( temp_grid.grid, row, col, dir );


  switch( dir )
  {
     case UP:
     case DOWN:
       GSET( temp_grid.grid, row, col - 1, RIGHT );
       GSET( temp_grid.grid, row, col + 1, LEFT  );
       break;

     case LEFT:
     case RIGHT:
       GSET( temp_grid.grid, row - 1, col, DOWN );       
       GSET( temp_grid.grid, row + 1, col, UP   );
       break;

     default:
       printf("%d\n",dir);
       puts("uh-ho");
       break;
  }

}



