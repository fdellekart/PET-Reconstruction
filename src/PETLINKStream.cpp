#include "PETLINKStream.h"

PETLINKStream::PETLINKStream(const char *listmode_file) : listmode_file(listmode_file)
{
    open(listmode_file);
};

PETLINKStream::~PETLINKStream() {
    close();
};
