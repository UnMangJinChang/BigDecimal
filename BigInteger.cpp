#include "BigInteger.hpp"

constexpr std::size_t CHAR_BITS = std::numeric_limits<unsigned char>::digits;
constexpr std::size_t ULONG_BITS = sizeof(unsigned long) * CHAR_BITS;
static std::regex BIG_INTEGER_REGEX = std::regex("^[+-]?[0-9]+$");

namespace umjc {
BigInteger const BIG_INTEGER_ZERO("0");
BigInteger const BIG_INTEGER_ONE("1");
BigInteger const BIG_INTEGER_TWO("2");
BigInteger const BIG_INTEGER_THREE("3");
BigInteger const BIG_INTEGER_FOUR("4");
BigInteger const BIG_INTEGER_FIVE("5");
BigInteger const BIG_INTEGER_SIX("6");
BigInteger const BIG_INTEGER_SEVEN("7");
BigInteger const BIG_INTEGER_EIGHT("8");
BigInteger const BIG_INTEGER_NINE("9");
BigInteger const BIG_INTEGER_TEN("10");
BigInteger const BIG_INTEGER_HUNDRED("100");
BigInteger const BIG_INTEGER_THOUSAND("1000");

unsigned long ulong_high(unsigned long x) {
    return x >> (ULONG_BITS / 2);
}
unsigned long ulong_low(unsigned long x) {
    return x & ((1ul << (ULONG_BITS / 2)) - 1ul);
}

void BigInteger::reset() {
    m_data = IntegerData(1, 0ul);
}
void BigInteger::bitset_increment(IntegerData& x) {
    if (x.empty()) {
        x = IntegerData(1, 1ul);
    }
    for (std::size_t i = 0; i < x.size(); i++) {
        x[i]++;
        if (x[i] != 0ul) break;
    }
    if (x.back() == 0ul) {
        x.push_back(1ul);
    }
    erase_leading_zeroes(x);
}
void BigInteger::bitset_decrement(IntegerData& x) {
    for (std::size_t i = 0; i < x.size(); i++) {
        x[i]--;
        if (x[i] != -1ul) break;
    }
    erase_leading_zeroes(x);
}

void BigInteger::erase_leading_zeroes(IntegerData& x) {
    while (x.size() > 1 and x.back() == 0ul) {
        x.pop_back();
    }
}
bool BigInteger::bitset_is_zero(IntegerData const& x) {
    return std::count(x.begin(), x.end(), 0ul) == x.size();
}

void BigInteger::bitset_shift_left(IntegerData& x, std::size_t n) {
    erase_leading_zeroes(x);
    if (bitset_is_zero(x)) return;
    if (n / ULONG_BITS != 0) x.insert(x.begin(), n / ULONG_BITS, 0ul);
    std::size_t pre_shifted = n / ULONG_BITS;
    n %= ULONG_BITS;
    if (n == 0) return;
    unsigned long cropped = 0ul;
    for (std::size_t i = pre_shifted; i < x.size(); i++) {
        unsigned long new_cropped = x[i] >> (ULONG_BITS - n);
        x[i] <<= n;
        x[i] |= cropped;
        cropped = new_cropped;
    }
    if (cropped != 0ul) x.push_back(cropped);
} 
void BigInteger::bitset_shift_right(IntegerData& x, std::size_t n) {
    if (n / ULONG_BITS >= x.size()) {
        x = IntegerData(1, 0ul);
        return;
    }
    if (n / ULONG_BITS != 0) {
        x.erase(x.begin(), x.begin() + n / ULONG_BITS);
    }
    n %= ULONG_BITS;
    if (n == 0) return;
    unsigned long cropped = 0ul;
    for (std::size_t i = 1; i <= x.size(); i++) {
        unsigned long new_cropped = x[x.size() - i] & ((1ul << n) - 1ul);
        x[x.size() - i] >>= n;
        x[x.size() - i] |=  (cropped << (ULONG_BITS - n));
        cropped = new_cropped;
    }
    erase_leading_zeroes(x);
}
void BigInteger::negate() {
    m_positive = !m_positive;
}
void BigInteger::bitset_add(IntegerData& a, IntegerData const& b, std::size_t b_left_shift) {
    if (bitset_is_zero(b)) {
        return;
    }
    if (a.size() < b.size() + b_left_shift) {
        a.resize(b.size() + b_left_shift, 0ul);
    }
    bool carry = false;
    for (std::size_t i = b_left_shift; i < a.size(); i++) {
        bool new_carry = false;
        if (carry) {
            new_carry = a[i] == -1ul;
            a[i]++;
        }
        if (i - b_left_shift < b.size()) {
            new_carry = a[i] + b[i - b_left_shift] < a[i];
        }
        carry = new_carry;
        if (i - b_left_shift < b.size()) {
            a[i] += b[i - b_left_shift];
        }
        if (!carry and i - b_left_shift >= b.size()) break;
    }
    if (carry) {
        a.push_back(1ul);
    }
}
//Caution: only use if a is greater than b (with left shift considered)
void BigInteger::bitset_subtract(IntegerData& a, IntegerData const& b, std::size_t b_left_shift) {
    if (bitset_is_zero(b)) {
        return;
    }
    bool borrow = false;
    for (std::size_t i = b_left_shift; i < a.size(); i++) {
        bool new_borrow = false;
        if (borrow) {
            new_borrow = a[i] == 0ul;
            a[i]--;
        }
        if (i - b_left_shift < b.size()) {
            new_borrow = a[i] < b[i - b_left_shift];
        }
        borrow = new_borrow;
        if (i - b_left_shift < b.size()) {
            a[i] -= b[i - b_left_shift];
        }
        if (!borrow and i - b_left_shift >= b.size()) break;
    }
    erase_leading_zeroes(a);
}
unsigned long BigInteger::bitset_divide_10(IntegerData& a) {
    IntegerData quotient(a.size(), 0ul);
    for (std::size_t i = 0; i < quotient.size(); i++) {
        unsigned long a_digits_splitted[4];
        a_digits_splitted[0] = ulong_low (*(a.rbegin() + i));
        a_digits_splitted[1] = ulong_high(*(a.rbegin() + i));
        if (i > 0) {
            a_digits_splitted[2] = ulong_low (*(a.rbegin() + i - 1));
            a_digits_splitted[3] = ulong_high(*(a.rbegin() + i - 1));
        }
        else {
            a_digits_splitted[2] = 0ul;
            a_digits_splitted[3] = 0ul;
        }
        unsigned long remainder = 0ul;
        for (std::size_t j = 1; j <= 4; j++) {
            unsigned long dividing_digit = (remainder << (ULONG_BITS / 2)) + a_digits_splitted[4 - j];
            a_digits_splitted[4 - j] = dividing_digit / 10ul;
            remainder = dividing_digit % 10ul;
        }
        *(quotient.rbegin() + i) = (a_digits_splitted[1] << (ULONG_BITS / 2)) + a_digits_splitted[0];
        *(a.rbegin() + i) = remainder;
        if (i > 0) {
            *(a.rbegin() + i - 1) = (a_digits_splitted[3] << (ULONG_BITS / 2)) + a_digits_splitted[2];
        }
    }
    std::swap(a, quotient);
    erase_leading_zeroes(a);
    return quotient[0];
}
//https://en.wikipedia.org/wiki/Karatsuba_algorithm
BigInteger::IntegerData BigInteger::karatsuba(
    unsigned long const* a, std::size_t len_a, 
    unsigned long const* b, std::size_t len_b
) {
    if (len_a == 0 or len_b == 0) {
        return IntegerData(1, 0ul);
    }
    //Base case
    constexpr std::size_t BASE_CASE_SIZE = 128;
    if (len_a <= BASE_CASE_SIZE and len_b <= BASE_CASE_SIZE) {
        constexpr std::size_t HALF_BITS = ULONG_BITS / 2;
        IntegerData return_value(1, 0ul);
        //primary school style multiplication
        for (std::size_t i = 0; i < len_b; i++) {
            IntegerData adding;
            adding.reserve(len_a + 1);
            unsigned long carry = 0;
            for (std::size_t j = 0; j < len_a; j++) {
                //Calculate carry i.e. overflow
                //(A*2^32+B)(C*2^32+D)=(AC)*2^64+(AD+BC)*2^32+BD
                unsigned long x_high = ulong_high(a[j]), y_high = ulong_high(b[i]);
                unsigned long x_low = ulong_low(a[j]), y_low = ulong_low(b[i]);
                unsigned long digit = x_low * y_low;
                unsigned long second_1 = x_high * y_low;
                unsigned long second_2 = x_low * y_high;
                unsigned long second = second_1 + second_2;
                unsigned long second_high = ulong_high(second), second_low = ulong_low(second);
                if (second_1 + second_2 < second_1) second_high += (1ul << HALF_BITS);
                unsigned long new_carry = x_high * y_high + second_high;
                if ((second_low << HALF_BITS) + digit < digit) new_carry++;
                digit += second_low << HALF_BITS;
                if (carry + digit < digit) new_carry++;
                digit += carry;
                carry = new_carry;
                adding.push_back(digit);
            }
            if (carry != 0ul) adding.push_back(carry);
            bitset_add(return_value, adding, i);
        }
        return return_value;
    }
    std::size_t div = std::max(len_a, len_b) / 2;
    if (len_a > div and len_b > div) {
        auto a_low = a;
        auto a_high = a + div;
        auto b_low = b;
        auto b_high = b + div;

        IntegerData z_0 = karatsuba(a_low, div, b_low, div);
        IntegerData z_2 = karatsuba(a_high, len_a - div, b_high, len_b - div);

        IntegerData a_high_low_sum(std::max(div, len_a - div), 0ul);
        //Calculate a_high_low_sum
        {
            bool carry = false;
            for (std::size_t i = 0; i < a_high_low_sum.size(); i++) {
                unsigned long a_low_digit = i < div ? a_low[i] : 0ul;
                unsigned long a_high_digit = i < len_a - div ? a_high[i] : 0ul;
                bool new_carry = a_low_digit + a_high_digit < a_low_digit;
                a_high_low_sum[i] = a_low_digit + a_high_digit;
                if (carry) {
                    if (++a_high_low_sum[i] == 0ul) {
                        new_carry = true;
                    }
                }
                carry = new_carry;
            }
            if (carry) a_high_low_sum.push_back(1ul);
            else erase_leading_zeroes(a_high_low_sum);
        }
        IntegerData b_high_low_sum(std::max(div, len_b - div), 0ul);
        //Calculate b_high_low_sum
        {
            bool carry = false;
            for (std::size_t i = 0; i < b_high_low_sum.size(); i++) {
                unsigned long b_low_digit = i < div ? b_low[i] : 0ul;
                unsigned long b_high_digit = i < len_b - div ? b_high[i] : 0ul;
                bool new_carry = b_low_digit + b_high_digit < b_low_digit;
                b_high_low_sum[i] = b_low_digit + b_high_digit;
                if (carry) {
                    if (++b_high_low_sum[i] == 0ul) {
                        new_carry = true;
                    }
                }
                carry = new_carry;
            }
            if (carry) b_high_low_sum.push_back(1ul);
            else erase_leading_zeroes(b_high_low_sum);
        }
        IntegerData z_1 = karatsuba(a_high_low_sum.data(), a_high_low_sum.size(), b_high_low_sum.data(), b_high_low_sum.size());
        bitset_subtract(z_1, z_2);
        bitset_subtract(z_1, z_0);
        bitset_add(z_0, z_1, div);
        bitset_add(z_0, z_2, div * 2);
        return z_0;
    }
    else if (len_b > div) {
        auto b_low = b;
        auto b_high = b + div;
        IntegerData z_0 = karatsuba(a, len_a, b_low, div);
        IntegerData z_1 = karatsuba(a, len_a, b_high, len_b - div);
        bitset_add(z_0, z_1, div);
        return z_0;
    }
    else {
        auto a_low = a;
        auto a_high = a + div;
        IntegerData z_0 = karatsuba(a_low, div, b, len_b);
        IntegerData z_1 = karatsuba(a_high, len_a - div, b, len_b);
        bitset_add(z_0, z_1, div);
        return z_0;
    }
}
BigInteger::IntegerData BigInteger::bitset_divide(BigInteger::IntegerData& a, BigInteger::IntegerData const& b) {
    if (bitset_is_zero(b)) {
        throw std::domain_error("Division by zero.");
    }
    if (bitset_is_zero(a)) {
        return IntegerData(1, 0ul);
    }
    if (bitset_compare(BIG_INTEGER_ONE.get_data(), b) == 0) {
        return IntegerData(1, 0ul);
    }
    if (bitset_compare(a, b) < 0) {
        IntegerData remainder = std::move(a);
        a = IntegerData(1, 0ul);
        return remainder;
    }
    IntegerData quotient(a.size());
    for (std::size_t i = 1; i <= quotient.size(); i++) {
        if (bitset_compare(a, b, quotient.size() - i) < 0) {
            quotient[quotient.size() - i] = 0ul;
            continue;
        }
        unsigned long q_digit_lower = 1ul;
        unsigned long q_digit_upper = -1ul;
        IntegerData q_digit_times_b_lower = b;
        while (q_digit_upper - q_digit_lower > 1ul) {
            unsigned long q_digit_middle = q_digit_lower + (q_digit_upper - q_digit_lower) / 2ul;
            IntegerData q_digit_times_b = karatsuba(b.data(), b.size(), &q_digit_middle, 1);
            int compare = bitset_compare(a, q_digit_times_b, quotient.size() - i);
            if (compare < 0) {
                q_digit_upper = q_digit_middle;
            }
            else {
                q_digit_lower = q_digit_middle;
                q_digit_times_b_lower = std::move(q_digit_times_b);
            }
        }
        quotient[quotient.size() - i] = q_digit_lower;
        bitset_subtract(a, q_digit_times_b_lower, quotient.size() - i);
    }
    std::swap(quotient, a);
    erase_leading_zeroes(a);
    return quotient;
}

int BigInteger::bitset_compare(IntegerData const& a, IntegerData const& b, std::size_t b_left_shift) {
    std::size_t max_size = std::max(a.size(), b.size() + b_left_shift);
    for (std::size_t i = 1; i <= max_size; i++) {
        unsigned long a_digit = max_size < i + a.size() ? a[max_size - i] : 0ul;
        unsigned long b_digit = i + b_left_shift <= max_size and max_size < i + b.size() + b_left_shift ? b[max_size - i - b_left_shift] : 0ul;
        if (a_digit < b_digit) return -1;
        if (a_digit > b_digit) return 1;
    } 
    return 0;
}

std::string BigInteger::to_string() const {
    if (bitset_is_zero(m_data)) {
        return "0";
    }
    std::string rtrn_v;
    rtrn_v.reserve(static_cast<unsigned long>(std::ceil(static_cast<double>(m_data.size()) * (static_cast<double>(ULONG_BITS) * 0.3010299956639812))) + 1);
    IntegerData dividing = m_data;
    while (!bitset_is_zero(dividing)) {
        unsigned long digit = bitset_divide_10(dividing);
        rtrn_v.push_back('0' + (unsigned char)(digit));
    }
    if (!m_positive) rtrn_v.push_back('-');
    std::reverse(rtrn_v.begin(), rtrn_v.end());
    return rtrn_v;
}
//Takes only not signed positive number strings.
void BigInteger::from_string(std::string const& input) {
    reset();
    IntegerData ten(1, 10ul);
    for (auto it = input.begin(); it != input.end(); it++) {
        m_data = karatsuba(m_data.data(), m_data.size(), ten.data(), 1);
        IntegerData digit(1, static_cast<unsigned long>(*it - '0'));
        bitset_add(m_data, digit);
    }
}
//Takes only not signed positive number strings.
void BigInteger::from_string(char const* input) {
    reset();
    IntegerData ten(1, 10ul);
    for (auto it = input; *it != '\0'; it++) {
        m_data = karatsuba(m_data.data(), m_data.size(), ten.data(), 1);
        IntegerData digit(1, static_cast<unsigned long>(*it - '0'));
        bitset_add(m_data, digit);
    }
}


BigInteger::BigInteger() {
    reset();
}
BigInteger::BigInteger(BigInteger const& x) {
    this->m_data = x.m_data;
    this->m_positive = x.m_positive;
}
BigInteger::BigInteger(BigInteger&& x) {
    this->m_data = std::move(x.m_data);
    x.m_data = IntegerData(1, 0ul);
    this->m_positive = std::move(x.m_positive);
}
BigInteger::BigInteger(char const* c_str) {
    if (!std::regex_match(c_str, BIG_INTEGER_REGEX)) return; 
    if (c_str[0] != '+' and c_str[0] != '-') {
        from_string(c_str);
        m_positive = true;
    }
    else {
        from_string(c_str + 1);
        m_positive = c_str[0] == '+';
    }
}
BigInteger::BigInteger(std::string const& str) {
    if (!std::regex_match(str, BIG_INTEGER_REGEX)) return;
    if (str[0] != '+' and str[0] != '-') {
        from_string(str);
        m_positive = true;
    }
    else {
        from_string(str.substr(1));
        m_positive = str[0] == '+';
    }
}

//Assignments
BigInteger& BigInteger::operator=(BigInteger const& x) {
    this->m_data = x.m_data;
    this->m_positive = x.m_positive;
    return *this;
}
BigInteger& BigInteger::operator=(BigInteger&& x) {
    this->m_data = std::move(x.m_data);
    x.m_data = IntegerData(1, 0ul);
    this->m_positive = std::move(x.m_positive);
    return *this;
}
BigInteger& BigInteger::operator=(char const* c_str) {
    if (!std::regex_match(c_str, std::regex("^[+-]?[0-9]+$"))) return *this; 
    if (c_str[0] != '+' and c_str[0] != '-') {
        from_string(c_str);
        m_positive = true;
    }
    else {
        from_string(c_str + 1);
        m_positive = c_str[0] == '+';
    }
    return *this;
}
BigInteger& BigInteger::operator=(std::string const& str) {
    if (!std::regex_match(str, std::regex("^[+-]?[0-9]+$"))) return *this;
    if (str[0] != '+' and str[0] != '-') {
        from_string(str);
        m_positive = true;
    }
    else {
        from_string(str.data() + 1);
        m_positive = str[0] == '+';
    }
    return *this;
}
//Increment/Decrement Operators
BigInteger& BigInteger::operator++() {
    if (is_zero()) {
        m_data = IntegerData(1, 1ul);
        m_positive = true;
    }
    else if (is_positive()) {
        bitset_increment(m_data);
    }
    else {
        bitset_decrement(m_data);
    }
    return *this;
}
BigInteger& BigInteger::operator++(int) {
    if (is_zero()) {
        m_data = IntegerData(1, 1ul);
        m_positive = true;
    }
    else if (is_positive()) {
        bitset_increment(m_data);
    }
    else {
        bitset_decrement(m_data);
    }
    return *this;
}
BigInteger& BigInteger::operator--() {
    if (is_zero()) {
        m_data = IntegerData(1, 1ul);
        m_positive = false;
    }
    else if (is_positive()) {
        bitset_decrement(m_data);
    }
    else {
        bitset_increment(m_data);
    }
    return *this;
}
BigInteger& BigInteger::operator--(int) {
    if (is_zero()) {
        m_data = IntegerData(1, 1ul);
        m_positive = false;
    }
    else if (is_positive()) {
        bitset_decrement(m_data);
    }
    else {
        bitset_increment(m_data);
    }
    return *this;
}
//Arithmetic Operators
BigInteger& BigInteger::operator+=(BigInteger const& y) {
    if (y.is_zero()) return *this;
    if (this->is_zero()) {
        m_positive = y.m_positive;
        m_data = y.m_data;
    }
    else if (m_positive == y.m_positive) {
        bitset_add(m_data, y.m_data);
    }
    else {
        int compare = bitset_compare(m_data, y.m_data);
        if (compare == 0) {
            this->reset();
        }
        else if (compare > 0) {
            bitset_subtract(m_data, y.m_data);
        }
        else {
            IntegerData subtracting = std::move(m_data);
            m_data = y.m_data;
            bitset_subtract(m_data, subtracting);
            m_positive = y.m_positive;
        }
    }
    return *this;
}
BigInteger& BigInteger::operator-=(BigInteger const& y) {
    if (y.is_zero()) return *this;
    if (this->is_zero()) {
        m_positive = !y.m_positive;
        m_data = y.m_data;
    }
    else if (m_positive != y.m_positive) {
        bitset_add(m_data, y.m_data);
    }
    else {
        int compare = bitset_compare(m_data, y.m_data);
        if (compare == 0) {
            this->reset();
        }
        else if (compare > 0) {
            bitset_subtract(m_data, y.m_data);
        }
        else {
            IntegerData cache = std::move(m_data);
            m_data = y.m_data;
            bitset_subtract(m_data, cache);
            m_positive = !y.m_positive;
        }
    }
    return *this;
}
BigInteger& BigInteger::operator*=(BigInteger const& y) {
    m_data = karatsuba(m_data.data(), m_data.size(), y.m_data.data(), y.m_data.size());
    erase_leading_zeroes(m_data);
    m_positive = m_positive == y.m_positive;
    return *this;
}

BigInteger& BigInteger::operator/=(BigInteger const& y) {
    if (is_negative()) {
        if (!bitset_is_zero(bitset_divide(m_data, y.m_data))) {
            bitset_increment(m_data);
        }
    }
    else {
        bitset_divide(m_data, y.m_data);
    }
    m_positive = m_positive == y.m_positive;
    return *this;
}

BigInteger& BigInteger::operator%=(BigInteger const& y) {
    bool x_was_negative = this->is_negative();
    IntegerData absolute_remainder = bitset_divide(this->m_data, y.m_data);
    bool remainder_is_zero = bitset_is_zero(absolute_remainder);
    if (!remainder_is_zero and x_was_negative) {
        m_data = y.m_data;
        bitset_subtract(m_data, absolute_remainder);
    }
    else {
        m_data = std::move(absolute_remainder);
    }
    m_positive = true;
    return *this;
}
BigInteger& BigInteger::operator<<=(std::size_t n) {
    bitset_shift_left(m_data, n);
    return *this;
}
BigInteger& BigInteger::operator>>=(std::size_t n) {
    bitset_shift_right(m_data, n);
    return *this;
}

BigInteger BigInteger::operator+(BigInteger const& y) const {
    BigInteger rtrn_v = *this;
    rtrn_v += y;
    return rtrn_v;
}

BigInteger BigInteger::operator-(BigInteger const& y) const {
    BigInteger rtrn_v = *this;
    rtrn_v -= y;
    return rtrn_v;
}

BigInteger BigInteger::operator*(BigInteger const& y) const {
    BigInteger rtrn_v = *this;
    rtrn_v *= y;
    return rtrn_v;
}

BigInteger BigInteger::operator/(BigInteger const& y) const {
    BigInteger rtrn_v = *this;
    rtrn_v /= y;
    return rtrn_v;
}

BigInteger BigInteger::operator%(BigInteger const& y) const {
    BigInteger rtrn_v = *this;
    rtrn_v %= y;
    return rtrn_v;
}
BigInteger BigInteger::operator<<(std::size_t n) const {
    BigInteger rtrn_v = *this;
    rtrn_v <<= n;
    return rtrn_v;
}
BigInteger BigInteger::operator>>(std::size_t n) const {
    BigInteger rtrn_v = *this;
    rtrn_v >>= n;
    return rtrn_v;
}

BigInteger BigInteger::operator+() const {
    BigInteger result = *this;
    return result;
}
BigInteger BigInteger::operator-() const {
    BigInteger result = *this;
    result.negate();
    return result;
}

//Comparison Operators
bool BigInteger::operator<(BigInteger const& y) const {
    if (this->is_zero() and y.is_zero()) return false;
    if (this->is_negative() and !y.is_negative()) return true;
    if (!this->is_negative() and y.is_negative()) return false;
    if (this->m_data.size() < y.m_data.size()) return m_positive;
    if (this->m_data.size() > y.m_data.size()) return !m_positive;
    for (std::size_t i = 1; i <= m_data.size(); i++) {
        if (m_data[m_data.size() - i] < y.m_data[m_data.size() - i]) return m_positive;
        if (m_data[m_data.size() - i] > y.m_data[m_data.size() - i]) return !m_positive;
    }
    return false;
}

bool BigInteger::operator>(BigInteger const& y) const {
    if (this->is_zero() and y.is_zero()) return false;
    if (this->is_negative() and !y.is_negative()) return false;
    if (!this->is_negative() and y.is_negative()) return true;
    if (this->m_data.size() < y.m_data.size()) return !m_positive;
    if (this->m_data.size() > y.m_data.size()) return m_positive;
    for (std::size_t i = 1; i <= m_data.size(); i++) {
        if (m_data[m_data.size() - i] < y.m_data[m_data.size() - i]) return !m_positive;
        if (m_data[m_data.size() - i] > y.m_data[m_data.size() - i]) return m_positive;
    }
    return false;
}

bool BigInteger::operator<=(BigInteger const& y) const {
    if (this->is_zero() and y.is_zero()) return true;
    if (this->is_negative() and !y.is_negative()) return true;
    if (!this->is_negative() and y.is_negative()) return false;
    if (this->m_data.size() < y.m_data.size()) return m_positive;
    if (this->m_data.size() > y.m_data.size()) return !m_positive;
    for (std::size_t i = 1; i <= m_data.size(); i++) {
        if (m_data[m_data.size() - i] < y.m_data[m_data.size() - i]) return m_positive;
        if (m_data[m_data.size() - i] > y.m_data[m_data.size() - i]) return !m_positive;
    }
    return true;
}

bool BigInteger::operator>=(BigInteger const& y) const {
    if (this->is_zero() and y.is_zero()) return true;
    if (this->is_negative() and !y.is_negative()) return false;
    if (!this->is_negative() and y.is_negative()) return true;
    if (this->m_data.size() < y.m_data.size()) return !m_positive;
    if (this->m_data.size() > y.m_data.size()) return m_positive;
    for (std::size_t i = 1; i <= m_data.size(); i++) {
        if (m_data[m_data.size() - i] < y.m_data[m_data.size() - i]) return !m_positive;
        if (m_data[m_data.size() - i] > y.m_data[m_data.size() - i]) return m_positive;
    }
    return true;
}

bool BigInteger::operator==(BigInteger const& y) const {
    if (this->is_zero() and y.is_zero()) return true;
    return m_data == y.m_data;
}

bool BigInteger::operator!=(BigInteger const& y) const {
    if (this->is_zero() and y.is_zero()) return false;
    return m_data != y.m_data;
}

bool BigInteger::is_zero() const {
    return m_data.size() == 1 and m_data[0] == 0ul;
}

bool BigInteger::is_negative() const {
    return !is_zero() and !m_positive;
}

bool BigInteger::is_positive() const {
    return !is_zero() and m_positive;
}
bool BigInteger::is_even() const {
    if (m_data.empty()) return true;
    return m_data.front() % 2ul == 0ul;
}
bool BigInteger::is_odd() const {
    if (m_data.empty()) return false;
    return m_data.front() % 2ul == 1ul;
}

std::ostream &operator<<(std::ostream &os, BigInteger const &x)
{
    if (x.is_zero()) {
        os << "0";
        return os;
    }
    if (x.is_negative()) {
        os << "-";
    }
    else if (os.flags() & os.showpos) {
        os << "+";
    }
    std::stack<char> rtrn_v;
    BigInteger::IntegerData dividing = x.m_data;
    while (!BigInteger::bitset_is_zero(dividing)) {
        unsigned long digit = BigInteger::bitset_divide_10(dividing);
        rtrn_v.push('0' + (unsigned char)(digit));
    }
    while (!rtrn_v.empty()) {
        os << rtrn_v.top();
        rtrn_v.pop();
    }
    return os;
}

std::istream &operator>>(std::istream &is, BigInteger &x)
{
    std::string input;
    is >> input;
    if (!std::regex_match(input, BIG_INTEGER_REGEX)) {
        is.setstate(std::ios::failbit);
    }
    else {
        if (input[0] != '+' and input[0] != '-') {
            x.from_string(input);
            x.m_positive = true;
        }
        else {
            x.from_string(input.data() + 1);
            x.m_positive = input[0] == '+';
        }
    }
    return is;
}

BigInteger::IntegerData const& BigInteger::get_data() const {
    return m_data;
}

BigInteger BigInteger::pow(BigInteger const &x, std::size_t y)
{
    if (y == 0) {
        return "1";
    }
    if (y == 1) {
        return x;
    }
    if (y == 2) {
        return x * x;
    }
    if (y == 3) {
        return x * x * x;
    }
    BigInteger half_pow = pow(x, y / 2);
    if (y % 2 == 0) {
        return half_pow * half_pow;
    }
    else {
        return half_pow * half_pow * x;
    }
}

BigInteger BigInteger::div(BigInteger const& y) {
    bool x_was_negative = this->is_negative();
    IntegerData absolute_remainder = bitset_divide(m_data, y.m_data);
    if (!bitset_is_zero(absolute_remainder) and x_was_negative) {
        BigInteger result;
        result.m_data = y.m_data;
        bitset_subtract(result.m_data, absolute_remainder);
        bitset_increment(m_data);
        result.m_positive = true;
        return result;
    }
    else {
        BigInteger result;
        result.m_data = std::move(absolute_remainder);
        result.m_positive = true;
        return result;
    }
}

std::size_t BigInteger::get_decimal_digits() const {
    if (this->is_zero()) {
        return 0;
    }
    std::size_t decimal_digits = 0;
    IntegerData data = m_data;
    while (!bitset_is_zero(data)) {
        bitset_divide_10(data);
        decimal_digits++;
    }
    return decimal_digits;
}
}

namespace std {
umjc::BigInteger abs(umjc::BigInteger const& x) {
    return x.is_negative() ? -x : x;
}
}