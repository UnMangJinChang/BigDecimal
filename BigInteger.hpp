#ifndef UMJC_BIG_INTEGER_HPP
#define UMJC_BIG_INTEGER_HPP

#include <cstdint>
#include <cmath>
#include <limits>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <regex>
#include <ostream>
#include <istream>

class BigInteger {
public:
    using IntegerData = std::vector<unsigned long>;
private:
    bool m_positive;
    IntegerData m_data;

    void reset();
    void negate();

    static void erase_leading_zeroes(IntegerData& x);
    static bool bitset_is_zero(IntegerData const& x);
    static void bitset_increment(IntegerData& x);
    static void bitset_decrement(IntegerData& x);
    static void bitset_shift_left(IntegerData& x, std::size_t n);
    static void bitset_shift_right(IntegerData& x, std::size_t n);
    static void bitset_add(IntegerData& a, IntegerData const& b, std::size_t b_left_shift = 0);
    static void bitset_subtract(IntegerData& a, IntegerData const& b, std::size_t b_left_shift = 0);
    static void bitset_multiply(IntegerData& a, IntegerData const& b);
    static IntegerData bitset_divide(IntegerData& a, IntegerData const& b);
    static unsigned long bitset_divide_10(IntegerData& a);
    static IntegerData karatsuba(
        unsigned long const* a, std::size_t len_a, 
        unsigned long const* b, std::size_t len_b
    );
    static int bitset_compare(IntegerData const& a, IntegerData const& b, std::size_t b_left_shift = 0);
    
    void from_string(std::string const& number_string);
    void from_string(char const* c_number_string);
public:
    BigInteger();
    BigInteger(char const* c_string);
    BigInteger(std::string const& number_string);
    
    BigInteger& operator=(char const* c_string);
    BigInteger& operator=(std::string const& number_string);

    BigInteger& operator+=(BigInteger const& x);
    BigInteger& operator-=(BigInteger const& x);
    BigInteger& operator*=(BigInteger const& x);
    BigInteger& operator/=(BigInteger const& x);
    BigInteger& operator%=(BigInteger const& x);
    BigInteger& operator++();
    BigInteger& operator++(int);
    BigInteger& operator--();
    BigInteger& operator--(int);

    BigInteger operator+(BigInteger const& x) const;
    BigInteger operator-(BigInteger const& x) const;
    BigInteger operator*(BigInteger const& x) const;
    BigInteger operator/(BigInteger const& x) const;
    BigInteger operator%(BigInteger const& x) const;

    bool operator<(BigInteger const& x) const;
    bool operator<=(BigInteger const& x) const;
    bool operator>(BigInteger const& x) const;
    bool operator>=(BigInteger const& x) const;
    bool operator==(BigInteger const& x) const;
    bool operator!=(BigInteger const& x) const;

    friend std::ostream& operator<<(std::ostream& os, BigInteger const& x);
    friend std::istream& operator>>(std::istream& is, BigInteger& x);


    bool is_negative() const;
    bool is_zero() const;
    bool is_positive() const;

    IntegerData const& get_data() const;
    std::string to_string() const;
};

#endif