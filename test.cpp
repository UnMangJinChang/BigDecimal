#include "BigInteger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream test_cases("add_test.txt");
    std::size_t test_case_count = 0;
    std::string x, y, answer_1, answer_2;
    bool test_success = true;
    auto start = std::chrono::system_clock::now();
    while (test_cases >> x >> y >> answer_1) {
        BigInteger int_x(x), int_y(y);
        BigInteger response = (int_x + int_y);
        if (response != answer_1) {
            std::cout << "Test case failed!\n"
                << "answer = " << answer_1 << ", but\n"
                << "x + y  = " << response << ".\n"
                << "x     = " << x << "\n"
                << "y     = " << y << "\n";
            test_success = true;
            break;
        }
        test_case_count++;
    }
    auto finish = std::chrono::system_clock::now();
    if (test_success) {
        std::cout << "Addition test passed: \n"
            << "    Average execution time: " << (finish - start) / test_case_count << "\n";
    }
    test_cases.close();
    test_cases.open("mul_test.txt");
    start = std::chrono::system_clock::now();
    while (test_cases >> x >> y >> answer_1) {
        BigInteger int_x(x), int_y(y);
        BigInteger response = (int_x * int_y);
        if (response != answer_1) {
            std::cout << "Test case failed!\n"
                << "answer = " << answer_1 << ", but\n"
                << "x * y  = " << response << ".\n"
                << "x     = " << x << "\n"
                << "y     = " << y << "\n";
            test_success = true;
            break;
        }
        test_case_count++;
    }
    finish = std::chrono::system_clock::now();
    if (test_success) {
        std::cout << "Multiplication test passed: \n"
            << "    Average execution time: " << (finish - start) / test_case_count << "\n";
    }
    test_cases.close();
    test_cases.open("div_test.txt");
    test_case_count = 0;
    test_success = true;
    start = std::chrono::system_clock::now();
    while (test_cases >> x >> y >> answer_1 >> answer_2) {
        BigInteger int_x(x), int_y(y);
        BigInteger response_1 = int_x / int_y;
        BigInteger response_2 = int_x % int_y;
        if (response_1 != answer_1 or response_2 != answer_2) {
            std::cout << "Test case failed!\n"
                << "answer_1 = " << answer_1 << " and \n" 
                << "answer_2 = " << answer_2 << ", but\n"
                << "x / y    = " << response_1 << " and \n" 
                << "x % y    = " << response_2 << ".\n"
                << "x     = " << x << "\n"
                << "y     = " << y << "\n";
            test_success = false;
            break;
        }
        test_case_count++;
    }
    finish = std::chrono::system_clock::now();
    if (test_success) {
        std::cout << "Division test passed: \n"
            << "    Average execution time: " << (finish - start) / test_case_count << "\n";
    }
    test_cases.close();
    return 0;
}