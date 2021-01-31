#pragma once

#include <stdio.h>

// this was the initial pass at getting console output for each test.
// each test was going to need to have this as their first line.
// then i got lazy and didn't want to have to remember that.
// thus adding the TEST macro below.
// which of course conflicted with the bit testing macro that was never useda.
// but the test macro wins in this case.
//
// #define ANNOUNCE_TEST() (printf("[TEST] %s\n",__func__))

#define TEST(x) ({puts("[TEST] " #x);x();})

// XXX: later on be cute and figure out somethign like attributes and generate the test runner
