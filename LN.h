#ifndef LN_H
#define LN_H

#include "MyVector.h"
#include <string_view>

class LN
{
  private:
	MyVector chunks;
	bool sign;
	bool Nan;

	int compareByModule(const LN &ln) const;

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

	void print() const;
};

LN operator"" _ln(const char *word);

LN operator"" _ln(uint64_t value);

#endif