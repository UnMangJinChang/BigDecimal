#ifndef UMJC_BIG_DECIMAL_HPP
#define UMJC_BIG_DECIMAL_HPP

#include "BigInteger.hpp"

namespace umjc {
class BigDecimal {
mutable BigInteger m_integer;
long long m_exponent;
static std::size_t m_significant_digits;

static void reduce_integer(BigInteger& x);

bool from_string(std::string const& x);
bool from_string(char const* x);
public:
BigDecimal();
BigDecimal(BigDecimal const& x);
BigDecimal(BigDecimal&& x);
BigDecimal(std::string const& x);
BigDecimal(char const* x);
BigDecimal(BigInteger const& x);
BigDecimal(BigInteger&& x);

BigDecimal& operator=(BigDecimal const& x);
BigDecimal& operator=(BigDecimal&& x);
BigDecimal& operator=(char const* c_string);
BigDecimal& operator=(std::string const& number_string);
BigDecimal& operator=(BigInteger const& x);
BigDecimal& operator=(BigInteger&& x);

BigDecimal& operator+=(BigDecimal const& x);
BigDecimal& operator-=(BigDecimal const& x);
BigDecimal& operator*=(BigDecimal const& x);
BigDecimal& operator/=(BigDecimal const& x);

BigDecimal operator+() const;
BigDecimal operator-() const;

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

static void set_significant_digits_capacity(std::size_t new_capacity);
static std::size_t get_significant_digits_capacity();

std::string to_string(std::size_t precision) const;
std::string to_scientific_string(std::size_t precision) const;
std::string to_fixed_string(std::size_t fraction_digits) const;

static BigDecimal abs(BigDecimal const& x);
static BigDecimal pow(BigDecimal const& x, std::size_t n);
static BigDecimal sqrt(BigDecimal const& x);
static BigDecimal cbrt(BigDecimal const& x);
static BigDecimal euler_number();
static BigDecimal exp(BigDecimal const& x);
static BigDecimal ln(BigDecimal const& x);
};
}

#endif