#!/usr/bin/python

import subprocess
import os

PAGE_SIZES = [
    "1024",
    "2048",
    "4096",
    "8192",
    "16384",
    "32768",
    "65536",
    "131072",
    "262144",
    "524288",
    "1048576",
]

SELECT_RANGES = [
    ("A", "Z"),
    ("A", "M"),
    ("A", "B"),
]

CSV_FILE_SIZE = "100000"  # number of tuples to generate
CSV_FILE_NAME = "large.csv"

if __name__ == "__main__":
    if CSV_FILE_NAME not in os.listdir("."):
        print("Creating a large csv, this will take a while.")
        subprocess.call(["python mkcsv.py", CSV_FILE_NAME, CSV_FILE_SIZE])

    print("Record size: {}".format(CSV_FILE_SIZE))
    for start, end in SELECT_RANGES:
        print("Start: {}, End: {}".format(start, end))
        for page_size in PAGE_SIZES:
            print("======================== {} ========================".format(page_size))
            # Generate new heap file each time to avoid the filesystem cache
            subprocess.call(["./csv2heapfile", CSV_FILE_NAME, "heap_file", page_size])

            subprocess.call(["./select", "heap_file", start, end, page_size, "--benchmark-mode"])

            os.remove("heap_file")
            print("====================================================")
