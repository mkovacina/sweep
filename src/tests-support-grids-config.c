#include <assert.h>
#include <string.h>

#include "input-data-source.h"
#include "support-grids-config.h"
#include "testing.h"
#include "trace.h"

// declaring these here because they are "private" meaning I don't want them in a header
// for "people" to be able to use, but I obviously need to declare them to be able to use them.
//
// and yes, this is a duplication of the forward declations in the support-grids-config.c file, but
// those declarations there are a convenience to allow all the implementations to be at the bottom of the
// file and the main logic to stay at the topic
void parseUniformInitialization(UniformInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );
void parseFileInitialization(FileInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );
void parseRandomInitialization(RandomInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );
void parseDistributedInitialization(DistributedInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );


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

void test_parseRandomInitialization()
{
	RandomInitializationParameters parameters;
	InputDataSource source;
	const size_t length = 1024;
	char buffer[length];

	InitializeInputDataSourceFromMemory(&source);
	AddInputData(&source, "1.0");
	AddInputData(&source, "2.0");
	parseRandomInitialization(&parameters, &source, buffer, length);

	const float expectedMinimumValue = 1.0;
	const float expectedMaximumValue = 2.0;

	assert(parameters.MinimumRandomValue == expectedMinimumValue && "unexpected value for MinimumValue");
	assert(parameters.MaximumRandomValue == expectedMaximumValue && "unexpected value for MaximumValue");
}
void test_parseDistributedInitialization()
{
	DistributedInitializationParameters parameters;
	InputDataSource source;
	const size_t length = 1024;
	char buffer[length];

	InitializeInputDataSourceFromMemory(&source);
	AddInputData(&source, "3");
	AddInputData(&source, "4.0");
	AddInputData(&source, "5.0");
	AddInputData(&source, "6.1");
	parseDistributedInitialization(&parameters, &source, buffer, length);

	const int expectedNumberOfValues = 3;
	const float expectedValue_0 = 4.0;
	const float expectedValue_1 = 5.0;
	const float expectedValue_2 = 6.1;

	assert(parameters.NumberOfValues == expectedNumberOfValues && "unexpected value for NumberOfValues");
	assert(parameters.Values[0] == expectedValue_0 && "unexpected value for Value_0");
	assert(parameters.Values[1] == expectedValue_1 && "unexpected value for Value_1");
	assert(parameters.Values[2] == expectedValue_2 && "unexpected value for Value_2");
}
void run_support_grids_config_tests()
{
	TEST(test_parseUniformInitialization);
	TEST(test_parseFileInitialization);
	TEST(test_parseRandomInitialization);
	TEST(test_parseDistributedInitialization);
}
