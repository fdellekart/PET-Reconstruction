#include "LookupTable.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(LookupTableTest, TestInitialize) {
  auto LUT = new LookupTable();
  delete LUT;
}