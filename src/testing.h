#pragma once

#define ANNOUNCE_TEST() (printf("[TEST] %s\n",__func__))

#define TEST(x) ({puts("[TEST] " #x);x();})