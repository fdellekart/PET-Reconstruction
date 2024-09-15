#include<fstream>
#include<memory>
#include<cstdint>
#include<variant>

#define PETLINK_STREAM_BUFFER_SIZE 10000000000 // 10 GB

/// @brief Coincidence Event
/// Can be either a prompt or delayed event
/// Position is indicated via the bin_address TODO: find out how this relates to actual positions
class Event {
    public:
        Event() {Event(0);};
        Event(uint32_t word);
        bool is_prompt;
        bool is_delayed;
        uint32_t bin_address;
};

/// @brief Tags store different metadata within the stream
/// For example timetags store the elapsed time
class Tag{
    public:
        Tag(): Tag(0xffffffff) {};
        Tag(uint32_t word);
        bool is_timetag;
        uint32_t elapsed_millis;
};

/// @brief Wrapper to return either event or tag
/// Can have either tag or event set but not both
struct EventOrTag {
    bool is_event;
    Event event;
    Tag tag;
};

/// @brief Input filestream to read listmode files in 32 bit PETLINK format
class PETLINKStream: public std::ifstream {
    public:
        PETLINKStream(const char* listmode_file);
        ~PETLINKStream();
        /// @brief Get the next element in the stream
        /// @return Tag or event, depending on what the next 32 bits represent
        std::shared_ptr<EventOrTag> get_next();
    protected:
        const char* listmode_file;
    private:
        uint32_t current_word;
};
