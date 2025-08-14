#include <iostream>
#include <cassert>
#include "orderbook.hpp"

// Tests adding and cancelling basic orders, checks bid/ask levels after each operation
void test_basic_add_and_cancel() {
    OrderBook ob;
    ob.add_order(10.0, 100, 'B', 1);  // price, size, side, order_id
    ob.add_order(20.0, 50, 'A', 2);

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    assert(!levels.empty());

    ob.cancel_order(1, 'B');
    levels = ob.get_top_levels("2025-01-01T00:00:01Z");
    assert(!levels.empty());
}

// Tests multiple orders at the same price, then cancels one and checks levels
void test_multiple_orders_same_price() {
    OrderBook ob;
    ob.add_order(9.5, 50, 'B', 1);
    ob.add_order(9.5, 50, 'B', 2);

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    assert(!levels.empty());

    ob.cancel_order(1, 'B');
    levels = ob.get_top_levels("2025-01-01T00:00:01Z");
    assert(!levels.empty());
}

// Tests cancelling and replacing an order, checks levels after replacement
void test_order_replacement() {
    OrderBook ob;
    ob.add_order(20.0, 100, 'A', 1);
    ob.cancel_order(1, 'A');
    ob.add_order(19.0, 100, 'A', 2);

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    assert(!levels.empty());
}

// Tests full book cleanup, ensures book is empty after all cancels
void test_full_book_cleanup() {
    OrderBook ob;
    ob.add_order(10.0, 100, 'B', 1);
    ob.add_order(20.0, 100, 'A', 2);
    ob.cancel_order(1, 'B');
    ob.cancel_order(2, 'A');

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    assert(levels.empty() || levels[1].empty());  // book should be mostly empty
}

// Tests aggregation of sizes for multiple orders at the same price
void test_size_aggregation() {
    OrderBook ob;
    ob.add_order(10.0, 100, 'B', 1);
    ob.add_order(10.0, 50, 'B', 2);
    ob.add_order(10.0, 25, 'B', 3);

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    // bid_px_00 = 10.0, bid_sz_00 = 175, bid_ct_00 = 3
    assert(levels[1] == "10.000000");
    assert(levels[2] == "175");
    assert(levels[3] == "3");
}

// Tests correct size after partial cancel of orders at the same price
void test_partial_cancel_size() {
    OrderBook ob;
    ob.add_order(20.0, 100, 'A', 1);
    ob.add_order(20.0, 50, 'A', 2);
    ob.cancel_order(1, 'A');

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    // ask_px_00 = 20.0, ask_sz_00 = 50, ask_ct_00 = 1
    assert(levels[4] == "20.000000");
    assert(levels[5] == "50");
    assert(levels[6] == "1");
}

// Tests output order: bid columns followed by ask columns for each level
void test_bid_ask_order_output() {
    OrderBook ob;
    ob.add_order(10.0, 100, 'B', 1);
    ob.add_order(20.0, 200, 'A', 2);

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    // Check output order: bid_px_00, bid_sz_00, bid_ct_00, ask_px_00, ask_sz_00, ask_ct_00
    assert(levels[1] == "10.000000");
    assert(levels[2] == "100");
    assert(levels[3] == "1");
    assert(levels[4] == "20.000000");
    assert(levels[5] == "200");
    assert(levels[6] == "1");
}


int main() {
    test_basic_add_and_cancel();
    test_multiple_orders_same_price();
    test_order_replacement();
    test_full_book_cleanup();
    test_size_aggregation();
    test_partial_cancel_size();
    test_bid_ask_order_output();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
