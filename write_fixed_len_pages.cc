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

    int num_free = fixed_len_page_freeslots(&page);
    std::cout << "new page has " << num_free << " free slots\n";

    int total_records = 0;
    int total_pages = 0;

    char* buf;

    // struct timeb t;

    while (csv_file) {
        std::string line;
        csv_file >> line;

        // remove all commas from the line
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());

        Record *r = new Record;

        // turn 'line' from string to char*
        fixed_len_read((char*)line.c_str(), record_size, r);

        /*
        // checking if fixed_len_read actually worked
        int i = 0;
        for (Record::iterator it = r->begin(); it != r->end(); ++it) {
            std::cout << *it << " ";
            std::cout << line.substr(i, 10);
            i = i + 10;
            std::cout << "\n";
        }
        std::cout << "\n";
        */

        int slot_index = add_fixed_len_page(&page, r);
        std::cout << "index for new record: " << slot_index << "\n";
        if (slot_index == -1) {
            total_pages++;

            // write page to page_file
            int buf_size = page.page_size * record_size;
            buf = new char[buf_size];
            std::cout << "writing page to outut buffer\n";
            for (int i = 0; i < page.page_size; i++) {
                fixed_len_write(&(page.data)->at(i), buf);
            }
            std::cout << "writing to file\n";
            page_file << buf;
            std::cout << "wrote to file\n";

            // allocate empty page
            init_fixed_len_page(&page, page_size, record_size);
            // recalculate slot index
            slot_index = add_fixed_len_page(&page, r);
        }

        write_fixed_len_page(&page, slot_index, r);

        total_records++;
    }

    // write last page to file
    if (fixed_len_page_freeslots(&page) > 0) {
        total_pages++;

        // write page to page_file
        int buf_size = page.page_size * record_size;
        buf = new char[buf_size];
        for (int i = 0; i < page.page_size; i++) {
            fixed_len_write(&(page.data)->at(i), buf);
        }
        std::cout << "writing to file\n";
        page_file << buf;
        std::cout << "wrote to file\n";

        // allocate new page
        init_fixed_len_page(&page, page_size, record_size);
    }

    csv_file.close();
    page_file.close();

    std::cout << "NUMBER OF RECORDS: " << total_records << "\n";
    std::cout << "NUMBER OF PAGES: " << total_pages << "\n";
    std::cout << "TOTAL TIME: " << " milliseconds\n";
    return 0;
}
