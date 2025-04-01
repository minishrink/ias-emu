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
    printf(" -- %x", _bool);
  }
  ++test_count;
  return _bool;
}
void inline _assert_streq(const char *x, const char *y) {
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
void inline _assert_uint64(uint64_t x, uint64_t y) {
  if (x == y) {
    PRINT_PASS();
    ++pass_count;
  }
  else {
    PRINT_FAIL();
    printf(" -- %10lx != %10lx", x,y);
  }
  ++test_count;
}
void inline _assert_uint32(uint32_t x, uint32_t y) {
  if (x == y) {
    PRINT_PASS();
    ++pass_count;
  }
  else {
    PRINT_FAIL();
    printf(" -- %10x != %10x", x,y);
  }
  ++test_count;
}
void inline _assert_uint16(uint16_t x, uint16_t y) {
  if (x==y) {
    PRINT_PASS();
    ++pass_count;
  }
  else {
    PRINT_FAIL();
    printf(" -- %10x != %10x", x, y);
  }
  ++test_count;
}
void inline _assert_uint8(uint8_t x, uint8_t y) {
  if (x==y) {
    PRINT_PASS();
    ++pass_count;
  }
  else {
    PRINT_FAIL();
    printf(" -- %10x != %10x", x, y);
  }
  ++test_count;
}
void inline _print_test_count(void) {
  printf("\n\033[1mPassed %3u / %3u\033[0m", pass_count, test_count);
}
void inline _new_test(const char *_name) {
  if (test_count > 0) {
    _print_test_count();
    test_count = 0u;
    pass_count = 0u;
  }
  printf("\n\n\033[1m--- %s ---\033[0m", _name);
}
