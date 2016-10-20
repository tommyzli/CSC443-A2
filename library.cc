#include <library.h>
#include "string.h"

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

void fixed_len_write(void *buf, int size, Record *record) {
    V attribute;
    for (int i = 0; i < size/ATTRIBUTE_SIZE; ++i) {
        attribute = new char[ATTRIBUTE_SIZE + 1];
        int attribute_index = i * ATTRIBUTE_SIZE;
        memcpy(&attribute, &((char *)buf)[attribute_index], ATTRIBUTE_SIZE);

        record->push_back(attribute);
    }
}
