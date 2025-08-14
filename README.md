Order Book Reconstruction from MBO to MBP-10
============================================

Overview:
---------
This program reconstructs a Market By Price (MBP) Level-10 order book from Market By Order (MBO) events. It parses timestamped updates (new, modify, cancel), updates internal bid/ask books accordingly, and outputs the top 10 price levels for each side at every event.

The final output is a CSV named `output.csv` that mimics the structure of `mbp.csv`.

Usage:
------
To build:
```sh
    make
```
To run:
```sh
    ./reconstruction.exe mbo.csv
```

To run unit tests:
```sh
    make test
    ./test.exe
```

To clean up:
```sh
    make clean
```

Optimization and Performance Considerations:
--------------------------------------------
1. **Efficient Price-Level Tracking**:
   - Used `std::map<double, std::set<long>>` for both `bids` and `asks`.
   - Bid map is sorted in **descending** order (`std::greater<double>`) to prioritize higher prices.
   - Ask map uses default ascending order to prioritize lower prices.
   - This enables O(log N) insertions, deletions, and fast access to top levels.

2. **Fast Order Lookup**:
   - Maintained an `order_id → (side, price, size)` hash table for O(1) retrieval during cancellations and modifications.
   - This avoids scanning the entire book during each event.

3. **Minimized Redundant Work**:
   - Only emitted top 10 levels at each update to avoid unnecessary formatting or extra copying.
   - Cached and reused stringstream buffers where applicable.

4. **Avoided Overhead from Unnecessary Copies**:
   - Passed large structures (like vectors or strings) by reference where possible.
   - Used `emplace` over `insert` to avoid temporary construction.

5. **Clean Separation of Logic**:
   - Parsing, order handling, and output logic are modular.
   - Unit tests target the `OrderBook` class in isolation.

6. **I/O Throughput Considerations**:
   - Used buffered output (via `std::ofstream`) to minimize I/O overhead.
   - Ensured consistent fixed-point precision for price formatting (to match `mbp.csv`).

Things to be aware of:
--------------------------------------
Orders with unknown sides or malformed rows are skipped silently to ensure robustness. The unit tests simulate a variety of realistic scenarios, including order additions, cancellations, and edge cases such as complete level depletion. The implementation ensures strict bid-side descending and ask-side ascending ordering at each depth level.

Takeaways and limitations:
-------------------------
While the current approach is efficient for processing up to approximately 10 million events, future scalability could benefit from enhancements like preallocating memory for price levels or bypassing stringstream overhead by writing directly to preformatted character buffers. Additionally, although the implementation currently handles one symbol at a time, the output writing could be parallelized to support multi symbol order books in future expansions. I prioritized clarity and safety by using standard containers and avoiding low level pointer manipulation or custom allocators. One area for improvement could be enhanced parsing resilience, such as incorporating fallback modes or logging malformed rows for further inspection instead of silently skipping them.

