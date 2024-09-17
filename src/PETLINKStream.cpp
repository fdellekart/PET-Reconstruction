#include "PETLINKStream.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

PETLINKStream::PETLINKStream(const char *listmode_file)
    : listmode_file(listmode_file) {
  char *buffer = new char[PETLINK_STREAM_BUFFER_SIZE]();
  this->rdbuf()->pubsetbuf(buffer, PETLINK_STREAM_BUFFER_SIZE);
  open(listmode_file, std::ifstream::in | std::ifstream::binary);
  // TODO: Does it really make sense to open the file in the constructor?
};

PETLINKStream::~PETLINKStream() { close(); };

std::shared_ptr<EventOrTag> PETLINKStream::get_next() {
  read(reinterpret_cast<char *>(&current_word), sizeof(current_word));
  std::shared_ptr<EventOrTag> result = std::make_shared<EventOrTag>();
  if (current_word >> 31) {
    result->is_event = false;
    result->tag = Tag(current_word);
  } else {
    result->is_event = true;
    result->event = Event(current_word);
  }
  return result;
};

bool PETLINKStream::seek_time(int32_t time) {
  uint32_t next_time = this->get_next_time();
  std::streampos smaller_position = this->tellg();

  this->seekg(0, ios_base::end);
  std::streampos bigger_position = this->tellg();
  std::streampos middle_position;
  std::shared_ptr<EventOrTag> smaller_time;
  std::shared_ptr<EventOrTag> bigger_time;

  bool success = false;

  while (!success) {
    middle_position = (bigger_position + smaller_position) / 2;
    middle_position = (middle_position / 4) * 4; // Round to next multiple of 4
    this->seekg(middle_position);
    next_time = this->get_next_time();
    if (next_time == time)
      success = true;
    if (next_time <= time)
      smaller_position = this->tellg();
    if (next_time >= time)
      bigger_position = this->tellg();
  }

  return success;
};

uint32_t PETLINKStream::get_next_time() {
  uint32_t time = 0;
  while (!time && !this->eof()) {
    auto next = this->get_next();
    if (!next->is_event && next->tag.is_timetag)
      time = next->tag.elapsed_millis;
    if (this->eof()) {
      throw std::runtime_error("Encountered EOF in PETLINK binary search. This "
                               "shouldn't be possible");
    };
  }
  this->seekg(this->tellg() - sizeof(int32_t));
  return time;
};

Event::Event(uint32_t word) : word(word) {
  assert(!(word >> 31));
  is_prompt = static_cast<bool>(word >> 30);
  is_delayed = !is_prompt;
  bin_address = word & 0x1ffffff;
};

Tag::Tag(uint32_t word) : word(word) {
  assert((word >> 31));
  is_timetag = (word >> 29) == 0x4;
  elapsed_millis = word & 0x01ffffff;
};
