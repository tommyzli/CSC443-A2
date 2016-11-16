CSC443 Assignment 2

To build:
    > make all

Page operations
---------------
write_fixed_len_pages: ./write_fixed_len_pages csv_file page_file page_size

read_fixed_len_page: ./read_fixed_len_page page_file page_size [--benchmark-mode]
    By default, read_fixed_len_page writes to stdout. You can pass in
    "--benchmark-mode" as the last arg and it will write to /dev/null.


Heap file operations
--------------------
csv2heapfile: ./csv2colstore csv_file heap_file page_size

scan: ./scan heapfile page_size

select: ./select heapfile attribute_id start end page_size [--benchmark-mode]
    By default, select writes to stdout. You can pass in "--benchmark-mode" as
    the last arg and it will write to /dev/null instead.

insert: ./insert heapfile csv_file page_size

delete: ./delete heapfile record_id page_size
    The record_id arg expects a string of the form "page_id%slot_number".

update: ./update heapfile record_id attribute_id new_value page_size
    The record_id arg expects a string of the form "page_id%slot_number".


Column store operations
-----------------------
csv2colstore: ./csv2colstore csv_file colstore_name page_size
     The colstore_name arg is the name of the directory you want the column files
     to be created in. It will be created as a child of your current directory.
     You can't have a preexisting directory of the same name when you run csv2colstore.

select2: ./select2 colstore_name attribute_id start end page_size
    By default, select2 writes to stdout. You can pass in "--benchmark-mode" as
    the last arg and it will write to /dev/null instead.

select3: ./select3 colstore_name attribute_id return_attribute_id start end page_size
    By default, select3 writes to stdout. You can pass in "--benchmark-mode" as
    the last arg and it will write to /dev/null instead.
