#include "PETLINKStream.h"
#include "Constants.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <variant>

PETLINKStream::PETLINKStream(const char *listmode_file)
    : listmode_file(listmode_file) {
  open(listmode_file, std::ifstream::in | std::ifstream::binary);
  // TODO: Does it really make sense to open the file in the constructor?
};

PETLINKStream::~PETLINKStream() { close(); };

std::variant<Tag, Event> PETLINKStream::get_next() {
  read(reinterpret_cast<char *>(&current_word), sizeof(current_word));

  if (current_word >> 31) {
    return Tag(current_word);
  } else {
    return Event(current_word);
  }
};

bool PETLINKStream::seek_time(std::chrono::milliseconds time) {
  this->seekg(0);
  std::chrono::milliseconds next_time = this->get_next_time();

  assert(time >= next_time);

  if (next_time == time)
    return true;
  std::streampos smaller_position = this->tellg();

  this->seekg(0, ios_base::end);
  std::streampos bigger_position = this->tellg();
  std::streampos middle_position;

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

std::chrono::milliseconds PETLINKStream::get_next_time() {
  std::variant<Tag, Event> next;
  bool found = false;
  while (!found && !this->eof()) {
    next = this->get_next();
    if (std::holds_alternative<Tag>(next) && std::get<Tag>(next).is_timetag)
      found = true;
  }

  if (this->eof()) {
    throw std::runtime_error("EOF reached when searching for next time tag.");
  };

  this->seekg(this->tellg() - sizeof(int32_t));
  return std::get<Tag>(next).time;
};

Event::Event(uint32_t word) : word(word) {
  assert(!(word >> 31));
  is_prompt = static_cast<bool>(word >> 30);
  is_delayed = !is_prompt;
  bin_address = word & 0x3fffffff;
};

LOR Event::get_lor() {
  LOR idxs;
  int32_t rest;

  idxs.tang_idx = bin_address % NSBINS;
  rest = bin_address / NSBINS;
  idxs.angle_idx = rest % NSANGLES; // view num
  rest = rest / NSANGLES;
  idxs.proj_idx = rest % NSINOS;

  return idxs;
};

Tag::Tag(uint32_t word) : word(word) {
  assert((word >> 31));
  is_timetag = (word >> 29) == 0x4;
  time = std::chrono::milliseconds(word & 0x01ffffff);
};
