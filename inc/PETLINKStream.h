#include<fstream>
#include<cstdint>

class Event {
    public:
        Event(uint32_t word);
        bool is_prompt;
        bool is_delayed;
        uint32_t bin_address;
};
class Tag{
    public:
        Tag(uint32_t word);
        bool is_timetag;
        uint32_t elapsed_millis;
};

struct EventOrTag {
    bool is_event;
    union {
        Event* event;
        Tag* tag;
        operator Event* () const {return event;}
        operator Tag* () const {return tag;}
    } value;
};

class PETLINKStream: public std::ifstream {
    public:
        PETLINKStream(const char* listmode_file);
        ~PETLINKStream();
        EventOrTag* get_next();
    protected:
        const char* listmode_file;
    private:
        uint32_t current_word;
};
