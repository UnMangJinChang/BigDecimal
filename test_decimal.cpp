#include <iostream>
#include "BigDecimal.hpp"

int main() {
    umjc::BigDecimal::set_significant_digits_capacity(30);
    umjc::BigDecimal euler = "0";
    umjc::BigDecimal factorial = "1";
    for (umjc::BigInteger i = "0"; i <= "50"; i++) {
        auto term = umjc::BigDecimal("1") / factorial;
        euler += term;
        factorial *= (i + umjc::BIG_INTEGER_ONE);
    }
    std::cout.precision(20);
    std::cout << (euler * "100000.0") << "\n";
    std::cout << (euler * "10000.0") << "\n";
    std::cout << (euler * "1000.0") << "\n";
    std::cout << (euler * "100.0") << "\n";
    std::cout << (euler * "10.0") << "\n";
    std::cout << (euler) << "\n";
    std::cout << (euler / "10.0") << "\n";
    std::cout << (euler / "100.0") << "\n";
    std::cout << (euler / "1000.0") << "\n";
    std::cout << (euler / "10000.0") << "\n";
    std::cout << (euler / "100000.0") << "\n";
    return 0;
}