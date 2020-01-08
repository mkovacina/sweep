#include <string.h>
#include <assert.h>

#include <stdio.h>

#include "string-util.h"

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

void run_string_util_tests()
{
    test_trim_right_inplace_NoModification();
    test_trim_right_inplace_WhitespaceAtEnd();
}