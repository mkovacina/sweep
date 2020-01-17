#include <stdio.h>
#include <assert.h>

#include "fsa.h"
#include "useful.h"

#include "testing.h"

void test_initialize_fsa_simple()
{
    ANNOUNCE_TEST();

    FILE* tmpFsaFile = tmpfile();

    // sources from all-follow.fsa
    fprintf(tmpFsaFile, "1  number of fsa's in the file\n");
    fprintf(tmpFsaFile, "#  Leader FSA comes first\n");
    fprintf(tmpFsaFile, "2  number of states in fsa\n");
    fprintf(tmpFsaFile, "#\n");
    fprintf(tmpFsaFile, "2  number of input symbols\n");
    fprintf(tmpFsaFile, "#\n");
    fprintf(tmpFsaFile, "0-0:0;1:1-wander\r\n");
    fprintf(tmpFsaFile, "1-0:0;1:1-follow\r\n");
    fprintf(tmpFsaFile, "####\n");
    rewind(tmpFsaFile);

    fsa_table_struct fsaTable;
    int status = initialize_fsa_table ( &fsaTable, tmpFsaFile);

    assert( status == SUCCESS && "failed to initialize the FSA table");
    assert( fsaTable.number_fsa == 1 && "incorrect number of FSAs found");

    fsa_struct *fsa = &(fsaTable.fsa[0]);

    assert( fsa->current_state == 0 && "incorrect current FSA state" );
    assert( fsa->number_states == 2 && "incorrect number of FSA states");


    state_struct* state0 = &fsa->state[0];
    state_struct* state1 = &fsa->state[1];

    assert( state0->number_transitions == 2 && "incorrect number of transitions");
    assert( strncmp(state0->description,"wander",6) == 0 && "incorrect state name");
    assert( strlen(state0->description)==6 && "incorrect state name length");
    //transition_struct* transition_0_0 = state1->transition[0];
    //transition_struct* transition_0_1 = state1->transition[1];

    //assert( transition_0_0->input)
}


void run_fsa_tests()
{
    TEST(test_initialize_fsa_simple);
}