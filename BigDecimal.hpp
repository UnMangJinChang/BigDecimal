#ifndef UMJC_BIG_DECIMAL_HPP
#define UMJC_BIG_DECIMAL_HPP

#include "BigInteger.hpp"

namespace umjc {
class BigDecimal {
mutable BigInteger m_integer;
std::int64_t m_exponent;
static std::size_t m_significant_digits;

void reduce_integer() const;
std::string to_string() const;
bool from_string(std::string const& x);
bool from_string(char const* x);
public:
BigDecimal();
BigDecimal(std::string const& x);
BigDecimal(char const* x);

BigDecimal& operator=(char const* c_string);
BigDecimal& operator=(std::string const& number_string);

BigDecimal& operator+=(BigDecimal const& x);
BigDecimal& operator-=(BigDecimal const& x);
BigDecimal& operator*=(BigDecimal const& x);
BigDecimal& operator/=(BigDecimal const& x);

BigDecimal operator+(BigDecimal const& x) const;
BigDecimal operator-(BigDecimal const& x) const;
BigDecimal operator*(BigDecimal const& x) const;
BigDecimal operator/(BigDecimal const& x) const;

bool operator<(BigDecimal const& x) const;
bool operator<=(BigDecimal const& x) const;
bool operator>(BigDecimal const& x) const;
bool operator>=(BigDecimal const& x) const;
bool operator==(BigDecimal const& x) const;
bool operator!=(BigDecimal const& x) const;

friend std::ostream& operator<<(std::ostream& os, BigDecimal const& x);
friend std::istream& operator>>(std::istream& is, BigDecimal& x);

bool is_negative() const;
bool is_zero() const;
bool is_positive() const;

};
}

#endif