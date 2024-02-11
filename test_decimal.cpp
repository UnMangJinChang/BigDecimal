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
    std::cout << "e = \n";
    umjc::BigDecimal::set_significant_digits_capacity(1000);
    umjc::BigDecimal euler = "0";
    umjc::BigDecimal factorial = "1";
    for (umjc::BigInteger i = "1"; i <= "500"; i++) {
        euler += umjc::BigDecimal("1.0") / factorial;
        factorial *= i;
    }
    std::cout << euler.to_string(1000) << "\n";
    std::cout << "sqrt(5) = \n";
    umjc::BigDecimal sqrt_5 = "3.0";
    for (umjc::BigInteger i = "1"; i <= "50"; i++) {
        sqrt_5 = umjc::BigDecimal("0.5") * (sqrt_5 + umjc::BigDecimal("5.0") / sqrt_5);
    }
    std::cout << sqrt_5.to_string(1000) << "\n";
    std::cout << "cbrt(2) = \n";
    umjc::BigDecimal cbrt_2 = "2.0";
    for (umjc::BigInteger i = "1"; i <= "50"; i++) {
        cbrt_2 = umjc::BigDecimal("2") * cbrt_2 / "3" + umjc::BigDecimal("2") / (umjc::BigDecimal("3") * cbrt_2 * cbrt_2);
    }
    std::cout << cbrt_2.to_string(1000) << "\n";
    return 0;
}