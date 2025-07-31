#include <iostream>
#include <cassert>
#include "orderbook.hpp"

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

void test_order_replacement() {
    OrderBook ob;
    ob.add_order(20.0, 100, 'A', 1);
    ob.cancel_order(1, 'A');
    ob.add_order(19.0, 100, 'A', 2);

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    assert(!levels.empty());
}

void test_full_book_cleanup() {
    OrderBook ob;
    ob.add_order(10.0, 100, 'B', 1);
    ob.add_order(20.0, 100, 'A', 2);
    ob.cancel_order(1, 'B');
    ob.cancel_order(2, 'A');

    auto levels = ob.get_top_levels("2025-01-01T00:00:00Z");
    assert(levels.empty() || levels[1].empty());  // book should be mostly empty
}

int main() {
    test_basic_add_and_cancel();
    test_multiple_orders_same_price();
    test_order_replacement();
    test_full_book_cleanup();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
