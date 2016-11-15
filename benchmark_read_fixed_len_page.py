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


CSV_FILE_SIZE = "100000"  # number of tuples to generate
CSV_FILE_NAME = "large.csv"

if __name__ == "__main__":
    if CSV_FILE_NAME not in os.listdir("."):
        print("Creating a large csv, this will take a while.")
        subprocess.call(["python mkcsv.py", CSV_FILE_NAME, CSV_FILE_SIZE])

    print("Record size: {}".format(CSV_FILE_SIZE))
    for page_size in PAGE_SIZES:
        print("======================== {} ========================".format(page_size))
        for _ in range(0, 5):
            # Generate a new page file each time to avoid filesystem cache
            subprocess.call(["./write_fixed_len_pages", "large.csv", "page_file", page_size, "--no-output"])
            subprocess.call(["./read_fixed_len_page", "page_file", page_size, "--benchmark-mode"])
            os.remove("page_file")
        print("====================================================")
