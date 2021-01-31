#include <string.h>
#include <assert.h>

#include <stdio.h>

#include "string-util.h"

#include "testing.h"

void test_trim_right_inplace_NoModification()
{
    char actual[] = "spaceman";
    const char* expected = "spaceman";
    trim_right_inplace(actual);
    int result = strncmp(actual, expected, strlen(actual));
    assert(result == 0 && "the string was not trimmed correctly");
}

void test_trim_right_inplace_WhitespaceAtEnd()
{
    char actual[] = "spaceman \t\n";
    const char* expected = "spaceman";
    trim_right_inplace(actual);
    int result = strncmp(actual, expected, strlen(actual));
    assert(result == 0 && "the string was not trimmed correctly");
}

void test_trim_comments()
{
	char actual[] = "something  xlkdlg";
	const int length = 16;
	const char* expected = "something";
	trim_comments(actual, length);
    int result = strncmp(actual, expected, strlen(actual));
    assert(result == 0 && "the comment was not trimmed correctly");
}

void test_trim_comments_respect_buffer_size()
{
	char actual[] = "something  xlkdlg";
	const int length = 5;
	const char* expected = "some";
	trim_comments(actual, length);
    int result = strncmp(actual, expected, strlen(actual));
    assert(result == 0 && "the comment was not trimmed correctly");
}

void run_string_util_tests()
{
    TEST(test_trim_right_inplace_NoModification);
    TEST(test_trim_right_inplace_WhitespaceAtEnd);
	TEST(test_trim_comments);
	TEST(test_trim_comments_respect_buffer_size);
}
