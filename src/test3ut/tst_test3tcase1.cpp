
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "util.h"

using namespace testing;

class UtilTests : public ::testing::Test
{
protected:
  UtilTests() { };
  ~UtilTests() override { };
};

TEST_F(UtilTests, test3tcase1)
{
  EXPECT_EQ(1, 1);
  ASSERT_THAT(0, Eq(0));
}

TEST_F(UtilTests,mult)
{
  int r = func1(3,5);
  EXPECT_EQ(15,r);
}
