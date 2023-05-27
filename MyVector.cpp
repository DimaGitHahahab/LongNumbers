#include "MyVector.h"

void MyVector::copyData(uint32_t *newData) const
{
	for (size_t i = 0; i < size; ++i)
	{
		newData[i] = data[i];
	}
}

void MyVector::resize()
{
	try
	{
		if (size == capacity)
		{
			capacity = (capacity == 0) ? 1 : capacity * 2;
			auto *newData = new uint32_t[capacity];

			copyData(newData);

			delete[] data;
			data = newData;
		}
	} catch (const std::bad_alloc &e)
	{
		throw e;
	}
}

MyVector::MyVector() : data(nullptr), size(0), capacity(0) {}

MyVector::~MyVector()
{
	delete[] data;
}

MyVector &MyVector::operator=(const MyVector &that)
{
	if (this != &that)
	{
		try
		{
			auto *newData = new uint32_t[that.capacity];
			that.copyData(newData);

			delete[] data;
			data = newData;
			size = that.size;
			capacity = that.capacity;
		} catch (const std::bad_alloc &e)
		{
			throw e;
		}
	}
	return *this;
}

uint32_t &MyVector::operator[](size_t idx)
{
	if (idx >= size)
	{
		throw std::out_of_range("Index is out of range");
	}
	if (data == nullptr)
	{
		throw std::runtime_error("Vector is empty");
	}

	return data[idx];
}

const uint32_t &MyVector::operator[](size_t idx) const
{
	if (idx >= size)
	{
		throw std::out_of_range("Index is out of range");
	}

	if (data == nullptr)
	{
		throw std::runtime_error("Vector is empty");
	}

	return data[idx];
}

size_t MyVector::getSize() const
{
	return size;
}

void MyVector::popBack()
{
	if (size == 0)
	{
		throw std::underflow_error("Vector is empty");
	}
	--size;
}

void MyVector::pushBack(uint32_t value)
{
	resize();
	data[size++] = value;
}

void MyVector::resize(size_t newSize)
{
	if (newSize > capacity)
	{
		try
		{
			auto *newData = new uint32_t[newSize];

			copyData(newData);

			delete[] data;
			data = newData;
			capacity = newSize;
		} catch (const std::bad_alloc &e)
		{
			throw e;
		}
	}
	size = newSize;
}
