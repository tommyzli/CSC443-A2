#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/timeb.h>
#include "library.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Error, usage must be:\n";
        std::cout << "./read_fixed_len_page <page_file> <page_size>\n";
        return 1;
    }

    std::ifstream page_file;
    page_file.open(argv[1]);
    if (!page_file) {
        std::cout << "Error, could not find file " << argv[1] << "\n";
        return 0;
    }

    int page_size = atoi(argv[2]);

    int record_size = NUM_ATTRIBUTES * ATTRIBUTE_SIZE;

    Page page;

    int total_records = 0;
    int total_pages = 0;

    struct timeb t;
    ftime(&t);
    long start_time_in_ms = (t.time * 1000) + t.millitm;

    char buf[page_size];
    while (page_file.read(buf, page_size)) {
        int amount_read = page_file.gcount();
        if (amount_read == 0) {
            break;
        }

        init_fixed_len_page(&page, page_size, record_size);

        for (int i = 0; i < fixed_len_page_capacity(&page); ++i) {
            Record *r = new Record;

            fixed_len_read(buf + (i * record_size), record_size, r);

            write_fixed_len_page(&page, i, r);

            // print entries in record here

            total_records++;
        }

        total_pages++;
    }

    ftime(&t);
    long total_run_time = ((t.time * 1000) + t.millitm) - start_time_in_ms;

    page_file.close();

    std::cout << "NUMBER OF RECORDS: " << total_records << "\n";
    std::cout << "NUMBER OF PAGES: " << total_pages << "\n";
    std::cout << "TOTAL TIME: " << total_run_time << " milliseconds\n";

    return 0;
}
