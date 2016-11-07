#include <algorithm>
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
    int record_size = NUM_ATTRIBUTES * ATTRIBUTE_SIZE;

    Page page;
    init_fixed_len_page(&page, page_size, record_size);

    int total_records = 0;
    int total_pages = 0;

    char* buf;

    struct timeb t;
    ftime(&t);
    long start_time_in_ms = (t.time * 1000) + t.millitm;

    while (csv_file) {
        std::string line;
        csv_file >> line;

        if (line.size() == 0) {
            // ignore empty lines
            continue;
        }

        // remove all commas from the line
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());

        Record *r = new Record;

        // turn 'line' from string to char*
        fixed_len_read((char*)line.c_str(), record_size, r);

        int slot_index = add_fixed_len_page(&page, r);

        // page is full
        if (slot_index == -1) {

            total_pages++;

            // write page to page_file
            int buf_size = page.page_size * record_size;
            buf = new char[buf_size];

            std::vector<Record> *page_data = page.data;
            for (int i = 0; i < fixed_len_page_capacity(&page); i++) {
                fixed_len_write(&page_data->at(i), buf);
            }

            // flush page to file
            page_file << buf << std::flush;

            // allocate empty page
            init_fixed_len_page(&page, page_size, record_size);
            // recalculate slot index
            slot_index = add_fixed_len_page(&page, r);
        }

        write_fixed_len_page(&page, slot_index, r);

        total_records++;
    }
    // write last page to file if it has records
    if (page.used_slots > 0) {
        total_pages++;

        // write page to page_file
        int buf_size = page.page_size * record_size;
        buf = new char[buf_size];
        std::vector<Record> *page_data = page.data;
        for (int i = 0; i < fixed_len_page_capacity(&page); i++) {
            if (!page_data->at(i).empty()) {
                fixed_len_write(&(page_data->at(i)), buf);
            }
        }

        // flush page to file
        page_file << buf << std::flush;
    }

    ftime(&t);
    long total_run_time = ((t.time * 1000) + t.millitm) - start_time_in_ms;

    csv_file.close();
    page_file.close();

    std::cout << "NUMBER OF RECORDS: " << total_records << "\n";
    std::cout << "NUMBER OF PAGES: " << total_pages << "\n";
    std::cout << "TOTAL TIME: " << total_run_time << " milliseconds\n";
    return 0;
}
