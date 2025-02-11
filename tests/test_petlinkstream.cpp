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

    for (uint32_t i = 1; i < 100; i++) {
      value = 0b10000000000000000000000000000000 | i; // timetag i ms
      write_value(value);
      value = 0b01000000000000000000000000000000 | i; // prompt /w bin address i
      write_value(value);
      value =
          0b00000000000000000000000000000000 | i; // delayed /w bin address i
      write_value(value);
    }

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

// Test if get_next properly parses individual entries
TEST_F(PETLINKStreamTest, TestGetNext) {
  ASSERT_TRUE(stream.good());

  EventOrTag element;

  element = stream.get_next();
  ASSERT_FALSE(element.is_event);
  EXPECT_TRUE(element.tag.is_timetag);
  EXPECT_EQ(element.tag.elapsed_millis, 1);

  element = stream.get_next();
  ASSERT_TRUE(element.is_event);
  EXPECT_TRUE(element.event.is_prompt);
  EXPECT_FALSE(element.event.is_delayed);
  EXPECT_EQ(element.event.bin_address, 1);

  element = stream.get_next();
  ASSERT_TRUE(element.is_event);
  EXPECT_TRUE(element.event.is_delayed);
  EXPECT_FALSE(element.event.is_prompt);
  EXPECT_EQ(element.event.bin_address, 1);
}

// Test if seek_time moves the stream to the proper timepoint
TEST_F(PETLINKStreamTest, TestSeekTime) {
  stream.seek_time(50);
  auto element = stream.get_next();
  EXPECT_FALSE(element.is_event);
  EXPECT_TRUE(element.tag.is_timetag);
  EXPECT_EQ(element.tag.elapsed_millis, 50);

  stream.seek_time(73);
  element = stream.get_next();
  EXPECT_FALSE(element.is_event);
  EXPECT_TRUE(element.tag.is_timetag);
  EXPECT_EQ(element.tag.elapsed_millis, 73);
}

// Test if the iterator works properly
// Checks that consecutive events never match
// to ensure that last element is not returned twice
//
// This could fail unintended if two identical events
// are put after each other in the stream fixture
TEST_F(PETLINKStreamTest, TestIterator) {
  EventOrTag last;
  bool is_first_iter = true;

  for (EventOrTag element : stream) {
    if (!is_first_iter) {
      ASSERT_NE(last.event.word, element.event.word);
    }
    last = element;
    is_first_iter = false;
  }
}
