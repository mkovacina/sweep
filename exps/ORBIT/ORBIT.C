
void circ_move(double *x_loc, double *y_loc, double radius, double time)

     /* version 1 of satellite position function, only uses location information */
{
  *x_loc = radius * cos(time * (3.141592 / 180.0) );
  *y_loc = radius * sin(time * (3.141592 / 180.0) );

}

void orbits(agent_struct *agent, int action_index)
/*  one agent orbits another agent
    does a simple circular orbit */
{

  int Xloc, Yloc, rows, cols, state, action_grid;

  static double t1 = 7.0, t2 = 132.0, t3 = 83.0, t4 = 13.0, t5 = 163.0;
  double r1 = 5.3, r2 = 10.2, r3 = 7.0, r4 = 3.7, r5 = 13.1;

  double x, y;

  function_struct temp_func;

  /* acquire the position of the current agent */
  Xloc = agent->x_pos;
  Yloc = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  action_grid = temp_func.update_grids[0].grid;

  rows = GGETR(0);
  cols = GGETC(0);

  /* set up 5 orbitting objects around central agent */


  circ_move(&x, &y, r1, t1);
  if ( 0 <= (int)y+Yloc && (int)y+Yloc < GGETC(0) && 0 <= (int)x+Xloc && (int)x+Xloc < GGETR(0) )
        GSET(action_grid, (int)y + Yloc, (int)x + Xloc, 1); 

  circ_move(&x, &y, r2, t2);
  if ( 0 <= (int)y+Yloc && (int)y+Yloc < GGETC(0) && 0 <= (int)x+Xloc && (int)x+Xloc < GGETR(0) )
        GSET(action_grid, (int)y + Yloc, (int)x + Xloc, 1);

  circ_move(&x, &y, r3, t3);
  if ( 0 <= (int)y+Yloc && (int)y+Yloc < GGETC(0) && 0 <= (int)x+Xloc && (int)x+Xloc < GGETR(0) )
        GSET(action_grid, (int)y + Yloc, (int)x + Xloc, 1);

  circ_move(&x, &y, r4, t4);
  if ( 0 <= (int)y+Yloc && (int)y+Yloc < GGETC(0) && 0 <= (int)x+Xloc && (int)x+Xloc < GGETR(0) )
        GSET(action_grid, (int)y + Yloc, (int)x + Xloc, 1);

  circ_move(&x, &y, r5, t5);
  if ( 0 <= (int)y+Yloc && (int)y+Yloc < GGETC(0) && 0 <= (int)x+Xloc && (int)x+Xloc < GGETR(0) )
        GSET(action_grid, (int)y + Yloc, (int)x + Xloc, 1);

  t1 += 12.0;
  t2 += 17.0;
  t3 += 8.0;
  t4 += 6.0;
  t5 += 3.0;
}

