#ifndef MYVECTOR_H
#define MYVECTOR_H

#include "return_codes.h"

#include <cstdint>
#include <new>
#include <stdexcept>

class MyVector
{
  private:
	size_t size_ = 0;
	size_t capacity_ = 0;
	uint32_t *data_ = nullptr;
	void copyData(uint32_t *newData) const;
	void resize();

  public:
	MyVector();
	~MyVector();
	MyVector &operator=(const MyVector &that);

	uint32_t &operator[](size_t idx);
	const uint32_t &operator[](size_t idx) const;

	size_t getSize() const;

	void popBack();
	void pushBack(uint32_t value);

	void resize(size_t newSize);
};

#endif