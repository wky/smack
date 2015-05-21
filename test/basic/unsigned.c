#include <smack.h>

// @expect verified

int main() {
  unsigned x = 2;
  unsigned y = 3;
  assert(x-y > 0);
  return 0;
}
