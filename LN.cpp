#include "LN.h"

#include <cmath>

LN::LN()
{
	sign_ = true;
	Nan_ = false;
	chunks_.pushBack(0);
}

LN::LN(long long value)
{
	Nan_ = false;
	if (value == 0)
	{
		sign_ = true;
		chunks_.pushBack(0);
		return;
	}
	sign_ = value > 0;
	uint64_t tmp;

	tmp = value > 0 ? value : -value;

	while (tmp > 0)
	{
		chunks_.pushBack(tmp);
		tmp = tmp >> 32;
	}
}

LN::LN(const LN &ln)
{
	sign_ = ln.sign_;
	Nan_ = ln.Nan_;
	chunks_ = ln.chunks_;
}

LN::LN(const char *word)
{
	Nan_ = false;
	size_t i = 0;
	if (word[0] != '-')
	{
		sign_ = true;
	}
	else
	{
		sign_ = false;
		i++;
	}
	chunks_.pushBack(0);
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
			Nan_ = true;
			return;
		}
		i++;
	}
	deleteExtraZeros();
}

LN::LN(std::string_view &word)
{
	Nan_ = false;
	size_t i = 0;
	if (word.front() != '-')
	{
		sign_ = true;
	}
	else
	{
		sign_ = false;
		i++;
	}
	chunks_.pushBack(0);
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
			Nan_ = true;
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

LN &LN::operator=(const LN &ln) = default;

LN &LN::operator+=(const LN &ln)
{
	if (Nan_ || ln.Nan_)
	{
		Nan_ = true;
		return *this;
	}
	if (sign_ == ln.sign_)
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
	if (Nan_ || ln.Nan_)
	{
		Nan_ = true;
		return *this;
	}
	if (sign_ != ln.sign_)
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
			sign_ = !sign_;
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
	result.sign_ = !result.sign_;
	if (result.chunks_.getSize() == 1 && result.chunks_[0] == 0)
	{
		result.sign_ = true;
	}
	return result;
}

LN &LN::operator*=(const LN &ln)
{
	if (Nan_ || ln.Nan_)
	{
		Nan_ = true;
		return *this;
	}
	if (sign_ == ln.sign_)
	{
		multiply(ln);
	}
	else
	{
		multiply(ln);
		sign_ = false;
	}
	if (chunks_.getSize() == 1 && chunks_[0] == 0)
	{
		sign_ = true;
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
	if (Nan_ || ln.Nan_)
	{
		Nan_ = true;
		return *this;
	}
	if (sign_ == ln.sign_)
	{
		divide(ln);
	}
	else
	{
		divide(ln);
		if (chunks_.getSize() == 1 && chunks_[0] == 0)
		{
			sign_ = true;
		}
		else
		{
			sign_ = false;
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
	if (Nan_ || ln.Nan_)
	{
		Nan_ = true;
		return *this;
	}
	if (sign_ == ln.sign_)
	{
		mod(ln);
	}
	else
	{
		mod(ln);
		if (chunks_.getSize() == 1 && chunks_[0] == 0)
		{
			sign_ = true;
		}
		else
		{
			sign_ = false;
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
	if (Nan_ || ln.Nan_)
	{
		return false;
	}
	if (sign_ == ln.sign_ && compareByModule(ln) == 0)
	{
		return true;
	}
	return false;
}

bool LN::operator!=(const LN &ln)
{
	if (Nan_ || ln.Nan_)
	{
		return true;
	}
	return !(*this == ln);
}

bool LN::operator<(const LN &ln)
{
	if (Nan_ || ln.Nan_)
	{
		return false;
	}
	if (sign_ != ln.sign_)
	{
		return !sign_;
	}
	else
	{
		if (sign_)
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
	if (Nan_ || ln.Nan_)
	{
		return false;
	}
	if (sign_ != ln.sign_)
	{
		return sign_;
	}
	else
	{
		if (sign_)
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
	if (Nan_ || ln.Nan_)
	{
		return false;
	}
	return *this < ln || *this == ln;
}

bool LN::operator>=(const LN &ln)
{
	if (Nan_ || ln.Nan_)
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
	if (Nan_)
	{
		throw std::out_of_range("Cannot convert NaN to long long");
	}
	if (chunks_.getSize() > 2)
	{
		throw std::out_of_range("Value too large to fit into a long long");
	}
	long long result = 0;
	for (size_t i = 0; i < chunks_.getSize(); i++)
	{
		result += chunks_[i] * pow(2, 32 * i);
	}
	if (!sign_)
	{
		result *= -1;
	}
	return result;
}

LN::operator bool() const
{
	if (Nan_)
	{
		return false;
	}
	if (chunks_.getSize() > 2)
	{
		return ERROR_DATA_INVALID;
	}
	if (chunks_.getSize() == 1 && chunks_[0] == 0)
	{
		return false;
	}
	return true;
}

int LN::compareByModule(const LN &that) const
{
	if (chunks_.getSize() > that.chunks_.getSize())
	{
		return 1;
	}
	else if (chunks_.getSize() < that.chunks_.getSize())
	{
		return -1;
	}
	else
	{
		for (size_t i = chunks_.getSize(); i > 0; i--)
		{
			if (chunks_[i - 1] > that.chunks_[i - 1])
			{
				return 1;
			}
			else if (chunks_[i - 1] < that.chunks_[i - 1])
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
	size_t maxSize = max(chunks_.getSize(), ln.chunks_.getSize());
	for (size_t i = 0; i < maxSize; i++)
	{
		uint64_t first = (i < chunks_.getSize() ? chunks_[i] : 0);
		uint64_t second = (i < ln.chunks_.getSize() ? ln.chunks_[i] : 0);

		uint64_t res = first + second + carry;
		carry = res >> 32;
		if (i < chunks_.getSize())
		{
			chunks_[i] = res;
		}
		else
		{
			chunks_.pushBack(res);
		}
	}
	if (carry != 0)
	{
		chunks_.pushBack(carry);
	}
	if (chunks_.getSize() == 1 && chunks_[0] == 0)
	{
		sign_ = true;
	}
	return *this;
}

LN &LN::subtract(const LN &ln)
{
	uint64_t carry = 0;
	size_t maxSize = max(chunks_.getSize(), ln.chunks_.getSize());
	for (size_t i = 0; i < maxSize; i++)
	{
		uint64_t first = (i < chunks_.getSize() ? chunks_[i] : 0);
		uint64_t second = (i < ln.chunks_.getSize() ? ln.chunks_[i] : 0);

		uint64_t res = static_cast< uint64_t >(std::numeric_limits< uint32_t >::max()) + 1 + first;
		// uint64_t res = pow(2, 32) + first + 1;
		res -= carry + second;

		chunks_[i] = res;
		carry = res >> 32 ^ 1;
	}
	deleteExtraZeros();
	if (chunks_.getSize() == 1 && chunks_[0] == 0)
	{
		sign_ = true;
	}
	return *this;
}

LN &LN::multiply(const LN &ln)
{
	if (Nan_ || ln.Nan_)
	{
		Nan_ = true;
		return *this;
	}
	LN result;
	result.chunks_.resize(chunks_.getSize() + ln.chunks_.getSize());
	for (size_t i = 0; i < chunks_.getSize(); i++)
	{
		uint64_t carry = 0;
		for (size_t j = 0; j < ln.chunks_.getSize(); j++)
		{
			uint64_t first = chunks_[i];
			uint64_t second = ln.chunks_[j];
			uint64_t res = first * second + result.chunks_[i + j] + carry;
			result.chunks_[i + j] = res;
			carry = res >> 32;
		}
		result.chunks_[i + ln.chunks_.getSize()] = carry;
	}
	result.sign_ = sign_ == ln.sign_;
	result.deleteExtraZeros();
	*this = result;
	if (chunks_.getSize() == 1 && chunks_[0] == 0)
	{
		sign_ = true;
	}
	return *this;
}

LN &LN::divide(const LN &ln)
{
	if (Nan_ || ln.Nan_)
	{
		Nan_ = true;
		return *this;
	}
	if (ln.chunks_.getSize() == 1 && ln.chunks_[0] == 0)
	{
		Nan_ = true;
		return *this;
	}
	if (compareByModule(ln) == -1)
	{
		*this = 0;
		sign_ = true;
		return *this;
	}
	if (compareByModule(ln) == 0)
	{
		*this = 1;
		sign_ = sign_ == ln.sign_;
		return *this;
	}
	LN result;
	result.chunks_.resize(chunks_.getSize() - ln.chunks_.getSize() + 1);
	LN tmp = *this;
	tmp.sign_ = true;
	LN tmp2 = ln;
	tmp2.sign_ = true;
	for (size_t i = chunks_.getSize() - ln.chunks_.getSize() + 1; i > 0; i--)
	{
		LN tmp3 = tmp2;
		while (tmp3 <= tmp)
		{
			tmp -= tmp3;
			result.chunks_[i - 1]++;
		}
	}
	result.sign_ = sign_ == ln.sign_;
	result.deleteExtraZeros();
	*this = result;
	if (chunks_.getSize() == 1 && chunks_[0] == 0)
	{
		sign_ = true;
	}
	return *this;
}

LN &LN::mod(const LN &ln)
{
	if (Nan_ || ln.Nan_)
	{
		Nan_ = true;
		return *this;
	}
	if (ln.chunks_.getSize() == 1 && ln.chunks_[0] == 0)
	{
		Nan_ = true;
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
	tmp.sign_ = true;
	LN tmp2 = ln;
	tmp2.sign_ = true;
	for (size_t i = chunks_.getSize() - ln.chunks_.getSize() + 1; i > 0; i--)
	{
		LN tmp3 = tmp2;
		while (tmp3 <= tmp)
		{
			tmp -= tmp3;
		}
	}
	*this = tmp;
	if (chunks_.getSize() == 1 && chunks_[0] == 0)
	{
		sign_ = true;
	}
	return *this;
}

LN &LN::sqrt()
{
	if (Nan_)
	{
		return *this;
	}
	if (!sign_)
	{
		Nan_ = true;
		return *this;
	}
	LN result;
	result.chunks_.resize(chunks_.getSize() / 2 + 1);
	LN tmp = *this;
	tmp.sign_ = true;
	for (size_t i = chunks_.getSize() / 2 + 1; i > 0; i--)
	{
		LN tmp2 = result;
		tmp2.chunks_[i - 1] = 1;
		while (tmp2 * tmp2 <= tmp)
		{
			tmp2 += 1;
		}
		tmp2 -= 1;
		result.chunks_[i - 1] = tmp2.chunks_[0];
		tmp -= tmp2 * tmp2;
		tmp2.chunks_[0] = 0;
	}
	result.deleteExtraZeros();
	*this = result;
	if (chunks_.getSize() == 1 && chunks_[0] == 0)
	{
		sign_ = true;
	}
	return *this;
}

void LN::deleteExtraZeros()
{
	while (chunks_.getSize() > 1 && chunks_[chunks_.getSize() - 1] == 0)
	{
		chunks_.popBack();
	}
}

LN &LN::addConst(uint64_t value)
{
	if (Nan_)
	{
		return *this;
	}
	uint64_t tmp = chunks_[0] + value;
	uint64_t carry = tmp >> 32;
	chunks_[0] = tmp;
	for (size_t i = 1; i < chunks_.getSize(); i++)
	{
		tmp = chunks_[i] + value + carry;
		chunks_[i] = tmp;
		carry = tmp >> 32;
	}
	if (carry != 0)
	{
		chunks_.pushBack(carry);
	}
	return *this;
}

LN &LN::multiplyByConst(uint64_t value)
{
	if (Nan_)
	{
		return *this;
	}
	uint64_t tmp = chunks_[0] * value;
	uint64_t carry = tmp >> 32;
	chunks_[0] = tmp;
	for (size_t i = 1; i < chunks_.getSize(); i++)
	{
		tmp = chunks_[i] * value + carry;
		chunks_[i] = tmp;
		carry = tmp >> 32;
	}
	if (carry != 0)
	{
		chunks_.pushBack(carry);
	}
	return *this;
}

LN LN::operator~()
{
	sqrt();
	return *this;
}
void LN::hexaDecimalPrint(std::ostream &outputStream)
{
	if (Nan_)
	{
		outputStream << "NaN";
		return;
	}
	if (!sign_)
	{
		outputStream << "-";
	}
	outputStream << std::hex << std::uppercase << chunks_[chunks_.getSize() - 1];
	for (size_t i = 1; i < chunks_.getSize(); i++)
	{
		outputStream << std::setfill('0') << std::setw(8) << chunks_[chunks_.getSize() - i - 1];
	}
	outputStream << std::endl;
	return;
}
