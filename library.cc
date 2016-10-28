#include <stdlib.h>
#include "string.h"
#include <iostream>
#include <vector>

#include "library.h"


int fixed_len_sizeof(Record *record) {
    int size = 0;
    for (Record::iterator it = record->begin(); it != record->end(); ++it) {
        size += strlen(*it);
    }
    return size;
    // return ATTRIBUTE_SIZE * NUM_ATTRIBUTES;
}

void fixed_len_write(Record *record, void *buf) {
    for (Record::iterator it = record->begin(); it != record->end(); ++it) {
        strcat((char *)buf, *it);
    }
}

void fixed_len_read(void *buf, int size, Record *record) {
    V attribute;
    for (int i = 0; i < size/ATTRIBUTE_SIZE; ++i) {
        attribute = new char[ATTRIBUTE_SIZE + 1];
        int attribute_index = i * ATTRIBUTE_SIZE;
        memcpy(&attribute, &((char *)buf)[attribute_index], ATTRIBUTE_SIZE);

        record->push_back(attribute);
    }
}

void init_fixed_len_page(Page *page, int page_size, int slot_size) {
    page->page_size = page_size;
    page->slot_size = slot_size;
    std::vector<Record> *data = new std::vector<Record>();

    int i;
    for (i = 0; i < fixed_len_page_capacity(page); ++i) {
        Record *r = new Record();
        data->push_back(*r);
    }
    page->data = data;
}

int fixed_len_page_capacity(Page *page) {
    return page->page_size / page->slot_size;
}

int fixed_len_page_freeslots(Page *page) {
    int freeslots = 0;
    for (std::vector<Record>::iterator it = page->data->begin(); it != page->data->end(); ++it) {
        if (it->empty()) {
            freeslots++;
        }
    }

    return freeslots;
}

int add_fixed_len_page(Page *page, Record *r) {
    int slot = 0;
    for (std::vector<Record>::iterator it = page->data->begin(); it != page->data->end(); ++it) {
        if (it->empty()) {
            return slot;
        }
        slot ++;
    }
    return -1;
}

void write_fixed_len_page(Page *page, int slot, Record *r) {
    page->data->at(slot) = *r;
}

void read_fixed_len_page(Page *page, int slot, Record *r) {
    *r = page->data->at(slot);
}

int main() {
    return 0;
}
