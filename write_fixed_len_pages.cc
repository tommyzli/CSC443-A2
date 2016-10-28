#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/timeb.h>

#include "library.h"


int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Error, usage must be:\n";
        std::cout << "./write_fixed_len_pages <csv_file> <page_file> <page_size>\n";
        return 1;
    }

    std::ifstream csv_file;
    csv_file.open(argv[1]);
    if (!csv_file) {
        std::cout << "Error, could not find file " << argv[1] << "\n";
    }

    std::ofstream page_file;
    page_file.open(argv[2]);
    if (!page_file) {
        std::cout << "Error, could not find file " << argv[2] << "\n";
    }

    int page_size = atoi(argv[3]);

    int total_records = 0;
    int total_pages = 0;

    struct timeb t;

    while (csv_file) {
        std::string line;
        csv_file >> line;
    }

    csv_file.close();
    page_file.close();
    return 0;
}
