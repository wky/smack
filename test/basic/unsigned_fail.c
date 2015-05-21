#include <smack.h>

// @expect error

int main() {
  int x = 2;
  int y = 3;
  assert(x-y > 0);
  return 0;
}
