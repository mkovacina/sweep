/*
** Purpose:
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#pragma once

/*--------------- Includes Needed for Definitions Below --------------*/

#include <stdio.h>
#include "constants.h"

/*---------------------------- Constants -----------------------------*/

/*------------------------------ Macros ------------------------------*/

/*------------------------------- Types ------------------------------*/

typedef struct { 

  char  input;
  float probability;
  int   destination;
  
} transition_struct;

typedef struct {

  char              *description;
  int               number_transitions;
  transition_struct *transition;
  // todo: rename this to be plural
  
} state_struct;

typedef struct {

  int          current_state;
  int          number_states;
  state_struct *state;
  
} fsa_struct;

typedef struct {

  fsa_struct *fsa;
  int        number_fsa;
  
} fsa_table_struct;

/*------------------------- Function Prototypes ----------------------*/
/* void sample (char *, int, Real);                                   */

int  initialize_fsa_table ( fsa_table_struct*, FILE*);
int  copy_fsa             ( fsa_struct*, fsa_struct* );
int  delete_fsa           ( fsa_struct* );
int  clear_fsa_table      ( fsa_table_struct* );

void change_state         ( fsa_struct*, char );
void print_fsa            ( fsa_struct* );
void print_fsa_table      ( fsa_table_struct* );

/*------------------------------ Globals -----------------------------*/
/* GLOBAL  float  example;                                            */
