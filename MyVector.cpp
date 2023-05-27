#include "MyVector.h"

#include <new>

MyVector::MyVector() = default;

MyVector::~MyVector()
{
	delete[] data;
}

MyVector &MyVector::operator=(const MyVector &vect)
{
	if (this == &vect)
	{
		return *this;
	}
	size = vect.size;
	capacity = vect.capacity;
	delete[] data;
	try
	{
		data = new uint32_t[capacity];
	} catch (const std::bad_alloc &e)
	{
		throw ERROR_OUT_OF_MEMORY;
	}
	for (size_t i = 0; i < size; ++i)
	{
		data[i] = vect.data[i];
	}
	return *this;
}

uint32_t &MyVector::operator[](size_t idx)
{
	if (idx >= size)
	{
		throw ERROR_DATA_INVALID;
	}
	return data[idx];
}

const uint32_t &MyVector::operator[](size_t idx) const
{
	if (idx >= size)
	{
		throw ERROR_DATA_INVALID;
	}
	return data[idx];
}

size_t MyVector::getSize() const
{
	return size;
}

void MyVector::resize()
{
	if (size == capacity)
	{
		capacity = capacity == 0 ? 1 : capacity * 2;
		uint32_t *newData;
		try
		{
			newData = new uint32_t[capacity];
		} catch (const std::bad_alloc &e)
		{
			throw ERROR_OUT_OF_MEMORY;
		}
		for (size_t i = 0; i < size; ++i)
		{
			newData[i] = data[i];
		}
		delete[] data;
		data = newData;
	}
}

void MyVector::popBack()
{
	if (size == 0)
	{
		throw ERROR_DATA_INVALID;
	}
	--size;
}

void MyVector::pushBack(uint32_t value)
{
	resize();
	data[size] = value;
	++size;
}

void MyVector::resize(size_t newSize)
{
	if (newSize > capacity)
	{
		capacity = newSize;
		uint32_t *newData;
		try
		{
			newData = new uint32_t[capacity];
		} catch (const std::bad_alloc &e)
		{
			throw ERROR_OUT_OF_MEMORY;
		}
		for (size_t i = 0; i < size; ++i)
		{
			newData[i] = data[i];
		}
		delete[] data;
		data = newData;
	}
	size = newSize;
}
