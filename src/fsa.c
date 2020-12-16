/*----------------------- Includes Needed ----------------------------*/

#include "fsa.h"
#include "errors.h"
#include "useful.h"
#include "string-util.h"

#include <stdlib.h>
#include <string.h>

/*------------------------ Externs Needed --------------------------- */

/*------------------------ Local Constants ---------------------------*/

/*------------------------- Local Macros -----------------------------*/

/*-------------------------- Local Types -----------------------------*/

/*---------------------- Local Function Prototypes -------------------*/
/* void sample (char *, int, Real);                                   */

void getinputdestprob     ( char*, char*, int*, float* );
int  getnumbertransitions ( char* );

int copy_state      ( state_struct*, state_struct* );
int copy_transition ( transition_struct*, transition_struct* );

/*---------------------- Function Definitions ------------------------*/

int initialize_fsa_table (fsa_table_struct *fsa_table, FILE* fsa_file) {
/* PURPOSE: Open the fsa initialization file and parse it to create a */
/*          a table of fsa structs                                    */
/* INPUT:   fsa_table  Uninitialized array of fsas                    */
/* OUTPUT:  fsa_table  Initialized array of fsas                      */
/* RETURN:  SUCCESS    Indicates successful initialization            */
/*          FAILURE    Indicates unsuccessful initialization          */

  char *current_char, buffer[MAX_BUFFER];
  
  int i,j,k;

  char  current_input;
  int   current_sum;

  /* Read in from the file the number of fsas */
  fgets( buffer, MAX_BUFFER, fsa_file );

  fsa_table->number_fsa = atoi( buffer );
  fsa_table->fsa = malloc( sizeof( fsa_struct ) * ( fsa_table->number_fsa ) );

  /* Eat next line */
  fgets( buffer, MAX_BUFFER, fsa_file );
  
  k = 0;
  
  /* Loop until EOF is encountered, first buffer char is null */
  while ( !strstr( buffer, "####" ) ) 
  {
    /* Retrieve the number of states and allocate space for them */
    fgets( buffer,MAX_BUFFER,fsa_file);
    fsa_table->fsa[k].number_states = atoi( buffer );
    fsa_table->fsa[k].state = malloc( sizeof( state_struct ) * fsa_table->fsa[k].number_states );
    
    /* Eat the next line */
    fgets( buffer, MAX_BUFFER, fsa_file );
    
    /* Retrieve the number of inputs */
    /* MAY BE USEFUL AT SOME FUTURE TIME */
    fgets( buffer, MAX_BUFFER, fsa_file );
    
    /* Eat the next line */
    fgets( buffer, MAX_BUFFER, fsa_file );
    
    /* For each state modify the fsa struct */
    for ( i = 0; i < fsa_table->fsa[k].number_states; i++ ) {
    
      fgets( buffer, MAX_BUFFER, fsa_file );
  
      current_char = &(buffer[0]);
      
      /* Find the first -, then point to first char after */
      for ( ; (*current_char) != '-'; current_char++ );
      current_char++;
      
      /* Get the number of transitions for the state and allocate space */
      fsa_table->fsa[k].state[i].number_transitions = getnumbertransitions( current_char );
      fsa_table->fsa[k].state[i].transition = malloc( sizeof( transition_struct ) * fsa_table->fsa[k].state[i].number_transitions );
      
      j = 0;
      
      while ( *(current_char) != '-' ) {
      
        getinputdestprob( current_char,
                          &(fsa_table->fsa[k].state[i].transition[j].input),
                          &(fsa_table->fsa[k].state[i].transition[j].destination),
                          &(fsa_table->fsa[k].state[i].transition[j].probability) );
        
        for ( ; ( *(current_char) != ';' ) && ( *(current_char) != '-' ); current_char++ );
        
        if ( *(current_char) == ';' )  current_char++;
        
        j++;
        
      }
      
      current_char++;
      
      /* Copy function name to list of functions */
      if ( ( *current_char != 10 ) || ( *current_char != 32 ) ) {

        trim_right_inplace(current_char);
        fsa_table->fsa[k].state[i].description = malloc( strlen( current_char )+1 );
        strcpy( fsa_table->fsa[k].state[i].description, current_char );
        
      } else {
      
        fsa_table->fsa[k].state[i].description = malloc( strlen( "" ) );
        strcpy( fsa_table->fsa[k].state[i].description, "" );
        
      }
      
      /* Probability amounts are still the same as they were in the file */
      /* Changed here to correct scale                                   */ 
      current_input = fsa_table->fsa[k].state[i].transition[0].input;      
      current_sum = 0.0;
      j = 0;
      
      while ( j < fsa_table->fsa[k].state[i].number_transitions ) {
      
        if ( current_input == fsa_table->fsa[k].state[i].transition[j].input ) {
        
          if ( fsa_table->fsa[k].state[i].transition[j].probability != 100.0 ) {
          
            current_sum += fsa_table->fsa[k].state[i].transition[j].probability;
            fsa_table->fsa[k].state[i].transition[j].probability = current_sum;
            
          }
          
          j++;
          
        } else {
        
          current_input = fsa_table->fsa[k].state[i].transition[j].input;
          current_sum = 0.0;
          
        }
        
      }
      
    }
    
    fgets( buffer, MAX_BUFFER, fsa_file );
    
    /* Set current_state to zero, intially */
    fsa_table->fsa[k].current_state = 0;
    
    k++;
    
  }
  
  return SUCCESS;
}

int delete_fsa ( fsa_struct* fsa ) {
/* PURPOSE:  Delete an fsa from memory  */
/* INPUT:    fsa  FSA to delete         */
/* OUTPUT:   NONE                       */
/* RETURN:   SUCCESS  Indicates success */
/*           FAILURE  Indicates failure */

  int i;

  for ( i = 0; i < fsa->number_states; i++ ) {

    free( fsa->state[i].description );
    free( fsa->state[i].transition );
 
  }

  free( fsa->state );

  free( fsa );

  return SUCCESS;

}

int clear_fsa_table ( fsa_table_struct* fsa_table ) {
/* PURPOSE: Delete all the fsa's from the table */
/* INPUT:   fsa_table  Table to delete from     */
/* OUTPUT:  NONE                                */
/* RETURN:  SUCCESS    Indicates success        */
/*          FAILURE    Indicates failure        */

  int i;

  for ( i = 0; i < fsa_table->number_fsa; i++ ) {

    delete_fsa( &(fsa_table->fsa[i]) );

  }

  free( fsa_table->fsa );

  return SUCCESS;

}

void print_fsa ( fsa_struct *fsa ) {
/* PURPOSE:  Prints out an fsa to screen */
/* INPUT:    fsa  Ptr to fsa to print    */
/* OUPUT:    NONE                        */
/* RETURN:   NONE                        */

  int i,j;
  
  printf( "FSA\n---\n\nCurrent State = %d\nNumber States = %d\n",
          fsa->current_state, fsa->number_states );
          
  for ( i = 0; i < fsa->number_states; i++ ) {
  
    printf( "\n  State %d:\n", i );   // DWP 6-30-99 added subscript 'i' to next line
    printf( "    Description = %s\n", fsa->state[i].description);
    printf( "    Number of transitions = %d\n\n", fsa->state[i].number_transitions );

    for ( j = 0; j < fsa->state[i].number_transitions; j++ ) {

      printf( "    Transition %d:\n", j );
      printf( "      Input = %c\n", fsa->state[i].transition[j].input );
      printf( "      Probability = %f\n", fsa->state[i].transition[j].probability );
      printf( "      Destination = %d\n", fsa->state[i].transition[j].destination );

    }

  }
  
}

void print_fsa_table ( fsa_table_struct *fsa_table ) {
/* PURPOSE:  Prints the entire fsa table to screen */
/* INPUT:    fsa_table  Ptr to fsa table to print  */
/* OUTPUT:   NONE                                  */
/* RETURN:   NONE                                  */

  int i, j, k;
  
  printf( "\nFSA TABLE\n---------\n\nNumber FSA:%d\n\n",
          fsa_table->number_fsa );
          
  for ( i = 0; i < fsa_table->number_fsa; i++ ) {
  
    printf( "FSA %d:\n", i );
    printf( "  Current State = %d\n", fsa_table->fsa[i].current_state );
    printf( "  Number States = %d\n", fsa_table->fsa[i].number_states );
    
    for ( j = 0; j < fsa_table->fsa[i].number_states; j++ ) {
    
      printf( "\n  State %d:\n", j );
      printf( "    Description = %s\n", fsa_table->fsa[i].state[j].description );
      printf( "    Number Transitions = %d\n\n", fsa_table->fsa[i].state[j].number_transitions );
      
      for ( k = 0; k < fsa_table->fsa[i].state[j].number_transitions; k++ ) {
      
        printf( "    Transition %d:\n", k);
        printf( "      Input = %c\n", fsa_table->fsa[i].state[j].transition[k].input);
        printf( "      Probability = %f\n", fsa_table->fsa[i].state[j].transition[k].probability);
        printf( "      Destination = %d\n", fsa_table->fsa[i].state[j].transition[k].destination);
        
      }
      
    }
    
  }
  
}

void change_state ( fsa_struct *fsa, char input ) {
/* PURPOSE:  This function will change the current state of the fsa */
/*           based on the input received                            */
/* INPUT:    fsa    Ptr to fsa who's state is to be changed         */
/*           input  Input received from world                       */
/* OUTPUT:   fsa    Ptr to fsa in new state                         */
/* RETURN:   NONE                                                   */

  int i = 0;
  
  float probability;
  
  /* Get the random number for the overall success / failure prob   */
  float failure = 100.0 * ( (double) rand() / (double) RAND_MAX );

  /* No failure continue normally */
  if ( failure > FAILURE_PERCENTAGE ) {
  
    /* Generate random number for check */
    probability = 100.0 * ( (double) rand() / (double) RAND_MAX );
    
    /* Look thru transition array until correct input is found */
    while ( fsa->state[fsa->current_state].transition[i].input != input )  i++;

    while ( (probability > fsa->state[fsa->current_state].transition[i].probability)
            && (fsa->state[fsa->current_state].transition[i].probability != 100) ) i++;
            
    fsa->current_state = fsa->state[fsa->current_state].transition[i].destination;
   
  } else {

    fsa->current_state = (int) ( fsa->number_states * ( (double) rand() / (double) RAND_MAX ) );
  
  }
} 

int getnumbertransitions ( char *current_char ) {
/* PURPOSE:  The format of a state string is as follows:            */
/*           #-*;*;*-                                               */
/*           Where the *'s are transitions, the total number of     */
/*           transitions is equal to the number of ;'s plus 1       */
/* INPUT:    current_char  Pointer to current string to be read     */
/* OUTPUT:   NONE                                                   */
/* RETURN:   Number of transitions                                  */

  char *tracer = current_char;
  
  int count = 1;
  
  while ( (*tracer) != '-' ) {
  
    if ( (*tracer) == ';' ) count++;
    
    tracer++;
    
  }

  return count;

}

void getinputdestprob ( char *current_char, char *input, 
                        int *destination, float *probability ) {
/* PURPOSE: This function will retrieve from the line pointed to by */
/*          current_char the input symbol, destination, and         */
/*          probability.  Pointer current_char comes pointing to    */
/*          the input symbol and will end up pointing to the to the */
/*          symbol after the destination value.                     */
/* INPUT:   current_char  Pointer to current character to be read   */
/* OUTPUT:  input         Character input symbol                    */
/*          destination   Destination state                         */
/*          probability   Probability associated with this state    */
/* RETURN:  NONE                                                    */

  *(input) = *(current_char);
  
  for ( ; ( *(current_char) != ':'); current_char++ );
  
  current_char++;
  
  *(destination) = atoi( current_char );
  
  /* Check to see if probability present */
  for ( ; ( *(current_char) != ';' ) && ( *(current_char) != '(' ) && ( *(current_char) != '-' ); current_char++ );
  
  if ( *(current_char) == '(' ) {
  
    current_char++;
    *(probability) = atof(current_char);
    
  } else *(probability) = 100;
  
}

int copy_fsa( fsa_struct *source, fsa_struct *destination ) {
/* PURPOSE:  Copy fsa from source to destination     */
/*           Space already allocated for destination */
/* INPUT:    source      Source to copy from         */
/* OUTPUT:   destination Destination to copy to      */
/* RETURN:   SUCCESS     Indicates success           */
/*           FAILURE     Indicates failure           */

  int i;

  /* Copy non-pointer related */
  destination->current_state = source->current_state;
  destination->number_states = source->number_states;
  
  /* Allocate space for structs */
  destination->state = malloc( sizeof( state_struct ) * destination->number_states );
  
  if ( destination->state == NULL ) {
  
    error( "Out of memory\n" );
    return FAILURE;
    
  }
  
  for ( i = 0; i < destination->number_states; i++ ) {
  
    if ( copy_state( &(source->state[i]), &(destination->state[i]) ) ) {
    
      error( "Cannot copy state\n" );
      return FAILURE;
      
    }
    
  }
  
  return SUCCESS;
  
}

int copy_state( state_struct *source, state_struct *destination ) {
/* PURPOSE:  Copy state struct from source to destination */
/*           Space was already allocated                  */
/* INPUT:    source       State struct to copy from       */
/* OUTPUT:   destination  State struct to copy to         */
/* RETURN:   SUCCESS      Indicates success               */
/*           FAILURE      Indicates failure               */

  int i;

  /* Copy non-ptr fields */
  destination->number_transitions = source->number_transitions;
  
  /* Copy string */
  destination->description = malloc( strlen( source->description )+1 );
  
  if ( destination->description == NULL ) {
  
    error( "Out of memory\n" );
    return FAILURE;
    
  }

  strcpy( destination->description, source->description );
  
  /* Allocate space for transitions */
  destination->transition = malloc( sizeof( transition_struct ) * destination->number_transitions );
  
  for ( i = 0; i < destination->number_transitions; i++ ) {
  
    if ( copy_transition( &(source->transition[i]), &(destination->transition[i]) ) ) {
    
      error( "Cannont copy transition\n" );
      return FAILURE;
      
    }
    
  }
  
  return SUCCESS;
  
}

int copy_transition( transition_struct *source, transition_struct *destination ) {
/* PURPOSE:  Copy transition struct from source to destination */
/*           Space already allocated for destination           */
/* INPUT:    source      Source transition struct              */
/* OUTPUT:   destination Destination transition struct         */ 
/* RETURN:   SUCCESS     Indicates success                     */
/*           FAILURE     Indicates failure                     */

  destination->input       = source->input;
  destination->probability = source->probability;
  destination->destination = source->destination;
 
  return SUCCESS;
  
}
