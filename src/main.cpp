#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    std::ifstream input_stream;

    auto filename = "/home/recon/Documents/Thesis-Dellekart/SIRF-STIR-PET-Reconstruction/input/1.3.12.2.1107.5.2.38.51035.30000017121607191856600000009.bf";

    input_stream.open(filename, std::ifstream::in);

    int tag = 0;
    int time_from_start;  // Time since acquisition start in seconds
    int itagu;
    int word;
    int n = 0;

    while (input_stream.good()) {
        input_stream.read(reinterpret_cast<char *>(&word), sizeof(word));
        if ((word >> 29) == -4) {
            time_from_start = (word & 0x1fffffff) / 1000;
            itagu = word - time_from_start * 1000;
            std::cout << word << std::endl;
        }
        n++;
    }

    input_stream.close();

    return 0;
}