#include <assert.h>
#include <string.h>

#include "input-data-source.h"
#include "support-grids-config.h"
#include "testing.h"
#include "trace.h"

void parseUniformInitialization(UniformInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );
void parseFileInitialization(FileInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );

void test_parseUniformInitialization()
{
	UniformInitializationParameters parameters;
	InputDataSource source;
	const size_t length = 1024;
	char buffer[length];

	InitializeInputDataSourceFromMemory(&source);
	AddInputData(&source, "3141");
	parseUniformInitialization(&parameters, &source, buffer, length);

	// XXX: consider if methods that take a buffer should specify a minimum length
	// if they don't and someone passes in a small buffer, these methods either need
	// to know how to handle that or may exhibit "odd" behavior
	
	const int expected = 3141;
	assert(parameters.InitializationValue == expected && "unexpected value for InitializationValue");
}

void test_parseFileInitialization()
{
	FileInitializationParameters parameters;
	InputDataSource source;
	const size_t length = 1024;
	char buffer[length];

	InitializeInputDataSourceFromMemory(&source);
	AddInputData(&source, "data.dat");
	parseFileInitialization(&parameters, &source, buffer, length);

	// XXX: consider if methods that take a buffer should specify a minimum length
	// if they don't and someone passes in a small buffer, these methods either need
	// to know how to handle that or may exhibit "odd" behavior
	
	const char* expected = "data.dat";
	assert(strncmp(parameters.FilePath,expected,strlen(expected)) == 0 && "unexpected value for FilePath");
}
void run_support_grids_config_tests()
{
	TEST(test_parseUniformInitialization);
	TEST(test_parseFileInitialization);
}
