#include <stdio.h>
#include <string.h>

#include <_assert.h>

uint16_t pass_count = 0u;
uint16_t test_count = 0u;

uint32_t inline _assert(uint32_t _bool) {
  if (_bool) {
    PRINT_PASS();
    ++pass_count;
  }
  else {
    PRINT_FAIL();
    printf(" -- %d", _bool);
  }
  ++test_count;
  return _bool;
}
void inline _assert_streq(const char* x, const char* y) {
  if (strcmp(x,y) == 0) {
    PRINT_PASS();
    ++pass_count;
  }
  else {
    PRINT_FAIL();
    printf(" -- %s != %s", x,y);
  }
  ++test_count;
}
void inline _print_test_count(void) {
  printf("\n\033[1mPassed %3u / %3u\033[0m", pass_count, test_count);
}

#define END_TEST() _print_test_count()
