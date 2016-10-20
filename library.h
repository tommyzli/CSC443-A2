#ifndef __LIBRARY_H_INCLUDED__
#define __LIBRARY_H_INCLUDED__

#include <vector>

typedef const char* V;
typedef std::vector<V> Record;

const int NUM_ATTRIBUTES = 100;
const int ATTRIBUTE_SIZE = 10;

// Compute the # of bytes required to serialize a record
int fixed_len_sizeof(Record *record);

// Serialize the record to a byte array to be stored in a buf
void fixed_len_write(Record *record, void *buf);

// Deserializes 'size' bytes from buffer 'buf' and stores the record in 'record'
void fixed_len_read(void *buf, int size, Record *record);

#endif
