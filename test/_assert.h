#pragma once
#include <stdint.h>

#define PRINT_PASS() (printf("\n\033[92mPASSED\033[0m"))
#define PRINT_FAIL() (printf("\n\033[91mFAILED\033[0m"))

uint32_t _assert(uint32_t _bool);
void     _assert_streq(const char *x, const char *y);
void     _assert_uinteq(uint64_t x, uint64_t y);
void     _assert_uint32(uint32_t x, uint32_t y);
void     _assert_uint8(uint8_t x, uint8_t y);
void     _print_test_count(void);
void     _new_test(const char *name);

#define ASSERT(_b) (_assert(_b))
#define ASSERT_STREQ(_x, _y) (_assert_streq(_x,_y))
#define ASSERT_UINTEQ(_x, _y) (_assert_uinteq(_x,_y))
#define ASSERT_UINT8(_x, _y) (_assert_uint8(_x,_y))
#define ASSERT_UINT32(_x, _y) (_assert_uint32(_x,_y))

#define START_TEST(_name) _new_test(_name)
#define END_TEST() (_print_test_count())

extern uint16_t pass_count;
extern uint16_t test_count;
