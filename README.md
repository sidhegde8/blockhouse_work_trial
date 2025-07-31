README.txt

Order Book Reconstruction from MBO to MBP-10
============================================

Overview:
---------
This program reconstructs a Market By Price (MBP) Level-10 order book from Market By Order (MBO) events. It parses timestamped updates (new, modify, cancel), updates internal bid/ask books accordingly, and outputs the top 10 price levels for each side at every event.

The final output is a CSV named `output.csv` that mimics the structure of `mbp.csv`.

Usage:
------
To build:
    make

To run:
    ./reconstruction_<yourname> mbo.csv

To run unit tests:
    make test

To clean up:
    make clean


