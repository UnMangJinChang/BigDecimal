#include <iostream>
#include "BigDecimal.hpp"

int main() {
    umjc::BigDecimal::set_significant_digits_capacity(100);
    umjc::BigDecimal input;
    std::cout << "√";
    std::cin >> input;
    umjc::BigDecimal square_root = input;
    for (std::size_t i = 0; i < 50; i++) {
        square_root = (square_root + input / square_root) / "2";
    }
    std::cout << "square root of " << input << ": " << square_root << "\n";

    std::cin >> input;
    umjc::BigDecimal factorial = "1";
    umjc::BigDecimal input_pow = input;
    umjc::BigDecimal sine = "0";
    for (umjc::BigInteger i = "0"; i < "50"; i++) {
        std::clog << factorial << " " << input_pow << " " << sine << "\n";
        if (i.is_even()) {
            sine += input_pow / factorial;
        }
        else {
            sine -= input_pow / factorial;
        }
        input_pow *= input * input;
        factorial *= (umjc::BIG_INTEGER_TWO * i + umjc::BIG_INTEGER_TWO);
        factorial *= (umjc::BIG_INTEGER_TWO * i + umjc::BIG_INTEGER_THREE);
    }
    std::cout << "sin(" << input << ") = " << sine << "\n";
    return 0;
}