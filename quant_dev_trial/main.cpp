#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "orderbook.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " mbo.csv" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open input file." << std::endl;
        return 1;
    }

    std::vector<std::string> all_lines;
    std::string line;

    // Skip header
    std::getline(infile, line);
    
    while (std::getline(infile, line)) {
        all_lines.push_back(line);
    }

    infile.close();

    OrderBook ob;
    std::ofstream out("output.csv");
    ob.process_lines(all_lines, out);
    out.close();

    std::cout << "Processed MBO file successfully." << std::endl;
    return 0;
}
