#include "BigInteger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>

int main() {
    std::ifstream ifs("add_test.txt");
    std::string x, y, answer;
    while (ifs >> x >> y >> answer) {
        BigInteger int_x(x), int_y(y);
        std::string response = (int_x + int_y).to_string();
        if (response != answer) {
            std::cout << "Test case failed!\n"
                << "answer = " << answer << ", but\n"
                << "x + y  = " << response << ".\n"
                << "x     = " << x << "\n"
                << "int_x = " << int_x << "\n"
                << "y     = " << y << "\n"
                << "int_y = " << int_y << "\n";
            break;
        }
    }
    return 0;
}