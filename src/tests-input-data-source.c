#include <assert.h>
#include <stdlib.h>

#include "input-data-source.h"
#include "testing.h"

void test_initialize_from_memory()
{
	InputDataSource source;

	InitializeInputDataSourceFromMemory(&source);

	AddInputData(&source, "sandy is on the move");
	AddInputData(&source, "what stuff on the floor");
	AddInputData(&source, "oh that stuff on the floor");

	assert(source.Type == MemorySourceType && "incorrect Type");
	assert(source.NumberOfLines == 3 && "incorrect NumberOfLines");
}

void run_input_data_source_tests()
{
	TEST(test_initialize_from_memory);
}
