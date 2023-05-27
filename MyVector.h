#ifndef MYVECTOR_H
#define MYVECTOR_H

#include "return_codes.h"

#include <cstddef>
#include <cstdint>

class MyVector
{
  private:
	size_t size = 0;
	size_t capacity = 0;
	uint32_t *data = nullptr;

  public:
	MyVector();

	~MyVector();

	MyVector &operator=(const MyVector &vect);
	uint32_t &operator[](size_t idx);

	const uint32_t &operator[](size_t idx) const;

	size_t getSize() const;

	void resize();

	void resize(size_t newSize);

	void popBack();

	void pushBack(uint32_t value);
};

#endif