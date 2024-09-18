#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <variant>

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

  /// @brief Get the next element in the stream
  /// @return Tag or event, depending on what the next 32 bits represent
  std::shared_ptr<EventOrTag> get_next();

  /// @brief Set the current position of the stream to a specific point in time
  /// @param time milliseconds since start
  /// @return bool indicating success
  ///
  /// Implements a binary search to find the relevant timetag
  /// because the exact position of the desired time cannot be known
  ///
  /// Stream will be positioned such that the returned element on the get_next
  /// call Is the requested timetag
  ///
  /// CAUTION: FIXME: Currently wouldn't work with a file that has a timetag for
  /// 0 ms or if the inidividual timetags are not exactly 1 ms apart.
  bool seek_time(int32_t time);

protected:
  const char *listmode_file;

private:
  uint32_t current_word;

  /// @brief Call get next until next timetag is encountered
  /// @return Time since start in milliseconds
  ///
  /// Resets the stream so that the next get_next call returns that timetag
  uint32_t get_next_time();
};
