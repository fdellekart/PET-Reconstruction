#include <memory>
#include <iostream>
#include <set>
#include <cstdint>
#include "PETLINKStream.h"

#define ITIME 1000  // Integration time (interval between two time tags?)

int main(int argc, char **argv) {
    auto filename = "/home/recon/Documents/Thesis-Dellekart/SIRF-STIR-PET-Reconstruction/input/1.3.12.2.1107.5.2.38.51035.30000017121607191856600000009.bf";
    PETLINKStream input_stream(filename);

    int time_from_start;  // Time since acquisition start in seconds
    std::set<uint32_t>* unique_bin_addresses = new std::set<uint32_t>;
    unsigned long n_prompts = 0;
    unsigned long n_delayeds = 0;
    unsigned long n_singles = 0;
    unsigned long n_timetags = 0;
    unsigned long n = 0;

    while (input_stream.good()) {
        std::shared_ptr<EventOrTag> next = input_stream.get_next();
        
        if (next->is_event) {
            std::shared_ptr<Event> event = next->value;
            if (event->is_prompt) {
                n_prompts++;
                unique_bin_addresses->insert(event->bin_address);
            } else { // delayeds
                n_delayeds++;
            };
        }
        else {
            std::shared_ptr<Tag> tag = next->value;
            if (tag->is_timetag) {
                n_timetags++;
                time_from_start = tag->elapsed_millis / ITIME;
            }
        }

        n++;
        if ((n_timetags % 10000) == 0) {
            std::cout << "-------------------------------------" << std::endl;
            std::cout << "Time: " << time_from_start << std::endl;
            std::cout << "Prompts: " << n_prompts << std::endl;
            std::cout << "Delayeds: " << n_delayeds << std::endl;
            std::cout << "Singles: " << n_singles << std::endl;
            std::cout << "Timetags: " << n_timetags << std::endl;
            std::cout << "Iterations: " << n << std::endl;
        }
    }

    for (uint32_t address : *unique_bin_addresses) {
        std::cout << address << std::endl;
    };

    std::cout << "Found " << unique_bin_addresses->size() << " unique bin addresses in file." << std::endl;

    return 0;
}