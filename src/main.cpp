#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    std::ifstream input_stream;

    auto filename = "/home/recon/Documents/Thesis-Dellekart/SIRF-STIR-PET-Reconstruction/input/1.3.12.2.1107.5.2.38.51035.30000017121607191856600000009.bf";

    input_stream.open(filename, std::ifstream::in);

    int tag = 0;
    int value;
    
    while (input_stream.good()) {
        input_stream.read(reinterpret_cast<char *>(&value), sizeof(value));
        std::cout << value << std::endl;
    }

    input_stream.close();

    return 0;
}