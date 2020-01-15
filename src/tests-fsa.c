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
    fprintf(tmpFsaFile, "1  number of fsa's in the file");
    fprintf(tmpFsaFile, "#  Leader FSA comes first");
    fprintf(tmpFsaFile, "2  number of states in fsa");
    fprintf(tmpFsaFile, "#");
    fprintf(tmpFsaFile, "2  number of input symbols");
    fprintf(tmpFsaFile, "#");
    fprintf(tmpFsaFile, "0-0:0;1:1-wander");
    fprintf(tmpFsaFile, "1-0:0;1:1-follow");
    fprintf(tmpFsaFile, "####");
    rewind(tmpFsaFile);

    fsa_table_struct fsaTable;
    int status = initialize_fsa_table ( &fsaTable, tmpFsaFile);

    assert( status == SUCCESS && "failed to initialize the FSA table");
    assert( fsaTable.number_fsa == 1 && "incorrect number of FSAs found");
}


void run_fsa_tests()
{
    TEST(test_initialize_fsa_simple);
}