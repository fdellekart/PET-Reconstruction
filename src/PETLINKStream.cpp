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
