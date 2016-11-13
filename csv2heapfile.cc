#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/timeb.h>
#include "library.h"

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Error, usage must be:\n";
        std::cout << "./csv2heapfile <csv_file> <heapfile> <page_size>\n";
        return 1;
    }
/*    
    bool show_output = true;
    if (argc == 5 && strcmp(argv[4], "--no-output") == 0) {
        show_output = false;
    }
*/
    // open csv file
    std::ifstream csv_file;
    csv_file.open(argv[1]);
    if (!csv_file) {
        std::cout << "Error, could not find file " << argv[1] << "\n";
        return 1;
    }    

    // initialize page
    int page_size = atoi(argv[3]);
    int record_size = NUM_ATTRIBUTES * ATTRIBUTE_SIZE;

    Page page;
    init_fixed_len_page(&page, page_size, record_size);

/*
    struct timeb t;
    ftime(&t);
    long start_time_in_ms = (t.time * 1000) + t.millitm;
*/

    // initialize heapfile
    Heapfile *heap = new Heapfile();
    FILE* heap_file = fopen(argv[2], "w+b");
    if (!heap_file) {
	std::cout << "Error, could not find file " << argv[2] << "\n";
        return 1;
    }

    init_heapfile(heap, atoi(argv[3]), heap_file);
    
    PageID pageID = alloc_page(heap);
    read_page(heap, pageID, &page);

    // create buffer to read csv file
    char* buf;
    
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

        // turn 'line' from string to char*, and read the values into r
        fixed_len_read((char*)line.c_str(), record_size, r);

        int slot_index = add_fixed_len_page(&page, r);

	if (slot_index == -1) {  // page is full

            // write page to page_file
            int buf_size = page.page_size * record_size;
            buf = new char[buf_size];

            std::vector<Record> *page_data = page.data;
            for (int i = 0; i < fixed_len_page_capacity(&page); i++) {
                fixed_len_write(&page_data->at(i), buf);
            }
	    
	    // write page to heap (disk)
	    write_page(&page, heap, pageID);

            // allocate new page
            pageID = alloc_page(heap);
            
	    // read the new page
	    read_page(heap, pageID, &page);

	    // recalculate slot index
            slot_index = add_fixed_len_page(&page, r);
        }

        write_fixed_len_page(&page, slot_index, r);

    }

    // write last page to file if it has records
    if (page.used_slots > 0) {
        
        // write page to page_file
        int buf_size = page.page_size * record_size;
        buf = new char[buf_size];
        std::vector<Record> *page_data = page.data;
        for (int i = 0; i < fixed_len_page_capacity(&page); i++) {
            if (!page_data->at(i).empty()) {
                fixed_len_write(&(page_data->at(i)), buf);
            }
        }

	// write page to heap (disk)
	write_page(&page, heap, pageID);
    }

//    ftime(&t);
//    long total_run_time = ((t.time * 1000) + t.millitm) - start_time_in_ms;

    csv_file.close();
    
}


