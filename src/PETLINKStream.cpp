#include <cassert>
#include "PETLINKStream.h"

PETLINKStream::PETLINKStream(const char *listmode_file) : listmode_file(listmode_file)
{
    open(listmode_file);
};

PETLINKStream::~PETLINKStream() {
    close();
};

EventOrTag* PETLINKStream::get_next() {
    read(reinterpret_cast<char *>(&current_word), sizeof(current_word));
    EventOrTag* result = new EventOrTag;
    if (current_word >> 31) {
        result->is_event = false;
        result->value.tag = new Tag(current_word);
    } else {
        result->is_event = true;
        result->value.event = new Event(current_word);
    }
    return result;
};

Event::Event(uint32_t word) {
    assert(!(word >> 31));
    is_prompt = static_cast<bool>(word>>30);
    is_delayed = !is_prompt;
    bin_address = word && 0x1ffffff;
};

Tag::Tag(uint32_t word) {
    assert((word >> 31));
    is_timetag = (word >> 29) == 0x4;
    elapsed_millis = word && 0x01ffffff;
};
