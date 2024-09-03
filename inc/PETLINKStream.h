#include<fstream>

class Event {};
class Tag{};

struct EventOrTag {
    bool is_event;
    union {
        Event event;
        Tag tag;
    } value;
};

class PETLINKStream: public std::ifstream {
    public:
        PETLINKStream(const char* listmode_file);
        ~PETLINKStream();
        EventOrTag get_next();
    protected:
        const char* listmode_file;
};
