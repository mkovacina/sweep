/* 
   This program reads in a probe file, and generates two C functions that 
   correspond to the swarm-based and agent-based probes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "support_grids.h"
#include "agent.h"

void skip_file_read(char buffer[], FILE * pfile)
{
	char * result;
	int count;

	/* determine the number of agent based counted items there are */
	result = fgets(buffer, MAX_BUFFER, pfile);

	while (!feof(pfile) && (*result == '\n' || buffer[0] == '#')){
		result = fgets(buffer, MAX_BUFFER, pfile);
	}
	count = 0;
	while (buffer[count++] != '\n'){}  /* find carriage return */
	buffer[count-1] = '\0';
}

int make_num(char buffer[], int *pos)
{
	int int_compute = 0;

	while (buffer[*pos] >= '0' && buffer[*pos] <= '9'){
		int_compute = int_compute * 10 + buffer[*pos] - '0';
		(*pos)++;
	}

	return int_compute;	
}

void clear_buffer(char buffer[])
{
	int pos = 0;

	for (pos=0; pos<MAX_BUFFER; pos++){
		buffer[pos] = '\n';
	}
}

int main()
{
	FILE *probe_file;
	char buffer[MAX_BUFFER];
	int count, pos;
	int ani_flag, check, count_var, num;
	int num_count_items, num_agent_count, num_val;
	int index, int_compute, state_id, num_grid_count, num_actions;
	int ani_world=0, ani_agents=0, time_step;

	/* open the priorities data file */
	probe_file = fopen("probe.dat", "r");
	if (probe_file == NULL) {
		printf("Can't open file: %s\n", "probe.dat");
		exit(1);
	}

	printf("#include \"support_grids.h\"\n");
	printf("#include \"agent.h\"\n");
	printf("#include \"display.h\"\n");
	printf("#include \"print_grid.h\"\n");
	printf("#include \"swarm.h\"\n\n");
	printf("#include \"constants.h\"\n\n");

	printf("\n\n /***** WARNING *****/\n");
	printf(" /***** WARNING *****/\n");
	printf(" /*                 */\n");
	printf(" /* these functions */\n");
	printf(" /* are generated   */\n");
	printf(" /* automatically!! */\n");
	printf(" /* do not modify!! */\n");
	printf(" /*                 */\n");
	printf(" /***** WARNING *****/\n");
	printf(" /***** WARNING *****/\n");
	printf("\n\n");
	printf("\n");

	skip_file_read(buffer, probe_file);
	num_count_items = atoi(buffer);                   

	/* generate global variables to communicate between probe functions */
	printf("/* Probe GLOBALs - not accessible outside probe code */\n");
	printf("int count_array[%d]={0};\n",num_count_items);
	printf("int iter_track, agent_count;\n");

	/* Determines if animation should be generated for this run */
	skip_file_read(buffer, probe_file);

	/* ensures that the string terminates at the end of "Animation" */
	buffer[9] ='\0';
	ani_flag = !strcmp(buffer, "Animation");

	if (!ani_flag){
		/* ensures that the string terminates at the end of "No" */
		buffer[2] = '\0';
		check = !strcmp(buffer, "No");
		if (!check) {
			printf("Illegal format for probe file, aborting program.\n");
			exit(1);
		}
	}

	if (ani_flag){
		printf("FILE* ANI_FILE;\n");

		if ( strncmp( &(buffer[10]), "default", strlen("default") ) == 0 )
		{
			printf("int animation_type = 0;\n" );


			if ( strncmp( &(buffer[strlen("default")+10]),"-stdout", strlen("-stdout") ) == 0 )
			{
				printf("int write_to_stdout = 1;\n");
			}
			else
			{
				printf("int write_to_stdout = 0;\n");
			}
		}
		else if ( strncmp( &(buffer[10]), "full_world", strlen("full_world") ) == 0 )
		{
			printf("int animation_type = 1;\n" );

			if ( strncmp( &(buffer[strlen("full_world")+10]),"-stdout", strlen("-stdout") ) == 0 )
			{
				printf("int write_to_stdout = 1;\n");
			}
			else
			{
				printf("int write_to_stdout = 0;\n");
			}
		}
		else
		{
			printf("int animation_type = 0;\n" );
			printf("int write_to_stdout = 0;\n");
		}

		printf("\n");
	}

#if 0
	printf("void count_agents(swarm_element_struct * swarm)\n");
	printf("{\n");
	printf("    swarm_element_struct *tracer = swarm;\n");
	printf("\n");
	printf("    agent_count = 0;\n");
	printf("\n");
	printf("    while ( tracer != NULL ) {\n");
	printf("       agent_count++;\n");
	printf("       tracer = tracer->next_agt;\n");
	printf("    }\n"); 
	printf("}\n");               
#endif

	//    printf("#include \"display.c\"\n\n");

	printf("void probe_agents(agent_ptr agent)\n");
	printf("{\n");
	printf("    static int iteration = 0;\n\n");

	printf("    if (iter_track != iteration){\n");
	printf("       agent_count = 0;\n");
	printf("       iteration++;\n");
	printf("    }\n\n");

	printf("    agent_count++;\n\n");

	skip_file_read(buffer, probe_file);

	num_agent_count = atoi(buffer);

	for (count=0; count<num_agent_count;count++){
		printf("    if( ");

		skip_file_read(buffer, probe_file);
		if (buffer[0] != 'C'){
			printf("Illegal probe file - count vars must begin w/ 'C'\n");
			exit(1);
		}

		index = 1;

		/* get count variable */
		int_compute = make_num(buffer, &index);

		if (int_compute != count){
			printf("Illegal probe file - count var #%d corrupted\n", count);
			exit(1);
		}

		/* find starting point */
		for (pos=index; buffer[pos] != ':';pos++){};

		/* eat spaces */
		while (buffer[++pos] == ' '){}

		/* convert 'A' counts to state tallies */
		if (buffer[pos] == 'A'){
			if (buffer[pos+1] >= '0' && buffer[pos+1] <= '9'){
				pos++;
				state_id = make_num(buffer, &pos);
				printf("agent->fsa->current_state == %d ){\n", state_id);

				printf("       count_array[%d]++;\n", int_compute);
				printf("    }\n\n");
			} 
			else { /* eventually positional information goes here */
				printf(" agent counts currently *must* be states only!\n");
				exit(1);
			}
		}		 
		else { /* eventually will handle conditional expressions */
			printf(" agent counts currently must be states only!\n");
			exit(1);
		}
	}
	printf("} /* probe_agents */\n\n\n");

	printf("void probe_swarm(int *done, swarm_element_struct *swarm)\n");
	printf("{\n");
	printf("    int terminate = 0;\n");
	printf("    static int initial_call = 1;\n");
	printf("    static int iter_count = 0;\n");
	printf("    int row, col, rows, cols, count;\n");

	/*
	   done so gcc will shut up about these vars being unused when
	   in fact they MAY be used, but not always (within probes.c)
	 */
	printf("    row = col = rows = cols = 0;\n");

	printf("    iter_count++;\n\n");

	printf("    if (initial_call){\n");
	printf("       initial_call = 0;\n\n");

	printf("       for (count=0; count<%d; count++){\n", num_count_items);
	printf("           count_array[count] = 0;\n");
	printf("       }\n\n");

	if (ani_flag) {
		printf(" if ( write_to_stdout == 1 ){\n" );
		printf("       ANI_FILE = stdout;\n");
		printf("    } else {\n" );
		printf("       ANI_FILE = fopen(\"swarm_sim.dat\", \"w\");\n");
		printf("    }\n");

		printf("       if (ANI_FILE == NULL){\n");
		printf("          printf(\"Error Opening Animation File: swarm_sim.dat\\n\");\n");
		printf("          printf(\"Exiting program\\n\"); \n");
		printf("          exit(1);\n");
		printf("       }\n");

		printf("       fprintf(ANI_FILE,\"%%d %%d\\n\", GGETR(0), GGETC(0));\n");
		//       printf("       count_agents(swarm);\n");
		printf("       fprintf(ANI_FILE,\"%%d\\n\", agent_count);\n");


	}

	printf("    }\n");

	/* Determine how many sets of probe-actions there are */
	skip_file_read(buffer, probe_file);

	num_grid_count = atoi(buffer);

	if (num_grid_count > 0){
		printf("    rows = GGETR(0);\n");
		printf("    cols = GGETC(0);\n\n");

		printf("    for (row=0;row<rows;row++){\n");
		printf("        for (col=0;col<cols;col++){\n");

		for (count=0; count<num_grid_count;count++){
			printf("           if ( ");

			clear_buffer(buffer);
			skip_file_read(buffer, probe_file);

			if (buffer[0] != 'C'){
				printf("Illegal probe file - count vars must begin w/ 'C'\n");
				exit(1);
			}

			index = 1;

			/* get count variable */
			int_compute = make_num(buffer, &index);

			if (int_compute != count + num_agent_count){
				printf("Illegal probe file - count var #%d corrupted\n", count);
				exit(1);
			}

			while (buffer[index] == ':' || buffer[index] == ' '){
				index++;
			}

			/* Get the boolean expression from the priority file */
			/* and insert it into the conditional expression */

			pos = index;
			while (buffer[pos] != '\n'){
				if (buffer[pos] == 'C'){
					pos++;
					num_val = make_num(buffer, &pos);
					printf("count_array[%d] ", num_val);
				}
				else {
					printf("%c", buffer[pos]);

					if (buffer[pos] == 'G') {
						pos++;
						num_val = make_num(buffer, &pos);
						printf("GET(%d,row,col) ", num_val);
					}
					else {
						pos++;
					}
					/* # sign is comment character for priority file, ignore */
					/* everything after it until the end of line             */
				}

				if (buffer[pos] == '#') {
					break;
				}	    
			}

			printf("){\n");

			printf("              count_array[%d]++;\n", int_compute);
			printf("           }\n");
			printf("\n");
		}

		printf("        }\n");
		printf("    }\n\n");
	}

	/* Probe-actions */

	skip_file_read(buffer, probe_file);
	num_actions = atoi(buffer);

	for (count=0; count<num_actions;count++){
		index = 0;
		printf("    if ( ");

		skip_file_read(buffer, probe_file);

		while (buffer[index] != ':'){
			if (buffer[index] == 'C'){
				index++;
				count_var = make_num(buffer, &index);
				printf("count_array[%d]", count_var); 
			}
			else if (buffer[index] == 'N'){
				printf("agent_count");
				index++;
			}
			else if (buffer[index] == 'I'){
				printf( "iter_track");
				index++;
			}
			else {
				printf("%c",buffer[index++]);
			}
		}

		printf("){\n"); /* close opened "if" clause */

		while (buffer[++index] == ' ') {
		}; /* eat spaces */

		if (buffer[index] == 'T' || buffer[index] == 't'){
			printf("      TraceLineVerbose(\"Probe action  %d -> TERMINATE\\n\");\n",count);

			printf("       terminate = 1;\n");
		}
		else if (buffer[index] == 'M'){
			printf("       printf(\"");
			index++;
			while (buffer[++index] != '"'){
				printf("%c", buffer[index]);
			}

			printf("\\n\");\n");
		}
		else if (buffer[index] == 'I'){
			index++;
			printf("       TraceLineVerbose(\"Iteration: \");");
			printf("       printf(\" %%d\\n\", iter_track);\n");
		}
		else if (buffer[index] == 'C'){
			index++;
			num = make_num(buffer, &index);
			printf("       printf(\"C%d: %%d\\n\", count_array[%d]);\n", 
					num, num);
		}
		else if (buffer[index] == 'G'){
			index++;
			count_var = make_num(buffer, &index);
			printf("      print_support_grid(%d);\n", count_var);
		}
		else if (buffer[index] == 'A'){
			index++;
			count_var = make_num(buffer, &index);
			printf("      print_agent_grid();\n");
		}       
		else if (buffer[index] == 'D'){
			index++;
			count_var = make_num(buffer, &index);
			printf("      grid_dump();\n");
		}
		else {
			printf("Error in Probe actions  action is %c\n", buffer[index]);
			exit(1);
		}

		printf("    }\n\n");
	}

	index = 0;
	skip_file_read( buffer, probe_file);
	time_step = make_num(buffer, &index);

	skip_file_read( buffer, probe_file);
	while (!feof(probe_file)){

		if (buffer[0] == 'W' && ani_world == 0 && ani_flag ){
			ani_world = 1;
			printf("\n");
			printf("    if (iter_count %% %d == 0){\n", time_step);
			printf("       switch( animation_type ){\n" );
			printf("           case 0: write_world(ANI_FILE); break;\n");
			printf("           case 1: write_world_full(ANI_FILE); break;\n");
			printf("       }\n");

			/*	  printf("       fprintf(ANI_FILE, \"\\n\");\n");  */
			printf("    }\n\n");
		}
		if (buffer[0] == 'A' && ani_agents == 0 && ani_flag ){
			ani_agents = 1;
			printf("\n");
			printf("    if (iter_count %% %d == 0){\n", time_step);
			printf("       write_swarm(ANI_FILE);\n");
			printf("    }\n\n");
		}

		if (buffer[0] == 'G'){
			index = 1;
			num = make_num(buffer, &index);
			printf("\n");
			printf("    if (iter_count %% %d == 0){\n", time_step);
			printf("       write_grid(ANI_FILE, %d);\n", num);
			printf("    }\n\n");
		}

		skip_file_read( buffer, probe_file);
	}

	printf("    if (terminate){\n");
	if (ani_flag){
		printf("       fprintf(ANI_FILE, \"#\\n\");\n");
		printf("       fclose(ANI_FILE);\n");
	}

	printf("       TraceLineVerbose(\" Program Terminating!\");\n");

	printf("       *done = 1;\n");
	printf("    }\n");

	printf("    iter_track = iter_count;\n");

	printf("    for (count=0; count<%d; count++){\n", num_count_items);
	printf("        count_array[count] = 0;\n");
	printf("    }\n\n");

	printf("}\n");

	return 0;
}










