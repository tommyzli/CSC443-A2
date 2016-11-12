#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include "library.h"

int main(int argc, char** argv) {
    if (argc < 6) {
        std::cout << "Error, usage must be:\n";
        std::cout << "./update <heapfile> <record_id> <attribute_id> <new_value> <page_size>\n";
        return 1;
    }

    FILE *heapfile = fopen(argv[1], "r+");
    if (!heapfile) {
        std::cout << "Error, could not find file " << argv[1] << "\n";
        return 1;
    }


    std::string serialized_record_id = argv[2];
    size_t delimiter_position = serialized_record_id.find(RID_DELIMITER);
    if (delimiter_position == 0 || delimiter_position == serialized_record_id.length() - 1) {
        std::cout << "Error, record_id must be:\n";
        std::cout << "<page_id>%<slot_number>\n";
        return 1;
    }
    RecordID *rid = new RecordID();
    rid->page_id = atoi(serialized_record_id.substr(0, delimiter_position).c_str());
    rid->slot = atoi(serialized_record_id.substr(delimiter_position + 1).c_str());

    int attribute_id = atoi(argv[3]);
    if (attribute_id < 0 || attribute_id > (NUM_ATTRIBUTES * ATTRIBUTE_SIZE) - 1) {
        std::cout << "Error, attribute_id must be between 0 and 99\n";
        return 1;
    }

    std::string new_value = argv[4];
    if (new_value.length() != 10) {
        std::cout << "Error, new_value must be 10 characters long\n";
        return 1;
    }

    int page_size = atoi(argv[5]);
}
