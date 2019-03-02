#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <big_integer.h>
#include <cstddef>
#include <iosfwd>
#include <cstdint>
#include "optimized_vector/op_vector.h"
#include <functional>

struct big_integer
{
 public:
  big_integer();
  big_integer(big_integer const& other);
  big_integer(int a);
  explicit big_integer(std::string const& str);
  ~big_integer() = default;
 private:
  big_integer(op_vector number, bool sign);
 public:
  op_vector number;
  bool sign;
 public:
  big_integer& operator=(big_integer const& other);
  big_integer& operator+=(big_integer const& rhs);
  big_integer& operator-=(big_integer const& rhs);
  big_integer& operator*=(big_integer const& rhs);
  big_integer& operator/=(big_integer const& rhs);
  big_integer& operator%=(big_integer const& rhs);

  big_integer& operator&=(big_integer const& rhs);
  big_integer& operator|=(big_integer const& rhs);
  big_integer& operator^=(big_integer const& rhs);

  big_integer& operator<<=(int rhs);
  big_integer& operator>>=(int rhs);

  big_integer operator+() const;
  big_integer operator-() const;
  big_integer operator~() const;

  big_integer& operator++();
  big_integer operator++(int);

  big_integer& operator--();
  big_integer operator--(int);

  friend bool operator==(big_integer const& a, big_integer const& b);
  friend bool operator!=(big_integer const& a, big_integer const& b);
  friend bool operator<(big_integer const& a, big_integer const& b);
  friend bool operator>(big_integer const& a, big_integer const& b);
  friend bool operator<=(big_integer const& a, big_integer const& b);
  friend bool operator>=(big_integer const& a, big_integer const& b);
  friend big_integer operator/(big_integer a, big_integer const& b);

  friend std::string to_string(big_integer const& a);
  bool is_zero() const;
 private:
  big_integer& binary_logical_function(big_integer const& rhs, function<uint32_t(uint32_t, uint32_t)> &f);
  big_integer& mul_long_short(uint32_t a);
  big_integer& add_long_short(uint32_t a);
  uint32_t div_long_short(uint32_t a);
  void delete_zeroes();
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif // BIG_INTEGER_H
