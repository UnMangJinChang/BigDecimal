#include <iostream>
#include <fstream>
#include "BigDecimal.hpp"

int main() {
    std::size_t precision;
    std::string input_1, input_2;
    umjc::BigDecimal input_decimal_1, input_decimal_2;
    std::string answer;
    std::string response;
    std::ifstream input_stream;

    //Addition test
    input_stream.open("decimal_add_test.txt");
    while (input_stream >> precision >> input_1 >> input_2 >> answer) {
        umjc::BigDecimal::set_significant_digits_capacity(precision);
        answer = umjc::BigDecimal(answer).to_scientific_string(precision);
        input_decimal_1 = input_1;
        input_decimal_2 = input_2;
        response = (input_decimal_1 + input_decimal_2).to_scientific_string(precision);
        if (answer.compare(response) != 0) {
            std::clog << "Test case failed!\n"
                << "answer   = " << answer << ", but\n"
                << "response = " << response << ".\n"
                << "input_1          = " << input_1 << "\n"
                << "input_1(decimal) = " << input_decimal_1.to_scientific_string(precision) << "\n"
                << "input_2          = " << input_2 << "\n"
                << "input_2(decimal) = " << input_decimal_2.to_scientific_string(precision) << "\n";
            return EXIT_FAILURE;
        }
    }
    std::cout << "Addition test passed...\n";

    //Multiplication test
    input_stream.open("decimal_mul_test.txt");
    while (input_stream >> precision >> input_1 >> input_2 >> answer) {
        umjc::BigDecimal::set_significant_digits_capacity(precision);
        answer = umjc::BigDecimal(answer).to_scientific_string(precision);
        input_decimal_1 = input_1;
        input_decimal_2 = input_2;
        response = (input_decimal_1 * input_decimal_2).to_scientific_string(precision);
        if (answer.compare(response) != 0) {
            std::clog << "Test case failed!\n"
                << "answer   = " << answer << ", but\n"
                << "response = " << response << ".\n"
                << "input_1          = " << input_1 << "\n"
                << "input_1(decimal) = " << input_decimal_1.to_scientific_string(precision) << "\n"
                << "input_2          = " << input_2 << "\n"
                << "input_2(decimal) = " << input_decimal_2.to_scientific_string(precision) << "\n";
            return EXIT_FAILURE;
        }
    }
    std::cout << "Multiplication test passed...\n";

    //Division test
    input_stream.open("decimal_div_test.txt");
    while (input_stream >> precision >> input_1 >> input_2 >> answer) {
        umjc::BigDecimal::set_significant_digits_capacity(precision);
        answer = umjc::BigDecimal(answer).to_scientific_string(precision);
        input_decimal_1 = input_1;
        input_decimal_2 = input_2;
        response = (input_decimal_1 / input_decimal_2).to_scientific_string(precision);
        if (answer.compare(response) != 0) {
            std::clog << "Test case failed!\n"
                << "answer   = " << answer << ", but\n"
                << "response = " << response << ".\n"
                << "input_1          = " << input_1 << "\n"
                << "input_1(decimal) = " << input_decimal_1.to_scientific_string(precision) << "\n"
                << "input_2          = " << input_2 << "\n"
                << "input_2(decimal) = " << input_decimal_2.to_scientific_string(precision) << "\n";
            return EXIT_FAILURE;
        }
    }
    std::cout << "Division test passed...\n";
    return 0;
}