#!/usr/bin/python

import shutil
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
        subprocess.call(["./mkcsv.py", CSV_FILE_NAME, CSV_FILE_SIZE])

    # ignore first read, try to have file system cache out.csv
    subprocess.call(["./csv2colstore", CSV_FILE_NAME, "colstore", "65536", "--no-output"])
    shutil.rmtree("colstore")

    for page_size in PAGE_SIZES:
        print("======================== {} ========================".format(page_size))
        for _ in range(0, 3):
            subprocess.call(["./csv2colstore", CSV_FILE_NAME, "colstore", page_size])
            shutil.rmtree("colstore")

        print("====================================================")
