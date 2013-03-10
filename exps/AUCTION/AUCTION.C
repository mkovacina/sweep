void initialize(agent_struct *agent, int action_index){

  char filename[20];

  FILE * msg_file;

  strcpy(filename, "agentX.dat");

  filename[5] = agent->id_number + '0';   

  msg_file = fopen(filename, "w");

  if (msg_file == NULL){
    printf("Unsuccessful opening of the file, ending program\n");
    exit(1);
  }

  fclose(msg_file);

  agent->field[7] = 0;

  printf("Agent %d at row, col %d %d\n", agent->id_number, agent->y_pos, agent->x_pos);

}

void find_angle(agent_struct *agent, update_grid_struct temp_grid){

  int target[2][10];
  float tar_row, tar_col;  /*Row and column of target */
  float ag_row, ag_col;    /*Row and column of agent */

  float angle;

  find_target(temp_grid, target);

  tar_row = target[0][(int)agent->field[8]];
  tar_col = target[1][(int)agent->field[8]];
  
  ag_row = agent->y_pos;
  ag_col = agent->x_pos;

  /* See actions.c, pick_any for more info on the following */
  /* This computes the tangent of the angle between the agent and the target */

  agent->count_amt = 0.0; 

  if(tar_col == ag_col){

    agent->update_amt = 0;

  }

  /* -(tar_row - ag_row) is used to translate the row column system SWEEP uses to cartesian, which atan2 uses */

  angle = atan2(-(tar_row - ag_row), (tar_col - ag_col));

  printf("Angle is %f\n", angle);

  if((angle >= 0) && (angle < (PI / 4))){

    agent->dir = 1;
    agent->switch_dir = 0;
    agent->update_amt = -((tar_row - ag_row) / (tar_col - ag_col));

  } else if((angle >= (PI / 4)) && (angle < (PI / 2))){

    agent->dir = 0;
    agent->switch_dir = 1;
    agent->update_amt = -tan((angle - (PI / 2)));

  } else if((angle >= (PI / 2)) && (angle < (( 3 * PI) / 4))){

    agent->dir = 0;
    agent->switch_dir = 3;
    agent->update_amt = tan((angle - (PI / 2)));

  } else if((angle >= (( 3 * PI) / 4))){

    agent->dir = 3;
    agent->switch_dir = 0;
    agent->update_amt = ((tar_row - ag_row) / (tar_col - ag_col));

  } else if(angle < -(( 3 * PI) / 4)){

    agent->dir = 3;
    agent->switch_dir = 2;
    agent->update_amt = -((tar_row - ag_row) / (tar_col - ag_col));

  } else if((angle >= -(( 3 * PI) / 4)) && (angle < -(PI / 2))){

    agent->dir = 2;
    agent->switch_dir = 3;
    agent->update_amt = -tan((angle + (PI / 2)));

  } else if((angle >= -(PI / 2)) && (angle < -(PI / 4))){

    agent->dir = 2;
    agent->switch_dir = 1;
    agent->update_amt = tan((angle + (PI / 2)));

  } else if(angle >= -(PI / 4)){

    agent->dir = 1;
    agent->switch_dir = 2;
    agent->update_amt = ((tar_row - ag_row) / (tar_col - ag_col));

  }

  printf("Agent update amt is %f\n", agent->update_amt);

}



void best_get(agent_struct *agent, int action_index){

  int i;

  int state;  

  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */  
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];


  for(i = 0; i < 11; i++){

    if(agent->target[0][i] != -1){

      agent->field[8] = agent->target[0][i];

      /* The sqrt is stored because the distance is computed as distance squared */
      agent->field[9] = sqrt(agent->target[1][i]);

      //      agent->target[i] = -1;

      break;
    }
  }

  printf("Agent %d choosing target number %d\n", agent->id_number, agent->target[0][i]);

  GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 2);

  find_angle(agent, temp_grid);

}


void read_msg(agent_struct *agent, int action_index){

  char filename[20];
  int my_num;
  int t_num;      /*Target number*/
  int distance;   /*Information from current message */
  int tot_dist = 0;   /*Holds total distance*/
  int i;          /*loop variable */
  int count = 0;  /*counter, holds info determining whether all agents have reported*/

  FILE * message;

  my_num = agent->id_number;

  strcpy(filename, "agentX.dat");

  filename[5] = my_num + '0';  /*Replaces "X" with the current agent's number */

  message = fopen(filename, "r");

  if (message == NULL){
    printf("Unsuccessful opening of the file, ending program\n");
    exit(1);
  }

  while(!feof(message)){

    fscanf(message, "%d %d", &t_num, &distance);

    if(t_num >= 0 && t_num < 11){

      for(i = 0; i < 11; i++){

	if(agent->target[0][i] == t_num){
	  agent->target[0][i] = -1;
	  tot_dist = tot_dist + distance;
	  break;
	}
      }
    }

  }

  fclose(message);

  message = fopen(filename, "w");  /* Opening in order to erase file */
  if (message == NULL){
    printf("Unsuccessful opening of the file, ending program\n");
    exit(1);
  }
  fclose(message);
  
}


void broadcast(agent_struct *agent, int action_index){

  char filename[20];
  int my_num;
  int count;

  FILE * message;

  strcpy(filename, "agentX.dat");

  for(count = 1; count < 11; count++){

    filename[5] = count + '0';  /*Places the receiving agent's number in the string*/

    message = fopen(filename, "a");

    if (message == NULL){
      printf("Unsuccessful opening of the file, ending program\n");
      exit(1);
    }

    fprintf(message, "%d %d ", (int)agent->field[8], (int)agent->field[9]);

    fclose(message);

    //  printf("Agent %d broadcasting %d %f to all other agents\n", agent->id_number, (int)agent->field[8], agent->field[9]);
  }


}

void ptop(agent_struct *agent, int action_index){

  char filename[20];
  int my_num;   /*Holds ID number of agent sending message */
  int receiver_num;    /*Holds ID number of agent receiving message */

  FILE * message;

  my_num = agent->id_number;

  receiver_num = (int)agent->field[10];  

  strcpy(filename, "agentX.dat");

  filename[5] = receiver_num + '0';  /*Replaces "X" with the receiving agent's number */

  message = fopen(filename, "a");

  if (message == NULL){
    printf("Unsuccessful opening of the file, ending program\n");
    exit(1);
  }

  fprintf(message, "%d %d ", (int)agent->field[8], (int)agent->field[9]);

  fclose(message);


}


void find_target(update_grid_struct temp_grid, int target[][10]){

  static int tar_loc[2][10];
  static int counter = 0;

  int count = 0;

  int i,j;

  if(counter == 0){

    /*Initializes array to -1*/
    for(i = 0; i < 2; i++){
      for(j = 0; j < 10; j++){

	tar_loc[i][j] = -1;

      }
    }

    counter = 1;

    for(i = 0; i < 40; i++){
      for(j = 0; j < 40; j++){

	if(GGET(temp_grid.grid, i, j) == 3){
	  
	  tar_loc[0][count] = i;
	  tar_loc[1][count] = j;

	  printf("Target %d is located at %d %d\n", count, i, j);

	  count++;

	}
      }
    }

  }

  /* Effectively passes back info through the array 'target' */

  for(i = 0; i < 2; i++){
    for(j = 0; j < 10; j++){

      target[i][j] = tar_loc[i][j];

    }
  }

}

void measure(agent_struct *agent, int action_index){

  int tar_info[2][10];  /*Holds target info: target number, row, col position */

  int i,j;   /*Counters */
  int count = 0;

  int distance[2][10];  /*Holds distance to each target */
  int temp;  /*Used when calculating distances */

  int min, remain, sorted, tar_num;  /* Used for sorting */

  int state;

  function_struct temp_func;
  update_grid_struct temp_grid;

  state = agent->fsa->current_state;
  temp_func = agent->state_functions[state].function_list[action_index];
  temp_grid = temp_func.update_grids[0];

  find_target(temp_grid, tar_info);

  while( count < 10){

    if(tar_info[0][count] == -1){
      break;
    }

    temp = pow((agent->x_pos - tar_info[1][count]), 2.0) + pow((agent->y_pos - tar_info[0][count]), 2.0);

    distance[0][count] = count;
    distance[1][count] = temp;

    count++;
  }

  /* Sorts the list of distances */
  for(sorted = 0; sorted < count;  sorted++){

    min = distance[1][sorted];

    for(remain = (sorted + 1); remain < count; remain++){

      if(min > distance[1][remain]){

	tar_num = distance[0][remain];
	min = distance[1][remain];

	distance[0][remain] = distance[0][sorted];
	distance[1][remain] = distance[1][sorted];

	distance[0][sorted] = tar_num;
	distance[1][sorted] = min;

      }
    }
  }

  for(i = 0; i < 2; i++){
    for(j = 0; j < 10; j++){

      agent->target[i][j] = distance[i][j];

    }
  }

}



void tar_move(agent_struct *agent, int action_index){

  int dir_store;
  int target[2][10];

  int state;

  function_struct temp_func;
  update_grid_struct temp_grid;

  state = agent->fsa->current_state;
  temp_func = agent->state_functions[state].function_list[action_index];
  temp_grid = temp_func.update_grids[0];

  find_target(temp_grid, target);

  if(agent->field[9] > 1.0){

    agent->field[9] = agent->field[9] - 1;

  } else {

    find_angle(agent, temp_grid);

  }

  switch (agent->dir){
  case 0: move_up(agent,0); break;
  case 1: move_right(agent,0); break;
  case 2: move_down(agent,0); break;
  case 3: move_left(agent,0); break;
  }

  agent->count_amt = agent->count_amt + agent->update_amt;

  if (agent->count_amt > 1.0){
    agent->count_amt = agent->count_amt - 1.0;

    dir_store = agent->dir;

    /*UGLY HACK!!!!! */
    agent->field[18] += 1;

    switch (agent->switch_dir){
    case 0: move_up(agent,0); break;
    case 1: move_right(agent,0); break;
    case 2: move_down(agent,0); break;
    case 3: move_left(agent,0); break;
    }

    agent->dir = dir_store;

  }

  if(GGET(temp_grid.grid, agent->y_pos, agent->x_pos) == 3){

    if((agent->x_pos == target[1][(int)agent->field[8]]) && (agent->y_pos == target[0][(int)agent->field[8]])){
      
      GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 5);

    }

  } else {

    GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 2);

  } 

}
