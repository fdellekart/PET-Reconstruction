#include<fstream>
#include<memory>
#include<cstdint>
#include<variant>

class Event {
    public:
        Event() {Event(0);};
        Event(uint32_t word);
        bool is_prompt;
        bool is_delayed;
        uint32_t bin_address;
};
class Tag{
    public:
        Tag(): Tag(0xffffffff) {};
        Tag(uint32_t word);
        bool is_timetag;
        uint32_t elapsed_millis;
};

struct EventOrTag {
    bool is_event;
    Event event;
    Tag tag;
};

class PETLINKStream: public std::ifstream {
    public:
        PETLINKStream(const char* listmode_file);
        ~PETLINKStream();
        std::shared_ptr<EventOrTag> get_next();
    protected:
        const char* listmode_file;
    private:
        uint32_t current_word;
};
