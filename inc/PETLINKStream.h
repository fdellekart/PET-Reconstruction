#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>

#define PETLINK_STREAM_BUFFER_SIZE 10000000000 // 10 GB

/// @brief Coincidence Event
/// Can be either a prompt or delayed event
/// Position is indicated via the bin_address TODO: find out how this relates to
/// actual positions
class Event {
public:
  Event() { Event(0); };
  Event(uint32_t word);
  uint32_t word;
  bool is_prompt;
  bool is_delayed;
  uint32_t bin_address;
};

/// @brief Tags store different metadata within the stream
/// For example timetags store the elapsed time
class Tag {
public:
  Tag() : Tag(0xffffffff) {};
  Tag(uint32_t word);
  uint32_t word;
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
class PETLINKStream : public std::ifstream {
public:
  PETLINKStream() {};
  PETLINKStream(const char *listmode_file);
  PETLINKStream(std::filesystem::path listmode_file)
      : PETLINKStream(listmode_file.c_str()) {};
  ~PETLINKStream();

  struct iterator {
    PETLINKStream *stream;
    std::shared_ptr<EventOrTag> current_element;

  public:
    iterator(PETLINKStream *s = nullptr, bool is_end = false) : stream(s) {
      if (stream && !is_end && *stream) {
        current_element = stream->get_next();
        if (stream->eof())
          stream = nullptr;
      } else {
        stream = nullptr;
      }
    }
    std::shared_ptr<EventOrTag> operator*() const { return current_element; };
    iterator &operator++() {
      if (stream) {
        current_element = stream->get_next();
        if (stream->eof())
          stream = nullptr;
      }
      return *this;
    };

    iterator operator++(int) {
      iterator temp = *this;
      ++(*this);
      return temp;
    };

    bool operator==(const iterator &other) const {
      return stream == other.stream;
    };
    bool operator!=(iterator other) const { return !(*this == other); };
  };

  iterator begin() { return iterator(this); }

  iterator end() { return iterator(this, true); }

  /// @brief Get the next element in the stream
  /// @return Tag or event, depending on what the next 32 bits represent
  std::shared_ptr<EventOrTag> get_next();

protected:
  const char *listmode_file;

private:
  uint32_t current_word;
};
