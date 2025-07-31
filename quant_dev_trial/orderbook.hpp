#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

class OrderBook {
private:
    std::unordered_map<long, std::pair<double, char>> order_map;

    std::map<double, std::set<long>, std::greater<double>> bids;
    std::map<double, std::set<long>> asks;

public:
    void add_order(double price, int size, char side, long order_id);
    void cancel_order(long order_id, char side);
    std::vector<std::string> get_top_levels(const std::string& ts_event, int depth = 10);
    void process_lines(const std::vector<std::string>& lines, std::ofstream& out);
    void write_output(const std::string& filename);  
};

#endif
