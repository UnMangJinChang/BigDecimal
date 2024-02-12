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
    umjc::BigDecimal::set_significant_digits_capacity(250);
    std::cout << "e = \n";
    std::cout << umjc::BigDecimal::euler_number().to_string(250) << "\n";
    std::cout << "sqrt(5.2) = \n";
    std::cout << umjc::BigDecimal::sqrt("5.2").to_string(250) << "\n";
    std::cout << "sqrt(0.314159265359) = \n";
    std::cout << umjc::BigDecimal::sqrt("0.314159265359").to_string(250) << "\n";
    std::cout << "cbrt(2) = \n";
    std::cout << umjc::BigDecimal::cbrt("2").to_string(250) << "\n";
    std::cout << "cbrt(2000) = \n";
    std::cout << umjc::BigDecimal::cbrt("2000").to_string(250) << "\n";
    std::cout << "exp(0.8) = \n";
    std::cout << umjc::BigDecimal::exp("0.8").to_string(250) << "\n";
    std::cout << "exp(-0.3) = \n";
    std::cout << umjc::BigDecimal::exp("-0.3").to_string(250) << "\n";
    std::cout << "exp(7) = \n";
    std::cout << umjc::BigDecimal::exp("7").to_string(250) << "\n";
    std::cout << "exp(-5) = \n";
    std::cout << umjc::BigDecimal::exp("-5").to_string(250) << "\n";
    std::cout << "ln(6) = \n";
    std::cout << umjc::BigDecimal::ln("6").to_string(250) << "\n";
    std::cout << "ln(0.2) = \n";
    std::cout << umjc::BigDecimal::ln("0.2").to_string(250) << "\n";
    return 0;
}