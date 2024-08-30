#include <iostream>
#include <fstream>

#define ITIME 1000  // Integration time (interval between two time tags?)

int main(int argc, char **argv) {
    std::ifstream input_stream;

    auto filename = "/home/recon/Documents/Thesis-Dellekart/SIRF-STIR-PET-Reconstruction/input/1.3.12.2.1107.5.2.38.51035.30000017121607191856600000009.bf";

    input_stream.open(filename, std::ifstream::in);

    bool is_prompt; //prompt bit
    int tag = 0;
    int time_from_start;  // Time since acquisition start in seconds
    int itagu;
    int word;
    unsigned long n_prompts = 0;
    unsigned long n_delayeds = 0;
    unsigned long n_singles = 0;
    unsigned long n_timetags = 0;
    unsigned long n = 0;

    while (input_stream.good()) {
        input_stream.read(reinterpret_cast<char *>(&word), sizeof(word));
        if (word > 0) {
            is_prompt = (word >> 30);
            if (is_prompt) {
                n_prompts++;
            } else { // delayeds
                n_delayeds++;
            };
        }
        else if ((word >> 30) == -3) { // single
            n_singles++;
        }
        else if ((word >> 29) == -4) { // time tag
            time_from_start = (word & 0x1fffffff) / ITIME;  // assuming that the tag is every 1ms
            itagu = word - time_from_start * ITIME;
            n_timetags++;
        }
        n++;
        if ((n_timetags % 100000) == 0) {
            std::cout << "-------------------------------------" << std::endl;
            std::cout << "Time: " << time_from_start << std::endl;
            std::cout << "Prompts: " << n_prompts << std::endl;
            std::cout << "Delayeds: " << n_delayeds << std::endl;
            std::cout << "Singles: " << n_singles << std::endl;
            std::cout << "Timetags: " << n_timetags << std::endl;
            std::cout << "Iterations: " << n << std::endl;
        }
    }

    input_stream.close();

    return 0;
}