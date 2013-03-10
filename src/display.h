/* These are functions relating to the display swarm experiement data */
/* These are functions relating to the swarm of agents */

# ifndef _display_
# define _display_

void write_world(FILE *);

void write_world_full(FILE *);

void write_world_stdout(FILE *);

void write_swarm(FILE * /* ,agent_struct*[]*/);

int write_probe(FILE *, agent_struct*[]);

void write_grid(FILE *, int);


# endif /* _display_ */
