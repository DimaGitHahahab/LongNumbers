#include "LN.h"

#include <climits>
#include <iostream>

LN::LN()
{
	sign = true;
	Nan = false;
	chunks.pushBack(0);
}

LN::LN(long long value)
{
	Nan = false;
	if (value == 0)
	{
		sign = true;
		chunks.pushBack(0);
		return;
	}
	sign = value > 0;
	uint64_t tmp;
	if (value == LLONG_MIN)
	{
		tmp = LLONG_MAX;
		++tmp;
	}
	else
	{
		tmp = value > 0 ? value : -value;
	}
	while (tmp > 0)
	{
		chunks.pushBack(tmp);
		tmp = tmp >> 32;
	}
}

LN::LN(const LN &ln)
{
	sign = ln.sign;
	Nan = ln.Nan;
	chunks = ln.chunks;
}

LN::LN(const char *word)
{
	size_t i = 0;
	if (word[0] != '-')
	{
		sign = true;
	}
	else
	{
		sign = false;
		i++;
	}
	chunks.pushBack(0);
	while (word[i] == '0')
	{
		i++;
	}
	while (word[i] != '\0')
	{
		multiplyByConst(16);
		if ((word[i] >= 'a') && (word[i] <= 'f'))
		{
			addConst(word[i] - 'a' + 10);
		}
		else if (word[i] >= 'A' && word[i] <= 'F')
		{
			addConst(word[i] - 'A' + 10);
		}
		else if (word[i] >= '0' && word[i] <= '9')
		{
			addConst(word[i] - '0');
		}
		else
		{
			Nan = true;
			return;
		}
		i++;
	}
	deleteExtraZeros();
}

LN::LN(std::string_view &word)
{
	size_t i = 0;
	if (word.front() != '-')
	{
		sign = true;
	}
	else
	{
		sign = false;
		i++;
	}
	chunks.pushBack(0);
	while (word[i] == '0')
	{
		i++;
	}
	while (i < word.size())
	{
		multiplyByConst(16);
		if ((word[i] >= 'a') && (word[i] <= 'f'))
		{
			addConst(word[i] - 'a' + 10);
		}
		else if (word[i] >= 'A' && word[i] <= 'F')
		{
			addConst(word[i] - 'A' + 10);
		}
		else if (word[i] >= '0' && word[i] <= '9')
		{
			addConst(word[i] - '0');
		}
		else
		{
			Nan = true;
			return;
		}
		i++;
	}
	deleteExtraZeros();
}

LN operator"" _ln(const char *word)
{
	return LN(word);
}

LN operator"" _ln(uint64_t value)
{
	return LN(value);
}

LN &LN::operator=(const LN &ln) = default;

LN &LN::operator+=(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		Nan = true;
		return *this;
	}
	if (sign == ln.sign)
	{
		add(ln);
	}
	else
	{
		int comparison = compareByModule(ln);
		if (comparison == 1)
		{
			subtract(ln);
		}
		else if (comparison == 0)
		{
			*this = 0;
		}
		else
		{
			LN tmp = ln;
			tmp.subtract(*this);
			*this = tmp;
		}
	}
	return *this;
}

LN LN::operator+(const LN &ln)
{
	LN result = *this;
	result += ln;
	return result;
}

LN &LN::operator-=(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		Nan = true;
		return *this;
	}
	if (sign != ln.sign)
	{
		add(ln);
	}
	else
	{
		if (compareByModule(ln) == 1)
		{
			subtract(ln);
		}
		else if (compareByModule(ln) == 0)
		{
			*this = 0;
		}
		else
		{
			LN tmp = ln;
			tmp.subtract(*this);
			*this = tmp;
			sign = !sign;
		}
	}
	return *this;
}

LN LN::operator-(const LN &ln)
{
	LN result = *this;
	result -= ln;
	return result;
}

LN LN::operator-()
{
	LN result = *this;
	result.sign = !result.sign;
	if (result.chunks.getSize() == 1 && result.chunks[0] == 0)
	{
		result.sign = true;
	}
	return result;
}

LN &LN::operator*=(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		Nan = true;
		return *this;
	}
	if (sign == ln.sign)
	{
		multiply(ln);
	}
	else
	{
		multiply(ln);
		sign = false;
	}
	if (chunks.getSize() == 1 && chunks[0] == 0)
	{
		sign = true;
	}
	return *this;
}

LN LN::operator*(const LN &ln)
{
	LN result = *this;
	result *= ln;
	return result;
}

LN &LN::operator/=(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		Nan = true;
		return *this;
	}
	if (sign == ln.sign)
	{
		divide(ln);
	}
	else
	{
		divide(ln);
		if (chunks.getSize() == 1 && chunks[0] == 0)
		{
			sign = true;
		}
		else
		{
			sign = false;
		}
	}
	return *this;
}

LN LN::operator/(const LN &ln)
{
	LN result = *this;
	result /= ln;
	return result;
}

LN &LN::operator%=(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		Nan = true;
		return *this;
	}
	if (sign == ln.sign)
	{
		mod(ln);
	}
	else
	{
		mod(ln);
		if (chunks.getSize() == 1 && chunks[0] == 0)
		{
			sign = true;
		}
		else
		{
			sign = false;
		}
	}
	return *this;
}

LN LN::operator%(const LN &ln)
{
	LN result = *this;
	result %= ln;
	return result;
}

bool LN::operator==(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		return false;
	}
	if (sign == ln.sign && compareByModule(ln) == 0)
	{
		return true;
	}
	return false;
}

bool LN::operator!=(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		return true;
	}
	return !(*this == ln);
}

bool LN::operator<(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		return false;
	}
	if (sign != ln.sign)
	{
		return !sign;
	}
	else
	{
		if (sign)
		{
			return compareByModule(ln) == -1;
		}
		else
		{
			return compareByModule(ln) == 1;
		}
	}
}

bool LN::operator>(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		return false;
	}
	if (sign != ln.sign)
	{
		return sign;
	}
	else
	{
		if (sign)
		{
			return compareByModule(ln) == 1;
		}
		else
		{
			return compareByModule(ln) == -1;
		}
	}
}

bool LN::operator<=(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		return false;
	}
	return *this < ln || *this == ln;
}

bool LN::operator>=(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		return false;
	}
	return *this > ln || *this == ln;
}

size_t max(size_t a, size_t b)
{
	return a > b ? a : b;
}

LN::operator long long() const
{
	if (Nan)
	{
		throw std::out_of_range("Cannot convert NaN to long long");
	}
	if (chunks.getSize() > 2)
	{
		throw std::out_of_range("Value too large to fit into a long long");
	}
	long long result = 0;
	for (size_t i = 0; i < chunks.getSize(); i++)
	{
		result += chunks[i] * pow(2, 32 * i);
	}
	if (!sign)
	{
		result *= -1;
	}
	return result;
}

LN::operator bool() const
{
	if (Nan)
	{
		return false;
	}
	if (chunks.getSize() > 2)
	{
		return ERROR_DATA_INVALID;
	}
	if (chunks.getSize() == 1 && chunks[0] == 0)
	{
		return false;
	}
	return true;
}

int LN::compareByModule(const LN &ln) const
{
	if (chunks.getSize() > ln.chunks.getSize())
	{
		return 1;
	}
	else if (chunks.getSize() < ln.chunks.getSize())
	{
		return -1;
	}
	else
	{
		for (size_t i = chunks.getSize(); i > 0; i--)
		{
			if (chunks[i - 1] > ln.chunks[i - 1])
			{
				return 1;
			}
			else if (chunks[i - 1] < ln.chunks[i - 1])
			{
				return -1;
			}
		}
		return 0;
	}
}

LN &LN::add(const LN &ln)
{
	uint64_t carry = 0;
	size_t maxSize = max(chunks.getSize(), ln.chunks.getSize());
	for (size_t i = 0; i < maxSize; i++)
	{
		uint64_t first = (i < chunks.getSize() ? chunks[i] : 0);
		uint64_t second = (i < ln.chunks.getSize() ? ln.chunks[i] : 0);

		uint64_t res = first + second + carry;
		carry = res >> 32;
		if (i < chunks.getSize())
		{
			chunks[i] = res;
		}
		else
		{
			chunks.pushBack(res);
		}
	}
	if (carry != 0)
	{
		chunks.pushBack(carry);
	}
	if (chunks.getSize() == 1 && chunks[0] == 0)
	{
		sign = true;
	}
	return *this;
}

LN &LN::subtract(const LN &ln)
{
	uint64_t carry = 0;
	size_t maxSize = max(chunks.getSize(), ln.chunks.getSize());
	for (size_t i = 0; i < maxSize; i++)
	{
		uint64_t first = (i < chunks.getSize() ? chunks[i] : 0);
		uint64_t second = (i < ln.chunks.getSize() ? ln.chunks[i] : 0);

		uint64_t res = static_cast< uint64_t >(std::numeric_limits< uint32_t >::max()) + 1 + first;
		res -= carry + second;

		chunks[i] = res;
		carry = res >> 32 ^ 1;
	}
	deleteExtraZeros();
	if (chunks.getSize() == 1 && chunks[0] == 0)
	{
		sign = true;
	}
	return *this;
}

LN &LN::multiply(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		Nan = true;
		return *this;
	}
	LN result;
	result.chunks.resize(chunks.getSize() + ln.chunks.getSize());
	for (size_t i = 0; i < chunks.getSize(); i++)
	{
		uint64_t carry = 0;
		for (size_t j = 0; j < ln.chunks.getSize(); j++)
		{
			uint64_t first = chunks[i];
			uint64_t second = ln.chunks[j];
			uint64_t res = first * second + result.chunks[i + j] + carry;
			result.chunks[i + j] = res;
			carry = res >> 32;
		}
		result.chunks[i + ln.chunks.getSize()] = carry;
	}
	result.sign = sign == ln.sign;
	result.deleteExtraZeros();
	*this = result;
	if (chunks.getSize() == 1 && chunks[0] == 0)
	{
		sign = true;
	}
	return *this;
}

LN &LN::divide(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		Nan = true;
		return *this;
	}
	if (ln.chunks.getSize() == 1 && ln.chunks[0] == 0)
	{
		Nan = true;
		return *this;
	}
	if (compareByModule(ln) == -1)
	{
		*this = 0;
		sign = true;
		return *this;
	}
	if (compareByModule(ln) == 0)
	{
		*this = 1;
		sign = sign == ln.sign;
		return *this;
	}
	LN result;
	result.chunks.resize(chunks.getSize() - ln.chunks.getSize() + 1);
	LN tmp = *this;
	tmp.sign = true;
	LN tmp2 = ln;
	tmp2.sign = true;
	for (size_t i = chunks.getSize() - ln.chunks.getSize() + 1; i > 0; i--)
	{
		LN tmp3 = tmp2;
		while (tmp3 <= tmp)
		{
			tmp -= tmp3;
			result.chunks[i - 1]++;
		}
	}
	result.sign = sign == ln.sign;
	result.deleteExtraZeros();
	*this = result;
	if (chunks.getSize() == 1 && chunks[0] == 0)
	{
		sign = true;
	}
	return *this;
}

LN &LN::mod(const LN &ln)
{
	if (Nan || ln.Nan)
	{
		Nan = true;
		return *this;
	}
	if (ln.chunks.getSize() == 1 && ln.chunks[0] == 0)
	{
		Nan = true;
		return *this;
	}
	if (compareByModule(ln) == -1)
	{
		return *this;
	}
	if (compareByModule(ln) == 0)
	{
		*this = 0;
		return *this;
	}
	LN tmp = *this;
	tmp.sign = true;
	LN tmp2 = ln;
	tmp2.sign = true;
	for (size_t i = chunks.getSize() - ln.chunks.getSize() + 1; i > 0; i--)
	{
		LN tmp3 = tmp2;
		while (tmp3 <= tmp)
		{
			tmp -= tmp3;
		}
	}
	*this = tmp;
	if (chunks.getSize() == 1 && chunks[0] == 0)
	{
		sign = true;
	}
	return *this;
}

LN &LN::sqrt()
{
	if (Nan)
	{
		return *this;
	}
	if (!sign)
	{
		Nan = true;
		return *this;
	}
	LN result;
	result.chunks.resize(chunks.getSize() / 2 + 1);
	LN tmp = *this;
	tmp.sign = true;
	for (size_t i = chunks.getSize() / 2 + 1; i > 0; i--)
	{
		LN tmp2 = result;
		tmp2.chunks[i - 1] = 1;
		while (tmp2 * tmp2 <= tmp)
		{
			tmp2 += 1;
		}
		tmp2 -= 1;
		result.chunks[i - 1] = tmp2.chunks[0];
		tmp -= tmp2 * tmp2;
		tmp2.chunks[0] = 0;
	}
	result.deleteExtraZeros();
	*this = result;
	if (chunks.getSize() == 1 && chunks[0] == 0)
	{
		sign = true;
	}
	return *this;
}

void LN::deleteExtraZeros()
{
	while (chunks.getSize() > 1 && chunks[chunks.getSize() - 1] == 0)
	{
		chunks.popBack();
	}
}

LN &LN::addConst(uint64_t value)
{
	if (Nan)
	{
		return *this;
	}
	uint64_t tmp = chunks[0] + value;
	uint64_t carry = tmp >> 32;
	chunks[0] = tmp;
	for (size_t i = 1; i < chunks.getSize(); i++)
	{
		tmp = chunks[i] + value + carry;
		chunks[i] = tmp;
		carry = tmp >> 32;
	}
	if (carry != 0)
	{
		chunks.pushBack(carry);
	}
	return *this;
}

LN &LN::multiplyByConst(uint64_t value)
{
	if (Nan)
	{
		return *this;
	}
	uint64_t tmp = chunks[0] * value;
	uint64_t carry = tmp >> 32;
	chunks[0] = tmp;
	for (size_t i = 1; i < chunks.getSize(); i++)
	{
		tmp = chunks[i] * value + carry;
		chunks[i] = tmp;
		carry = tmp >> 32;
	}
	if (carry != 0)
	{
		chunks.pushBack(carry);
	}
	return *this;
}
void LN::print() const
{
	if (Nan)
	{
		std::cout << "NaN";
		return;
	}
	if (!sign)
	{
		std::cout << "-";
	}
	std::cout << chunks[chunks.getSize() - 1];
	for (size_t i = chunks.getSize() - 1; i > 0; i--)
	{
		std::cout << chunks[i - 1] << std::endl;
	}
}
LN LN::operator~()
{
	sqrt();
	return *this;
}
