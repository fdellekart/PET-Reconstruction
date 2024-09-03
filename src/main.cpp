#include <memory>
#include <iostream>
#include <unordered_set>
#include <cstdint>
#include "PETLINKStream.h"
#include "DataSummary.h"

#define ITIME 1000 // Integration time (interval between two time tags?)

int main(int argc, char **argv)
{
    auto filename = "/home/recon/Documents/Thesis-Dellekart/SIRF-STIR-PET-Reconstruction/input/1.3.12.2.1107.5.2.38.51035.30000017121607191856600000009.bf";
    PETLINKStream input_stream(filename);

    int time_from_start; // Time since acquisition start in seconds
    std::unordered_set<uint32_t> *unique_bin_addresses = new std::unordered_set<uint32_t>;
    DataSummary summary;

    while (input_stream.good())
    {
        std::shared_ptr<EventOrTag> next = input_stream.get_next();

        if (next->is_event)
        {
            if (next->event.is_prompt)
            {
                summary.n_prompts++;
                unique_bin_addresses->insert(next->event.bin_address);
            }
            else
            { // delayeds
                summary.n_delayeds++;
            };
        }
        else
        {
            if (next->tag.is_timetag)
            {
                summary.n_timetags++;
                time_from_start = next->tag.elapsed_millis / ITIME;
            }
        }

        summary.tot_entries++;
        if ((summary.n_timetags % 10000) == 0)
        {
            std::cout << "-------------------------------------" << std::endl;
            std::cout << "Time: " << time_from_start << std::endl;
            std::cout << "Prompts: " << summary.n_prompts << std::endl;
            std::cout << "Delayeds: " << summary.n_delayeds << std::endl;
            std::cout << "Singles: " << summary.n_singles << std::endl;
            std::cout << "Timetags: " << summary.n_timetags << std::endl;
            std::cout << "Iterations: " << summary.tot_entries << std::endl;
        }
    }

    for (uint32_t address : *unique_bin_addresses)
    {
        std::cout << address << std::endl;
    };

    std::cout << "Found " << unique_bin_addresses->size() << " unique bin addresses in file." << std::endl;

    return 0;
}