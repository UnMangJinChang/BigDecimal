#include "BigDecimal.hpp"

namespace umjc {
std::size_t BigDecimal::m_significant_digits = 128;

static std::regex const BIG_DECIMAL_REGEX_1 = std::regex("^[+\\-]?[0-9]+\\.[0-9]+$");
static std::regex const BIG_DECIMAL_REGEX_2 = std::regex("^[+\\-]?.[0-9]+$");
static std::regex const BIG_DECIMAL_REGEX_3 = std::regex("^[+\\-]?[0-9]+\\.?$");

void BigDecimal::reduce_integer() const {
    std::size_t integer_digits = m_integer.get_decimal_digits();
    if (integer_digits > m_significant_digits) {
        std::size_t reducing = integer_digits - m_significant_digits;
        while (reducing --> 0) {
            unsigned long remainder = BigInteger::bitset_divide_10(m_integer.m_data);
            if (remainder >= 5) {
                BigInteger::bitset_increment(m_integer.m_data);
            }
        }
    }
}

std::string BigDecimal::to_string() const {
    if (m_integer.is_zero()) {
        return "0.0";
    }
    if (m_exponent < 0) {
        std::string representation;
        if (m_integer.is_negative()) {
            representation += '-';
        }
        representation += "0.";
        if (1 < -m_exponent) {
            representation.insert(representation.end(), -m_exponent - 1, '0');
        }
        BigInteger::IntegerData integer_data = m_integer.m_data;
        std::stack<char> digits;
        while (!BigInteger::bitset_is_zero(integer_data)) {
            unsigned long digit = BigInteger::bitset_divide_10(integer_data);
            digits.push('0' + digit);
        }
        while (!digits.empty()) {
            representation.push_back(digits.top());
            digits.pop();
        }
        //remove tailing zeroes
        while (representation.back() == '0') {
            representation.pop_back();
        }
        return representation;
    }
    else {
        std::string representation;
        BigInteger::IntegerData integer_data = m_integer.m_data;
        std::stack<char> digits;
        while (!BigInteger::bitset_is_zero(integer_data)) {
            unsigned long digit = BigInteger::bitset_divide_10(integer_data);
            digits.push('0' + digit);
        }
        while (!digits.empty()) {
            representation.push_back(digits.top());
            digits.pop();
        }
        if (representation.length() == m_exponent + 1) {
            representation += ".0";
        }
        else if (representation.length() > m_exponent + 1) {
            representation.insert(m_exponent + 1, 1, '.');
        }
        else {
            representation.insert(representation.end(), representation.length() - m_exponent - 1, '0');
            representation += ".0";
        }
        return representation;
    }
}

BigDecimal::BigDecimal() {
    m_integer = BIG_INTEGER_ZERO;
    m_exponent = 0;
}
BigDecimal::BigDecimal(std::string const& x) {
    if (!from_string(x)) {
        m_integer = BIG_INTEGER_ZERO;
        m_exponent = 0;
    }
}
BigDecimal::BigDecimal(char const* x) {
    if (!from_string(x)) {
        m_integer = BIG_INTEGER_ZERO;
        m_exponent = 0;
    }
}
}