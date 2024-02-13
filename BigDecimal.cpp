#include "BigDecimal.hpp"
#include <iostream>

umjc::BigInteger pow_10(std::size_t n) {
    if (n == 0) {
        return umjc::BIG_INTEGER_ONE;
    }
    if (n == 1) {
        return umjc::BIG_INTEGER_TEN;
    }
    if (n == 2) {
        return umjc::BIG_INTEGER_HUNDRED;
    }
    if (n == 3) {
        return umjc::BIG_INTEGER_THOUSAND;
    }
    umjc::BigInteger half_pow = pow_10(n / 2);
    if (n % 2 == 0) {
        return half_pow * half_pow;
    }
    else {
        return half_pow * half_pow * umjc::BIG_INTEGER_TEN;
    }
}

namespace umjc {
std::size_t BigDecimal::m_significant_digits = 128;

static std::regex const BIG_DECIMAL_REGEX = std::regex("^([+\\-])?([0-9]+\\.?[0-9]*|\\.[0-9]+)?(e[+\\-]?[0-9]+)?$");

void BigDecimal::reduce_integer(BigInteger& x) {
    if (x.is_zero()) {
        return;
    }
    std::size_t decimal_digits = x.get_decimal_digits();
    if (decimal_digits > m_significant_digits) {
        std::size_t reducing = decimal_digits - m_significant_digits;
        BigInteger::IntegerData divisor = pow_10(reducing).m_data;
        BigInteger::IntegerData remainder = BigInteger::bitset_divide(x.m_data, divisor);
        BigInteger::bitset_shift_left(remainder, 1);
        int compare = BigInteger::bitset_compare(remainder, divisor);
        if (compare > 0) {
            BigInteger::bitset_increment(x.m_data);
        }
        else if (compare == 0) {
            BigInteger::IntegerData current_data = x.m_data;
            unsigned long last_digit = BigInteger::bitset_divide_10(current_data);
            if (last_digit % 2 != 0) {
                BigInteger::bitset_increment(x.m_data);
            }
        }
    }
}

std::string BigDecimal::to_string(std::size_t count) const {
    if (m_integer.is_zero()) {
        return "0";
    }
    BigInteger::IntegerData digit_data = m_integer.m_data;
    std::stack<char> digits;
    bool found_first_nonzero = false;
    while (!BigInteger::bitset_is_zero(digit_data)) {
        unsigned long remainder = BigInteger::bitset_divide_10(digit_data);
        if (remainder != 0 or found_first_nonzero) {
            found_first_nonzero = true;
            digits.push('0' + remainder);
        }
    }
    std::string result;
    //1. Fixed 1
    if (0 <= m_exponent and m_exponent < count) {
        for (std::size_t i = 0; i < count; i++) {
            if (digits.empty()) {
                if (i >= m_exponent + 1) {
                    break;
                }
                result += "0";
            }
            else {
                if (i == m_exponent + 1) {
                    result += ".";
                }
                result += digits.top();
                digits.pop();
            }
        }
        //number string add 
        if (!digits.empty() and digits.top() > '4') {
            //Round half to even
            bool carry = digits.top() != '5';
            if (digits.top() == '5') {
                bool all_zero = true;
                digits.pop();
                while (!digits.empty()) {
                    if (digits.top() != '0') {
                        all_zero = false;
                        break;
                    }
                    else {
                        digits.pop();
                    }
                }
                carry = !all_zero or (result.back() - '0') % 2 != 0;
            }
            for (auto rit = result.rbegin(); rit != result.rend(); rit++) {
                if (*rit == '.') {
                    continue;
                }
                if (carry) {
                    (*rit)++;
                }
                if (*rit == '0' + 10) {
                    *rit = '0';
                    carry = true;
                }
                else {
                    carry = false;
                    break;
                }
            }
            if (carry) {
                result = "1" + result;
            }
        }
    }
    //2. Fixed 2
    else if (m_exponent < 0 and -m_exponent <= count) {
        result = "0.";
        if (m_exponent < -1) {
            result += std::string(-m_exponent - 1, '0');
        }
        for (std::size_t i = -m_exponent; i < count; i++) {
            if (digits.empty()) {
                break;
            }
            else {
                result += digits.top();
                digits.pop();
            }
        }
        //number string add 
        if (!digits.empty() and digits.top() > '4') {
            //Round half to even
            bool carry = digits.top() != '5';
            if (digits.top() == '5') {
                bool all_zero = true;
                digits.pop();
                while (!digits.empty()) {
                    if (digits.top() != '0') {
                        all_zero = false;
                        break;
                    }
                    else {
                        digits.pop();
                    }
                }
                carry = !all_zero or (result.back() - '0') % 2 != 0;
            }
            for (auto rit = result.rbegin(); rit != result.rend(); rit++) {
                if (*rit == '.') {
                    continue;
                }
                if (carry) {
                    (*rit)++;
                }
                if (*rit == '0' + 10) {
                    *rit = '0';
                    carry = true;
                }
                else {
                    carry = false;
                    break;
                }
            }
            if (carry) {
                result = "1" + result;
            }
        }
    }
    //3. Scientific
    else {
        for (std::size_t i = 0; i < count; i++) {
            if (digits.empty()) {
                break;
            }
            else {
                if (i == 1) {
                    result += ".";
                }
                result += digits.top();
                digits.pop();
            }
        }
        //number string add 
        if (!digits.empty() and digits.top() > '4') {
            //Round half to even
            bool carry = digits.top() != '5';
            if (digits.top() == '5') {
                bool all_zero = true;
                digits.pop();
                while (!digits.empty()) {
                    if (digits.top() != '0') {
                        all_zero = false;
                        break;
                    }
                    else {
                        digits.pop();
                    }
                }
                carry = !all_zero or (result.back() - '0') % 2 != 0;
            }
            for (auto rit = result.rbegin(); rit != result.rend(); rit++) {
                if (*rit == '.') {
                    continue;
                }
                if (carry) {
                    (*rit)++;
                }
                if (*rit == '0' + 10) {
                    *rit = '0';
                    carry = true;
                }
                else {
                    carry = false;
                    break;
                }
            }
            if (carry) {
                result = "1" + result;
            }
        }
        //exponent
        result += "e";
        if (m_exponent > 0) {
            result += "+";
        }
        result += std::to_string(m_exponent);
    }
    if (!m_integer.m_positive) {
        result = "-" + result;
    }
    return result;
}

std::string BigDecimal::to_scientific_string(std::size_t count) const {
    if (m_integer.is_zero()) {
        return "0";
    }
    BigInteger::IntegerData digit_data = m_integer.m_data;
    std::stack<char> digits;
    bool found_first_nonzero = false;
    while (!BigInteger::bitset_is_zero(digit_data)) {
        unsigned long remainder = BigInteger::bitset_divide_10(digit_data);
        if (remainder != 0 or found_first_nonzero) {
            found_first_nonzero = true;
            digits.push('0' + remainder);
        }
    }
    std::string result;
    for (std::size_t i = 0; i < count; i++) {
        if (digits.empty()) {
            break;
        }
        else {
            if (i == 1) {
                result += ".";
            }
            result += digits.top();
            digits.pop();
        }
    }
    //number string add 
    if (!digits.empty() and digits.top() > '4') {
        //Round half to even
        bool carry = digits.top() != '5';
        if (digits.top() == '5') {
            bool all_zero = true;
            digits.pop();
            while (!digits.empty()) {
                if (digits.top() != '0') {
                    all_zero = false;
                    break;
                }
                else {
                    digits.pop();
                }
            }
            carry = !all_zero or (result.back() - '0') % 2 != 0;
        }
        for (auto rit = result.rbegin(); rit != result.rend(); rit++) {
            if (*rit == '.') {
                continue;
            }
            if (carry) {
                (*rit)++;
            }
            if (*rit == '0' + 10) {
                *rit = '0';
                carry = true;
            }
            else {
                carry = false;
                break;
            }
        }
        if (carry) {
            result = "1" + result;
        }
    }
    //exponent
    result += "e";
    if (m_exponent > 0) {
        result += "+";
    }
    result += std::to_string(m_exponent);
    if (!m_integer.m_positive) {
        result = "-" + result;
    }
    return result;
}

std::string BigDecimal::to_fixed_string(std::size_t count) const {
    if (m_integer.is_zero()) {
        return "0." + std::string(count, '0');
    }
    BigInteger::IntegerData digit_data = m_integer.m_data;
    std::stack<char> digits;
    bool found_first_nonzero = false;
    while (!BigInteger::bitset_is_zero(digit_data)) {
        unsigned long remainder = BigInteger::bitset_divide_10(digit_data);
        if (remainder != 0 or found_first_nonzero) {
            found_first_nonzero = true;
            digits.push('0' + remainder);
        }
    }
    std::string result;
    if (m_exponent + 1 == 0) {
        result = "0";
        for (std::size_t i = 0; i < count; i++) {
            if (digits.empty()) {
                result += "0";
            }
            else {
                result += digits.top();
                digits.pop();
            }
        }
    }
    else if (m_exponent + 1 > 0) {
        for (std::size_t i = 0; i < count + m_exponent + 1; i++) {
            if (digits.empty()) {
                result += "0";
            }
            else {
                result += digits.top();
                digits.pop();
            }
        }
    }
    else {
        std::size_t padding_zeroes = std::min<std::size_t>(count, -m_exponent - 1);
        result = std::string(padding_zeroes + 1, '0');
        for (std::size_t i = 0; i < count - padding_zeroes; i++) {
            if (digits.empty()) {
                result += "0";
            }
            else {
                result += digits.top();
                digits.pop();
            }
        }
    }
    //number string add 
    if (!digits.empty() and digits.top() > '4') {
        //Round half to even
        bool carry = digits.top() != '5';
        if (digits.top() == '5') {
            bool all_zero = true;
            digits.pop();
            while (!digits.empty()) {
                if (digits.top() != '0') {
                    all_zero = false;
                    break;
                }
                else {
                    digits.pop();
                }
            }
            carry = !all_zero or (result.back() - '0') % 2 != 0;
        }
        for (auto rit = result.rbegin(); rit != result.rend(); rit++) {
            if (*rit == '.') {
                continue;
            }
            if (carry) {
                (*rit)++;
            }
            if (*rit == '0' + 10) {
                *rit = '0';
                carry = true;
            }
            else {
                carry = false;
                break;
            }
        }
        if (carry) {
            result = "1" + result;
        }
    }
    result.insert(result.length() - count, ".");
    if (!m_integer.m_positive) {
        result = "-" + result;
    }
    return result;
}

bool BigDecimal::from_string(std::string const& str) {
    m_integer = BIG_INTEGER_ZERO;
    m_exponent = 0;
    std::smatch regex_match;
    if (!std::regex_match(str, regex_match, BIG_DECIMAL_REGEX)) {
        return false;
    }
    // Group 1: Sign
    bool positive = true;
    if (regex_match[1].matched and regex_match[1].str() == "-") {
        positive = false;
    }
    // Group 2: Significant digits
    if (regex_match[2].matched) {
        std::string match_str = regex_match[2].str();
        std::string::const_iterator lstrip_iterator = match_str.begin();
        while (*lstrip_iterator == '0') {
            lstrip_iterator++;
        }
        std::string::size_type point_pos = match_str.find('.');
        if (point_pos != std::string::npos) {
            m_exponent = (long long)point_pos - 1;
            if (point_pos == lstrip_iterator - match_str.begin() and lstrip_iterator != match_str.begin()) {
                m_exponent -= lstrip_iterator - match_str.begin();
            }
            match_str.erase(point_pos, 1);
        }
        else {
            m_exponent = match_str.length() - 1;
        }
        bool round_up = false;
        if (match_str.length() > m_significant_digits) {
            if (match_str[m_significant_digits] >= '5') {
                round_up = true;
            }
            match_str.erase(m_significant_digits);
        }
        std::string::const_reverse_iterator rstrip_iterator = match_str.rbegin();
        while (*rstrip_iterator == '0') {
            rstrip_iterator++;
        }
        std::string::size_type strip_idx = lstrip_iterator - match_str.cbegin();
        std::string::size_type strip_len = rstrip_iterator.base() - lstrip_iterator;
        m_integer = match_str.substr(strip_idx, strip_len);
        if (round_up) {
            m_integer++;
        }
    }
    //Group 3: additional exponent
    if (regex_match[3].matched) {
        m_exponent += std::stoll(regex_match[3].str().substr(1));
    }
    m_integer.m_positive = positive;
    return true;
}

bool BigDecimal::from_string(char const* str) {
    m_integer = BIG_INTEGER_ZERO;
    m_exponent = 0;
    std::cmatch regex_match;
    if (!std::regex_match(str, regex_match, BIG_DECIMAL_REGEX)) {
        return false;
    }
    // Group 1: Sign
    bool positive = true;
    if (regex_match[1].matched and regex_match[1].str() == "-") {
        positive = false;
    }
    // Group 2: Significant digits
    if (regex_match[2].matched) {
        std::string match_str = regex_match[2].str();
        std::string::const_iterator lstrip_iterator = match_str.begin();
        while (*lstrip_iterator == '0') {
            lstrip_iterator++;
        }
        std::string::size_type point_pos = match_str.find('.');
        if (point_pos != std::string::npos) {
            m_exponent = (long long)point_pos - 1;
            if (point_pos == lstrip_iterator - match_str.begin() and lstrip_iterator != match_str.begin()) {
                m_exponent -= lstrip_iterator - match_str.begin();
            }
            match_str.erase(point_pos, 1);
        }
        else {
            m_exponent = match_str.length() - 1;
        }
        bool round_up = false;
        if (match_str.length() > m_significant_digits) {
            if (match_str[m_significant_digits] >= '5') {
                round_up = true;
            }
            match_str.erase(m_significant_digits);
        }
        std::string::const_reverse_iterator rstrip_iterator = match_str.rbegin();
        while (*rstrip_iterator == '0') {
            rstrip_iterator++;
        }
        std::string::size_type strip_idx = lstrip_iterator - match_str.cbegin();
        std::string::size_type strip_len = rstrip_iterator.base() - lstrip_iterator;
        m_integer = match_str.substr(strip_idx, strip_len);
        if (round_up) {
            m_integer++;
        }
    }
    //Group 3: additional exponent
    if (regex_match[3].matched) {
        m_exponent += std::stoll(regex_match[3].str().substr(1));
    }
    m_integer.m_positive = positive;
    return true;
}

BigDecimal::BigDecimal() {
    m_integer = BIG_INTEGER_ZERO;
    m_exponent = 0;
}
BigDecimal::BigDecimal(BigDecimal const& x) {
    m_integer = x.m_integer;
    m_exponent = x.m_exponent;
}
BigDecimal::BigDecimal(BigDecimal&& x) {
    m_integer = std::move(x.m_integer);
    m_exponent = std::move(x.m_exponent);
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
BigDecimal::BigDecimal(BigInteger const& x) {
    m_integer = x;
    if (m_integer.is_zero()) {
        m_exponent = 0;
    }
    else {
        m_exponent = m_integer.get_decimal_digits() - 1;
    }
    reduce_integer(m_integer);
}
BigDecimal::BigDecimal(BigInteger&& x) {
    m_integer = std::move(x);
    if (m_integer.is_zero()) {
        m_exponent = 0;
    }
    else {
        m_exponent = m_integer.get_decimal_digits() - 1;
    }
    reduce_integer(m_integer);
}

BigDecimal& BigDecimal::operator=(BigDecimal const& x) {
    m_integer = x.m_integer;
    m_exponent = x.m_exponent;
    return *this;
}
BigDecimal& BigDecimal::operator=(BigDecimal&& x) {
    m_integer = std::move(x.m_integer);
    m_exponent = std::move(x.m_exponent);
    return *this;
}
BigDecimal& BigDecimal::operator=(char const* c_string) {
    if (!from_string(c_string)) {
        m_integer = BIG_INTEGER_ZERO;
        m_exponent = 0;
    }
    return *this;
}
BigDecimal& BigDecimal::operator=(BigInteger const& x) {
    m_integer = x;
    if (m_integer.is_zero()) {
        m_exponent = 0;
    }
    else {
        m_exponent = m_integer.get_decimal_digits() - 1;
    }
    reduce_integer(m_integer);
    return *this;
}
BigDecimal& BigDecimal::operator=(BigInteger&& x) {
    m_integer = std::move(x);
    if (m_integer.is_zero()) {
        m_exponent = 0;
    }
    else {
        m_exponent = m_integer.get_decimal_digits() - 1;
    }
    reduce_integer(m_integer);
    return *this;
}
BigDecimal& BigDecimal::operator=(std::string const& string) {
    if (!from_string(string)) {
        m_integer = BIG_INTEGER_ZERO;
        m_exponent = 0;
    }
    return *this;
}

BigDecimal& BigDecimal::operator+=(BigDecimal const& x) {
    if (x.is_zero()) {
        return *this;
    }
    if (this->is_zero()) {
        m_integer = x.m_integer;
        m_exponent = x.m_exponent;
        return *this;
    }
    //x is small enough to neglect
    if (m_exponent > (long long)m_significant_digits + x.m_exponent) {
        return *this;
    }
    //this is small enough to replace with x
    if (x.m_exponent > (long long)m_significant_digits + m_exponent) {
        this->m_integer = x.m_integer;
        this->m_exponent = x.m_exponent;
        return *this;
    }
    long long this_frac_len = this->m_integer.get_decimal_digits();
    long long x_frac_len = x.m_integer.get_decimal_digits();
    this_frac_len -= m_exponent + 1;
    x_frac_len -= x.m_exponent + 1;
    long long fraction_len = std::max(this_frac_len, x_frac_len);
    if (this_frac_len < x_frac_len) {
        m_integer = this->m_integer * pow_10(x_frac_len - this_frac_len) + x.m_integer;
    }
    else if (this_frac_len > x_frac_len) {
        m_integer = this->m_integer + x.m_integer * pow_10(this_frac_len - x_frac_len);
    }
    else {
        m_integer = this->m_integer + x.m_integer;
    }
    m_exponent = (long long)m_integer.get_decimal_digits() - fraction_len - 1;
    reduce_integer(m_integer);
    return *this;
}

BigDecimal& BigDecimal::operator-=(BigDecimal const& x) {
    if (x.is_zero()) {
        return *this;
    }
    if (this->is_zero()) {
        m_integer = x.m_integer;
        m_exponent = x.m_exponent;
        return *this;
    }
    //x is small enough to neglect
    if (m_exponent > (long long)m_significant_digits + x.m_exponent) {
        return *this;
    }
    //this is small enough to replace with x
    if (x.m_exponent > (long long)m_significant_digits + m_exponent) {
        this->m_integer = x.m_integer;
        this->m_exponent = x.m_exponent;
        return *this;
    }
    long long this_frac_len = this->m_integer.get_decimal_digits();
    long long x_frac_len = x.m_integer.get_decimal_digits();
    this_frac_len -= m_exponent + 1;
    x_frac_len -= x.m_exponent + 1;
    long long fraction_len = std::max(this_frac_len, x_frac_len);
    if (this_frac_len < x_frac_len) {
        m_integer = this->m_integer * pow_10(x_frac_len - this_frac_len) - x.m_integer;
    }
    else if (this_frac_len > x_frac_len) {
        m_integer = this->m_integer - x.m_integer * pow_10(this_frac_len - x_frac_len);
    }
    else {
        m_integer = this->m_integer - x.m_integer;
    }
    m_exponent = (long long)m_integer.get_decimal_digits() - fraction_len - 1;
    reduce_integer(m_integer);
    return *this;
}
BigDecimal& BigDecimal::operator*=(BigDecimal const& x) {
    std::size_t this_decimal_digits = this->m_integer.get_decimal_digits();
    std::size_t x_decimal_digits = x.m_integer.get_decimal_digits();
    this->m_integer *= x.m_integer;
    this->m_exponent += x.m_exponent;
    std::size_t result_decimal_digits = this->m_integer.get_decimal_digits();
    if (result_decimal_digits + 1 > x_decimal_digits + this_decimal_digits) {
        this->m_exponent++;
    }
    reduce_integer(m_integer);
    return *this;
}
BigDecimal& BigDecimal::operator/=(BigDecimal const& x) {
    if (x.is_zero()) {
        throw std::domain_error("Division by zero.");
    }
    if (this->is_zero()) {
        return *this;
    }
    std::size_t this_decimal_digits = this->m_integer.get_decimal_digits();
    std::size_t x_decimal_digits = x.m_integer.get_decimal_digits();
    if (this_decimal_digits > x_decimal_digits) {
        this->m_integer *= pow_10(m_significant_digits);
        auto divisor = x.m_integer * pow_10(this_decimal_digits - x_decimal_digits);
        BigInteger remainder = this->m_integer.div(divisor);
        remainder <<= 1;
        if (remainder > BigInteger::abs(divisor)) {
            this->m_integer++;
        }
        else if (remainder == BigInteger::abs(divisor) and this->m_integer.is_odd()) {
            this->m_integer++;
        }
    }
    else if (this_decimal_digits < x_decimal_digits) {
        this->m_integer *= pow_10(m_significant_digits + (x_decimal_digits - this_decimal_digits));
        BigInteger remainder = this->m_integer.div(x.m_integer);
        remainder <<= 1;
        if (remainder > BigInteger::abs(x.m_integer)) {
            this->m_integer++;
        }
        else if (remainder == BigInteger::abs(x.m_integer) and this->m_integer.is_odd()) {
            this->m_integer++;
        }
    }
    else {
        this->m_integer *= pow_10(m_significant_digits);
        BigInteger remainder = this->m_integer.div(x.m_integer);
        remainder <<= 1;
        if (remainder > BigInteger::abs(x.m_integer)) {
            this->m_integer++;
        }
        else if (remainder == BigInteger::abs(x.m_integer) and this->m_integer.is_odd()) {
            this->m_integer++;
        }
    }
    this->m_exponent -= x.m_exponent;
    std::size_t division_decimal_digits = this->m_integer.get_decimal_digits();
    if (division_decimal_digits <= m_significant_digits) {
        this->m_exponent--;
    }
    reduce_integer(m_integer);
    return *this;
}

BigDecimal BigDecimal::operator+() const {
    BigDecimal result = *this;
    return result;
}
BigDecimal BigDecimal::operator-() const {
    BigDecimal result = *this;
    result.m_integer.m_positive = !result.m_integer.m_positive;
    return result;
}

BigDecimal BigDecimal::operator+(BigDecimal const& x) const {
    BigDecimal result = *this;
    result += x;
    return result;
}
BigDecimal BigDecimal::operator-(BigDecimal const& x) const {
    BigDecimal result = *this;
    result -= x;
    return result;
}
BigDecimal BigDecimal::operator*(BigDecimal const& x) const {
    BigDecimal result = *this;
    result *= x;
    return result;
}
BigDecimal BigDecimal::operator/(BigDecimal const& x) const {
    BigDecimal result = *this;
    result /= x;
    return result;
}

bool BigDecimal::operator<(BigDecimal const& x) const {
    if (this->is_zero()) {
        return x.is_positive();
    }
    if (x.is_zero()) {
        return !this->m_integer.m_positive;
    }
    if (this->m_integer.m_positive == x.m_integer.m_positive) {
        if (this->m_exponent < x.m_exponent) return this->m_integer.m_positive;
        else if (this->m_exponent > x.m_exponent) return !this->m_integer.m_positive;
        std::stack<unsigned long> this_digits, x_digits;
        BigInteger::IntegerData this_data = this->m_integer.m_data;
        BigInteger::IntegerData x_data = x.m_integer.m_data;
        while (!BigInteger::bitset_is_zero(this_data)) {
            this_digits.push(BigInteger::bitset_divide_10(this_data));
        }
        while (!BigInteger::bitset_is_zero(x_data)) {
            x_digits.push(BigInteger::bitset_divide_10(x_data));
        }
        while (!this_digits.empty() or !x_digits.empty()) {
            unsigned long this_digit = this_digits.empty() ? 0ul : this_digits.top();
            unsigned long x_digit = x_digits.empty() ? 0ul : x_digits.top();
            if (this_digit < x_digit) {
                return this->m_integer.m_positive;
            }
            else if (this_digit > x_digit) {
                return !this->m_integer.m_positive;
            }
            if (!this_digits.empty()) this_digits.pop();
            if (!x_digits.empty()) x_digits.pop();
        }
        return false;
    }
    else return !this->m_integer.m_positive;
}
bool BigDecimal::operator>(BigDecimal const& x) const {
    if (this->is_zero()) {
        return x.is_negative();
    }
    if (x.is_zero()) {
        return this->m_integer.m_positive;
    }
    if (this->m_integer.m_positive == x.m_integer.m_positive) {
        if (this->m_exponent < x.m_exponent) return !this->m_integer.m_positive;
        else if (this->m_exponent > x.m_exponent) return this->m_integer.m_positive;
        std::stack<unsigned long> this_digits, x_digits;
        BigInteger::IntegerData this_data = this->m_integer.m_data;
        BigInteger::IntegerData x_data = x.m_integer.m_data;
        while (!BigInteger::bitset_is_zero(this_data)) {
            this_digits.push(BigInteger::bitset_divide_10(this_data));
        }
        while (!BigInteger::bitset_is_zero(x_data)) {
            x_digits.push(BigInteger::bitset_divide_10(x_data));
        }
        while (!this_digits.empty() or !x_digits.empty()) {
            unsigned long this_digit = this_digits.empty() ? 0ul : this_digits.top();
            unsigned long x_digit = x_digits.empty() ? 0ul : x_digits.top();
            if (this_digit > x_digit) {
                return this->m_integer.m_positive;
            }
            else if (this_digit < x_digit) {
                return !this->m_integer.m_positive;
            }
            if (!this_digits.empty()) this_digits.pop();
            if (!x_digits.empty()) x_digits.pop();
        }
        return false;
    }
    else return this->m_integer.m_positive;
}
bool BigDecimal::operator<=(BigDecimal const& x) const {
    if (this->is_zero()) {
        return !x.is_negative();
    }
    if (x.is_zero()) {
        return !this->m_integer.m_positive;
    }
    if (this->m_integer.m_positive == x.m_integer.m_positive) {
        if (this->m_exponent < x.m_exponent) return this->m_integer.m_positive;
        else if (this->m_exponent > x.m_exponent) return !this->m_integer.m_positive;
        std::stack<unsigned long> this_digits, x_digits;
        BigInteger::IntegerData this_data = this->m_integer.m_data;
        BigInteger::IntegerData x_data = x.m_integer.m_data;
        while (!BigInteger::bitset_is_zero(this_data)) {
            this_digits.push(BigInteger::bitset_divide_10(this_data));
        }
        while (!BigInteger::bitset_is_zero(x_data)) {
            x_digits.push(BigInteger::bitset_divide_10(x_data));
        }
        while (!this_digits.empty() or !x_digits.empty()) {
            unsigned long this_digit = this_digits.empty() ? 0ul : this_digits.top();
            unsigned long x_digit = x_digits.empty() ? 0ul : x_digits.top();
            if (this_digit < x_digit) {
                return this->m_integer.m_positive;
            }
            else if (this_digit > x_digit) {
                return !this->m_integer.m_positive;
            }
            if (!this_digits.empty()) this_digits.pop();
            if (!x_digits.empty()) x_digits.pop();
        }
        return true;
    }
    else return !this->m_integer.m_positive;
}
bool BigDecimal::operator>=(BigDecimal const& x) const {
    if (this->is_zero()) {
        return !x.is_positive();
    }
    if (x.is_zero()) {
        return this->m_integer.m_positive;
    }
    if (this->m_integer.m_positive == x.m_integer.m_positive) {
        if (this->m_exponent < x.m_exponent) return !this->m_integer.m_positive;
        else if (this->m_exponent > x.m_exponent) return this->m_integer.m_positive;
        std::stack<unsigned long> this_digits, x_digits;
        BigInteger::IntegerData this_data = this->m_integer.m_data;
        BigInteger::IntegerData x_data = x.m_integer.m_data;
        while (!BigInteger::bitset_is_zero(this_data)) {
            this_digits.push(BigInteger::bitset_divide_10(this_data));
        }
        while (!BigInteger::bitset_is_zero(x_data)) {
            x_digits.push(BigInteger::bitset_divide_10(x_data));
        }
        while (!this_digits.empty() or !x_digits.empty()) {
            unsigned long this_digit = this_digits.empty() ? 0ul : this_digits.top();
            unsigned long x_digit = x_digits.empty() ? 0ul : x_digits.top();
            if (this_digit > x_digit) {
                return this->m_integer.m_positive;
            }
            else if (this_digit < x_digit) {
                return !this->m_integer.m_positive;
            }
            if (!this_digits.empty()) this_digits.pop();
            if (!x_digits.empty()) x_digits.pop();
        }
        return true;
    }
    else return this->m_integer.m_positive;
}
bool BigDecimal::operator==(BigDecimal const& x) const {
    if (x.m_exponent != this->m_exponent) {
        return false;
    }
    if (x.m_integer.is_zero()) {
        return this->m_integer.is_zero();
    }
    if (this->m_integer.is_zero()) {
        return false;
    }
    std::stack<unsigned long> this_digits, x_digits;
    BigInteger::IntegerData this_data = this->m_integer.m_data;
    BigInteger::IntegerData x_data = x.m_integer.m_data;
    while (!BigInteger::bitset_is_zero(this_data)) {
        this_digits.push(BigInteger::bitset_divide_10(this_data));
    }
    while (!BigInteger::bitset_is_zero(x_data)) {
        x_digits.push(BigInteger::bitset_divide_10(x_data));
    }
    while (!this_digits.empty() or !x_digits.empty()) {
        unsigned long this_digit = this_digits.empty() ? 0ul : this_digits.top();
        unsigned long x_digit = x_digits.empty() ? 0ul : x_digits.top();
        if (this_digit != x_digit) {
            return false;
        }
        if (!this_digits.empty()) this_digits.pop();
        if (!x_digits.empty()) x_digits.pop();
    }
    return m_integer.m_positive == x.m_integer.m_positive;
}
bool BigDecimal::operator!=(BigDecimal const& x) const {
    if (x.m_exponent != this->m_exponent) {
        return true;
    }
    if (x.m_integer.is_zero()) {
        return !this->m_integer.is_zero();
    }
    if (this->m_integer.is_zero()) {
        return true;
    }
    std::stack<unsigned long> this_digits, x_digits;
    BigInteger::IntegerData this_data = this->m_integer.m_data;
    BigInteger::IntegerData x_data = x.m_integer.m_data;
    while (!BigInteger::bitset_is_zero(this_data)) {
        this_digits.push(BigInteger::bitset_divide_10(this_data));
    }
    while (!BigInteger::bitset_is_zero(x_data)) {
        x_digits.push(BigInteger::bitset_divide_10(x_data));
    }
    while (!this_digits.empty() or !x_digits.empty()) {
        unsigned long this_digit = this_digits.empty() ? 0ul : this_digits.top();
        unsigned long x_digit = x_digits.empty() ? 0ul : x_digits.top();
        if (this_digit != x_digit) {
            return true;
        }
        if (!this_digits.empty()) this_digits.pop();
        if (!x_digits.empty()) x_digits.pop();
    }
    return m_integer.m_positive != x.m_integer.m_positive;
}

std::ostream& operator<<(std::ostream& os, BigDecimal const& x) {
    if (os.flags() & std::ios::showpos and x.is_positive()) {
        os << "+";
    }
    if (os.flags() & std::ios::scientific) {
        os << x.to_scientific_string(os.precision());
    }
    else if (os.flags() & std::ios::fixed) {
        os << x.to_fixed_string(os.precision());
    }
    else {
        os << x.to_string(os.precision());
    }
    return os;
}
std::istream& operator>>(std::istream& is, BigDecimal& x) {
    std::string y;
    is >> y;
    if (!x.from_string(y)) {
        is.setstate(std::ios::failbit);
    }
    return is;
}

bool BigDecimal::is_negative() const {
    return m_integer.is_negative();
}
bool BigDecimal::is_zero() const {
    return m_integer.is_zero();
}
bool BigDecimal::is_positive() const {
    return m_integer.is_positive();
}

void BigDecimal::set_significant_digits_capacity(std::size_t len) {
    m_significant_digits = len;
}
std::size_t BigDecimal::get_significant_digits_capacity() {
    return m_significant_digits;
}

BigDecimal BigDecimal::abs(BigDecimal const& x) {
    return x.is_negative() ? -x : x;
}
BigDecimal BigDecimal::pow(BigDecimal const& x, std::size_t n) {
    if (n == 0) return "1.0";
    if (n == 1) return x;
    if (n == 2) return x * x;
    if (n == 3) return x * x * x;
    BigDecimal half_pow = BigDecimal::pow(x, n / 2);
    if (n % 2 == 0) return half_pow * half_pow;
    else return half_pow * half_pow * x;
}
BigDecimal BigDecimal::sqrt(BigDecimal const& x) {
    if (x.is_negative()) throw std::domain_error("Cannot calculate square root of a negative number.");
    if (x.is_zero()) return "0.0";
    BigDecimal target = x;
    long long sqrt_exponent = target.m_exponent < 0 ? -(-target.m_exponent / 2 + 1) : target.m_exponent / 2;
    target.m_exponent -= sqrt_exponent * 2;
    //Now target is not less than 1 and less than 100.
    BigDecimal result = "1e+1";
    BigDecimal prev_result;
    do {
        prev_result = std::move(result);
        result = (prev_result + target / prev_result) / "2";
    }
    while (result < prev_result);
    result.m_exponent += sqrt_exponent;
    return result;
}
BigDecimal BigDecimal::cbrt(BigDecimal const& x) {
    if (x.is_zero()) return "0.0";
    BigDecimal target = x;
    long long cbrt_exponent = target.m_exponent < 0 ? -(-target.m_exponent / 3 + 1) : target.m_exponent / 3;
    target.m_exponent -= cbrt_exponent * 3;
    //Now target is not less than 1 and less than 1000.
    BigDecimal result = x.is_positive() ? "10.0" : "-10.0";
    BigDecimal prev_result;
    do {
        prev_result = std::move(result);
        result = (prev_result * "2" + target / (prev_result * prev_result)) / "3";
    }
    while (result < prev_result);
    result.m_exponent += cbrt_exponent;
    return result;
}
BigDecimal BigDecimal::euler_number() {
    static std::size_t previous_precision = 0;
    static BigDecimal euler;
    if (previous_precision != BigDecimal::m_significant_digits) {
        previous_precision = BigDecimal::m_significant_digits;
        // Recursive formula: f(n)=1+f(n+1)/n
        // Find n such that e/(n+1)! < 10e-{precision + 1}
        // ln(x) approx. x*ln(x)-x
        auto error_term_func = [](double x, std::size_t precision) {
            return static_cast<double>(precision + 1) + std::log10(2.718281828459045) + (x - x * std::log(x)) / std::log(10.0);
        };
        auto error_term_func_deriv = [](double x) {
            return -std::log10(x);
        };
        //Newton-Rhapson
        double number_of_terms = 1e6;
        double number_of_terms_step;
        do {
            number_of_terms_step = -error_term_func(number_of_terms, previous_precision) / error_term_func_deriv(number_of_terms);
            number_of_terms += number_of_terms_step;
        }
        while (std::abs(number_of_terms_step) >= 1e-10); 
        BigInteger n = BigInteger(std::to_string(static_cast<int>(std::ceil(number_of_terms))));
        n++;
        euler = "1.0";
        while (n --> BIG_INTEGER_ZERO) {
            euler /= (n + BIG_INTEGER_ONE);
            euler += "1";
        }
        euler += "1";
    }
    return euler;
}
BigDecimal BigDecimal::exp(BigDecimal const& x) {
    if (x.is_zero()) return "1.0";
    if (x == "1") return euler_number();
    if (x == "-1") return BigDecimal("1") / euler_number();
    if (x.m_exponent < 0) {
        double number_of_terms = 1e6;
        // Recursive formula: f(n)=x+f(n+1)/n
        // ln(x) approx. x*ln(x)-x
        if (x.m_integer.m_positive) {
            // Find n such that e/(n+1)! < 10e-{precision}
            auto error_term_func = [](double x, std::size_t precision) {
                return static_cast<double>(precision + 1) + std::log10(2.718281828459045) + (x - x * std::log(x)) / std::log(10.0);
            };
            auto error_term_func_deriv = [](double x) {
                return -std::log10(x);
            };
            //Newton-Rhapson
            double number_of_terms_step;
            do {
                number_of_terms_step = -error_term_func(number_of_terms, m_significant_digits) / error_term_func_deriv(number_of_terms);
                number_of_terms += number_of_terms_step;
            }
            while (std::abs(number_of_terms_step) >= 1e-10); 
            BigInteger n = BigInteger(std::to_string(static_cast<int>(std::ceil(number_of_terms))));
            n++;
            BigDecimal result = "1";
            while (--n > BIG_INTEGER_ZERO) {
                result *= x;
                result /= n;
                result += "1";
            }
            return result;
        }
        else {
            // Find n such that 1/(n+1)! < 10e-{precision + 1}
            auto error_term_func = [](double x, std::size_t precision) {
                return static_cast<double>(precision + 1) + (x - x * std::log(x)) / std::log(10.0);
            };
            auto error_term_func_deriv = [](double x) {
                return -std::log10(x);
            };
            //Newton-Rhapson
            double number_of_terms_step;
            do {
                number_of_terms_step = -error_term_func(number_of_terms, m_significant_digits + 1) / error_term_func_deriv(number_of_terms);
                number_of_terms += number_of_terms_step;
            }
            while (std::abs(number_of_terms_step) >= 1e-10); 
            BigInteger n = BigInteger(std::to_string(static_cast<int>(std::ceil(number_of_terms))));
            n++;
            BigDecimal result = "1";
            while (--n > BIG_INTEGER_ZERO) {
                result *= -x;
                result /= n;
                result += "1";
            }
            return BigDecimal("1") / result;
        }
    }
    BigDecimal half_exp = BigDecimal::exp(x / "2");
    return half_exp * half_exp;
}
BigDecimal BigDecimal::ln(BigDecimal const& x) {
    if (!x.is_positive()) throw std::domain_error("Logarithm of a negative number is not defined.");
    if (x == "1") return "0";
    if (x > "1") return -ln(BigDecimal("1") / x);
    BigDecimal inv_e = BigDecimal("1") / euler_number();
    BigDecimal new_x = x;
    BigInteger result_1 = "0";
    while (new_x < inv_e) {
        result_1--;
        new_x *= euler_number();
    }
    //Newton-Rhapson
    BigDecimal result_2 = "0";
    BigDecimal prev_result;
    do {
        prev_result = std::move(result_2);
        BigDecimal exp_result_2 = BigDecimal::exp(prev_result);
        result_2 = prev_result + (new_x - exp_result_2) / (new_x + exp_result_2) * "2";
    }
    while (result_2 < prev_result);
    return prev_result + result_1;
}
}