/// @brief Class to represent summary of data in listmode file
class DataSummary {
public:
  DataSummary()
      : n_prompts(0), n_delayeds(0), n_singles(0), n_timetags(0),
        tot_entries(0) {};

  unsigned long n_prompts;
  unsigned long n_delayeds;
  unsigned long n_singles;
  unsigned long n_timetags;
  unsigned long tot_entries;
};