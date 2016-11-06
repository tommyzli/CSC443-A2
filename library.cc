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

void fixed_len_read(char *buf, int size, Record *record) {
    for (int i = 0; i < size/ATTRIBUTE_SIZE; ++i) {
        char* attribute = new char[ATTRIBUTE_SIZE];
        int attribute_index = i * ATTRIBUTE_SIZE;
        strncpy(attribute, buf + attribute_index, ATTRIBUTE_SIZE);

        // not sure why garbage values are ending up in 'attribute'
        attribute[ATTRIBUTE_SIZE] = '\0';

        record->push_back(attribute);
    }
    std::cout << "\n";
}

void init_fixed_len_page(Page *page, int page_size, int slot_size) {
    page->page_size = page_size;
    page->slot_size = slot_size;
    page->used_slots = 0;
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
    return fixed_len_page_capacity(page) - page->used_slots;
}

int add_fixed_len_page(Page *page, Record *r) {
    int slot = 0;
    std::vector<Record> records = *(page->data);
    for (std::vector<Record>::iterator it = records.begin(); it != records.end(); ++it) {
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
