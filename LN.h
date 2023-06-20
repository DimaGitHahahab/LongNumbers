#ifndef LN_H
#define LN_H

#include "MyVector.h"
#include <string_view>

#include <cstdint>
#include <iomanip>
#include <iostream>

class LN
{
  private:
	MyVector chunks_;
	bool sign_;
	bool Nan_;

	int compareByModule(const LN &that) const;

	void deleteExtraZeros();

	LN &add(const LN &ln);

	LN &subtract(const LN &ln);

	LN &multiply(const LN &ln);

	LN &divide(const LN &ln);

	LN &mod(const LN &ln);

	LN &sqrt();

	LN &addConst(uint64_t value);

	LN &multiplyByConst(uint64_t value);

  public:
	LN();

	LN(long long value);

	LN(const LN &ln);

	explicit LN(const char *word);

	explicit LN(std::string_view &word);

	operator long long() const;

	operator bool() const;

	LN &operator+=(const LN &ln);

	LN operator+(const LN &ln);

	LN &operator=(const LN &ln);

	LN &operator-=(const LN &ln);

	LN operator-(const LN &ln);

	LN operator-();

	LN &operator*=(const LN &ln);

	LN operator*(const LN &ln);

	LN &operator/=(const LN &ln);

	LN operator/(const LN &ln);

	LN &operator%=(const LN &ln);

	LN operator%(const LN &ln);

	LN operator~();

	bool operator==(const LN &ln);

	bool operator!=(const LN &ln);

	bool operator<(const LN &ln);

	bool operator>(const LN &ln);

	bool operator<=(const LN &ln);

	bool operator>=(const LN &ln);

	void hexaDecimalPrint(std::ostream &outputStream);
};

LN operator"" _ln(const char *word);

#endif	  // LN_H