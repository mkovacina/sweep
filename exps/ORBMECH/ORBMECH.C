void x_thrust(agent_struct *agent, int action_index)
{

  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* sets the x direction thrust to be the passed in value */
  agent->field[6] = temp_grid.grid_value;
}

void y_thrust(agent_struct *agent, int action_index)
{
  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* sets the y direction thrust to be the passed in value */
  agent->field[7] = temp_grid.grid_value;
}

void z_thrust(agent_struct *agent, int action_index)
{
  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* sets the z direction thrust to be the passed in value */
  agent->field[8] = temp_grid.grid_value;
}

void x_setpos(agent_struct *agent, int action_index)
{
  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  printf("Setting the X position to this: %f\n", temp_grid.grid_value);
  /* sets the x initial position to be the passed in value */
  agent->field[0] = temp_grid.grid_value;
}

void y_setpos(agent_struct *agent, int action_index)
{
  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* sets the y initial positon to be the passed in value */
  agent->field[1] = temp_grid.grid_value;
}

void z_setpos(agent_struct *agent, int action_index)
{
  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* sets the z initial position to be the passed in value */
  agent->field[2] = temp_grid.grid_value;
}

void x_setvel(agent_struct *agent, int action_index)
{
  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* sets the initial x velocity to be the passed in value */
  agent->field[3] = temp_grid.grid_value;
}

void y_setvel(agent_struct *agent, int action_index)
{
  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* sets the initial y velocity to be the passed in value */
  agent->field[4] = temp_grid.grid_value;
}

void z_setvel(agent_struct *agent, int action_index)
{
  int state;
  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;
  
  /* access the support grid to be used by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* sets the initial z velocity to be the passed in value */
  agent->field[5] = temp_grid.grid_value;
}

// Runge-kutta algorithm for satellite relative dynamics

// float dt, w, step_size, ang_freq;

// The following nine variables are inputs to the function.
// float x0, y0, z0; //initial position (x0,y0,z0)
// float Vx0, Vy0, Vz0; //initial velocity (Vx0,Vy0,Vz0)
// float Ux, Uy, Uz; //input force vector (Ux,Uy,Uz)

void runge_kutta(float * x, float * y, float * z,
		     float * Vx, float * Vy, float *Vz,
		     float step_size, 
		     float Ux, float Uy, float Uz)
{

	float f11, f21, f31, f41, f51, f61;
	float f12, f22, f32, f42, f52, f62;
	float f13, f23, f33, f43, f53, f63;
	float f14, f24, f34, f44, f54, f64;
	
	float ang_freq = 1.0;
	float x0, y0, z0; //initial position (x0,y0,z0)
	float Vx0, Vy0, Vz0; //initial velocity (Vx0,Vy0,Vz0)


	float dt = step_size; // suggest take value be 1E-2~1E-3 of "w"
	float w = ang_freq;  /* Dimensionless parameter for angular frequency
                  of the dynamic system, has to be a constant throughout
                  the simulation, and suggest take value = 1 */

	/* setting up the initial values from the input parameters */
	x0 = *x;
	y0 = *y;
	z0 = *z;
	Vx0 = *Vx;
	Vy0 = *Vy;
	Vz0 = *Vz;

    	f11 = Vx0;
    	f21 = 2*w*Vy0+Ux;
    	f31 = Vy0;
    	f41 = -2*w*Vx0+3*w*w*y0+Uy;
    	f51 = Vz0;
    	f61 = -w*w*z0+Uz;

    	f12 = Vx0+dt*f21/2;
    	f22 = 2*w*(Vy0+dt*f41/2)+Ux;
    	f32 = Vy0+dt*f41/2;
    	f42 = -2*w*(Vx0+dt*f21/2)+3*w*w*(y0+dt*f31/2)+Uy;
    	f52 = Vz0+dt*f61/2;
    	f62 = -w*w*(z0+dt*f51/2)+Uz;

   	f13 = Vx0+dt*f22/2;
    	f23 = 2*w*(Vy0+dt*f42/2)+Ux;
    	f33 = Vy0+dt*f42/2;
    	f43 = -2*w*(Vx0+dt*f22/2)+3*w*w*(y0+dt*f32/2)+Uy;
    	f53 = Vz0+dt*f62/2;
    	f63 = -w*w*(z0+dt*f52/2)+Uz;

    	f14 = Vx0+dt*f23;
    	f24 = 2*w*(Vy0+dt*f43)+Ux;
    	f34 = Vy0+dt*f43;
    	f44 = -2*w*(Vx0+dt*f23)+3*w*w*(y0+dt*f33)+Uy;
    	f54 = Vz0+dt*f63;
    	f64 = -w*w*(z0+dt*f53)+Uz;

    	//output variables:
    	*x = x0+dt*(f11+2*f12+2*f13+f14)/6;
    	*Vx = Vx0+dt*(f21+2*f22+2*f23+f24)/6;

    	*y = y0+dt*(f31+2*f32+2*f33+f34)/6;
    	*Vy = Vy0+dt*(f41+2*f42+2*f43+f44)/6;

    	*z = z0+dt*(f51+2*f52+2*f53+f54)/6;
    	*Vz = Vz0+dt*(f61+2*f62+2*f63+f64)/6;
}


void orbit_update(agent_struct *agent, int action_index)
{
  float x, y, z;
  float vx, vy, vz;
  float thrx, thry, thrz;
  int rows, cols;

  rows = GGETR(0);
  cols = GGETC(0);

  x = agent->field[0];
  y = agent->field[1];
  z = agent->field[2];

  vx = agent->field[3];
  vy = agent->field[4];
  vz = agent->field[5];

  thrx = agent->field[6];
  thry = agent->field[7];
  thrz = agent->field[8];

  printf("X position is: %f\n", x);
  runge_kutta(&x, &y, &z, &vx, &vy, &vz, 0.01, thrx, thry, thrz);

  printf("New X position is: %f\n", x);
  agent->field[6] = 0.0;
  agent->field[7] = 0.0;
  agent->field[8] = 0.0;

  agent->field[0] = x;
  agent->field[1] = y;
  agent->field[2] = z;

  agent->field[3] = vx;
  agent->field[4] = vy;
  agent->field[5] = vz;

  /* update actual position of the satellite  with the values from the XZ plane */
  //agent->x_pos = 100+(int)(50 * x);
  //agent->y_pos = 100+(int)(50 * z);
  agent->x_pos = (int)x;
  agent->y_pos = (int)z;

}

