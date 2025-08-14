#include "orderbook.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>


// Adds a new order to the order book and maps it to the order ID.
void OrderBook::add_order(double price, int size, char side, long order_id) {
    if (side == 'B') {
        bids[price].insert(order_id);
    } else if (side == 'A') {
        asks[price].insert(order_id);
    }
    order_map[order_id] = {price, side, size};
}

// Cancels an order by removing it from the order book and the order map.
void OrderBook::cancel_order(long order_id, char side) {
    if (order_map.find(order_id) == order_map.end()) return;

    auto& info = order_map[order_id];
    double price = info.price;
    char stored_side = info.side;

    if (stored_side == 'B') {
        auto it = bids.find(price);
        if (it != bids.end()) {
            it->second.erase(order_id);
            if (it->second.empty()) bids.erase(it);
        }
    } else if (stored_side == 'A') {
        auto it = asks.find(price);
        if (it != asks.end()) {
            it->second.erase(order_id);
            if (it->second.empty()) asks.erase(it);
        }
    }

    order_map.erase(order_id);
}

// Retireves the top N levels of the orderbook
std::vector<std::string> OrderBook::get_top_levels(const std::string& ts_event, int depth) {
    std::vector<std::string> row;
    row.push_back(ts_event);

    // Prepare bid and ask vectors
    std::vector<std::string> bid_data, ask_data;
    int bid_count = 0, ask_count = 0;

    for (auto it = bids.begin(); it != bids.end() && bid_count < depth; ++it, ++bid_count) {
        int total_size = 0;
        for (auto order_id : it->second) {
            total_size += order_map[order_id].size;
        }
        bid_data.push_back(std::to_string(it->first));
        bid_data.push_back(std::to_string(total_size));
        bid_data.push_back(std::to_string(it->second.size()));
    }
    while (bid_count++ < depth) {
        bid_data.push_back(""); bid_data.push_back(""); bid_data.push_back("");
    }

    for (auto it = asks.begin(); it != asks.end() && ask_count < depth; ++it, ++ask_count) {
        int total_size = 0;
        for (auto order_id : it->second) {
            total_size += order_map[order_id].size;
        }
        ask_data.push_back(std::to_string(it->first));
        ask_data.push_back(std::to_string(total_size));
        ask_data.push_back(std::to_string(it->second.size()));
    }
    while (ask_count++ < depth) {
        ask_data.push_back(""); ask_data.push_back(""); ask_data.push_back("");
    }

    for (int i = 0; i < depth; ++i) {
        row.push_back(bid_data[i * 3]);
        row.push_back(bid_data[i * 3 + 1]);
        row.push_back(bid_data[i * 3 + 2]);
        row.push_back(ask_data[i * 3]);
        row.push_back(ask_data[i * 3 + 1]);
        row.push_back(ask_data[i * 3 + 2]);
    }

    return row;
}

void OrderBook::process_lines(const std::vector<std::string>& lines, std::ofstream& out) {
    bool header_written = false;

    for (const auto& line : lines) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> cols;

        while (std::getline(ss, token, ',')) {
            cols.push_back(token);
        }

        if (cols.size() < 13 || cols[5] == "R")
            continue;

        std::string ts_event = cols[1];
        std::string action = cols[5];
        char side = cols[6].empty() ? ' ' : cols[6][0];
        double price = cols[7].empty() ? 0.0 : std::stod(cols[7]);
        int size = cols[8].empty() ? 0 : std::stoi(cols[8]);
        long order_id = cols[10].empty() ? 0 : std::stol(cols[10]);

        if (action == "A") {
            add_order(price, size, side, order_id);
        } else if (action == "C") {
            cancel_order(order_id, side);
        }

        auto row = get_top_levels(ts_event);
        if (!header_written) {
            out << "ts_event";
            for (int i = 0; i < 10; ++i) {
                out << ",bid_px_" << std::setw(2) << std::setfill('0') << i;
                out << ",bid_sz_" << std::setw(2) << std::setfill('0') << i;
                out << ",bid_ct_" << std::setw(2) << std::setfill('0') << i;
                out << ",ask_px_" << std::setw(2) << std::setfill('0') << i;
                out << ",ask_sz_" << std::setw(2) << std::setfill('0') << i;
                out << ",ask_ct_" << std::setw(2) << std::setfill('0') << i;
            }
            out << "\n";
            header_written = true;
        }

        for (size_t i = 0; i < row.size(); ++i) {
            if (i != 0) out << ",";
            out << row[i];
        }
        out << "\n";
    }
}
