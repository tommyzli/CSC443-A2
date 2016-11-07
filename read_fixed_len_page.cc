#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/timeb.h>
#include "library.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Error, usage must be:\n";
        std::cout << "./read_fixed_len_page <page_file> <page_size>\n";
        return 1;
    }

    bool print_records = true;
    if (argc == 4 && strcmp(argv[3], "--no-records") == 0) {
        print_records = false;
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

    struct timeb t;
    ftime(&t);
    long start_time_in_ms = (t.time * 1000) + t.millitm;

    char buf[page_size];
    while (page_file.read(buf, page_size)) {
        // clear the page
        init_fixed_len_page(&page, page_size, record_size);

        for (int i = 0; i < fixed_len_page_capacity(&page); ++i) {
            Record *r = new Record;

            fixed_len_read(buf + (i * record_size), record_size, r);

            write_fixed_len_page(&page, i, r);

            // print entries in record
            if (print_records) {
                for (Record::iterator it = r->begin(); it != r->end(); ++it) {
                    std::cout << *it << ",";
                }
                std::cout << "\n";
            }
        }
    }

    ftime(&t);
    long total_run_time = ((t.time * 1000) + t.millitm) - start_time_in_ms;

    page_file.close();

    std::cout << "TOTAL TIME: " << total_run_time << " milliseconds\n";

    return 0;
}
