#pragma once
#include <stdint.h>

#define PRINT_PASS() (printf("\n\033[92mPASSED\033[0m"))

#define PRINT_FAIL() (printf("\n\033[91mFAILED\033[0m"))
uint32_t _assert(uint32_t _bool);
void _assert_streq(const char* x, const char* y);
#define ASSERT(_b) (_assert(_b))
#define ASSERT_STREQ(_x, _y) (_assert_streq(_x,_y))
void _print_test_count(void);
#define END_TEST() _print_test_count()

extern uint16_t pass_count;
extern uint16_t test_count;
