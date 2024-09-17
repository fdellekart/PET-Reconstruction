#include "PETLINKStream.h"
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>

class PETLINKStreamTest : public testing::Test {
protected:
  PETLINKStreamTest()
      : filepath(std::filesystem::temp_directory_path() / "temp_test.bf") {
    uint32_t value;
    testfile.open(filepath, std::ios_base::binary);

    value = 0b10000000000000000000000000000001; // timetag 1 ms
    write_value(value);
    value = 0b01000000000000000000000000000001; // prompt /w bin address 1
    write_value(value);
    value = 0b00000000000000000000000000000001; // delayed /w bin address 1
    write_value(value);

    value = 0b10000000000000000000000000000010; // timetag 2 ms
    write_value(value);
    value = 0b01000000000000000000000000000010; // prompt /w bin address 2
    write_value(value);
    value = 0b00000000000000000000000000000010; // delayed /w bin address 2
    write_value(value);

    testfile.close();
    stream.open(filepath, std::ifstream::in | std::ifstream::binary);
  };
  ~PETLINKStreamTest() {
    std::remove(filepath.c_str());
    stream.close();
  };
  PETLINKStream stream;
  std::filesystem::path filepath;
  std::ofstream testfile;

private:
  void write_value(uint32_t value) {
    testfile.write(reinterpret_cast<const char *>(&value), sizeof(uint32_t));
  };
};

// Demonstrate some basic assertions.
TEST_F(PETLINKStreamTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_TRUE(stream.good());
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
