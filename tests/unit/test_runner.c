#include "cutest.h"

void test_fail()
{
  TEST_CHECK(1 == 2);
}

TEST_LIST = {
  { "first-test", test_fail },
  { 0 }
};
