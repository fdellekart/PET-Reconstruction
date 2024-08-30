#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    std::ifstream input_stream;

    input_stream.open("test.txt", std::ifstream::in);

    char c = input_stream.get();

    while (input_stream.good()) {
        std::cout << c;
        c = input_stream.get();
    }

    input_stream.close();

    return 0;
}