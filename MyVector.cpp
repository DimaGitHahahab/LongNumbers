#include "MyVector.h"

void MyVector::copyData(uint32_t *newData) const
{
	for (size_t i = 0; i < size_; ++i)
	{
		newData[i] = data_[i];
	}
}

void MyVector::resize()
{
	try
	{
		if (size_ == capacity_)
		{
			capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
			auto *newData = new uint32_t[capacity_];

			copyData(newData);

			delete[] data_;
			data_ = newData;
		}
	} catch (const std::bad_alloc &e)
	{
		throw e;
	}
}

MyVector::MyVector() : data_(nullptr), size_(0), capacity_(0) {}

MyVector::~MyVector()
{
	delete[] data_;
}

MyVector &MyVector::operator=(const MyVector &that)
{
	if (this != &that)
	{
		try
		{
			auto *newData = new uint32_t[that.capacity_];
			that.copyData(newData);

			delete[] data_;
			data_ = newData;
			size_ = that.size_;
			capacity_ = that.capacity_;
		} catch (const std::bad_alloc &e)
		{
			throw e;
		}
	}
	return *this;
}

uint32_t &MyVector::operator[](size_t idx)
{
	if (idx >= size_)
	{
		throw std::out_of_range("Index is out of range");
	}
	if (data_ == nullptr)
	{
		throw std::runtime_error("Vector is empty");
	}

	return data_[idx];
}

const uint32_t &MyVector::operator[](size_t idx) const
{
	if (idx >= size_)
	{
		throw std::out_of_range("Index is out of range");
	}

	if (data_ == nullptr)
	{
		throw std::runtime_error("Vector is empty");
	}

	return data_[idx];
}

size_t MyVector::getSize() const
{
	return size_;
}

void MyVector::popBack()
{
	if (size_ == 0)
	{
		throw std::underflow_error("Vector is empty");
	}
	--size_;
}

void MyVector::pushBack(uint32_t value)
{
	resize();
	data_[size_++] = value;
}

void MyVector::resize(size_t newSize)
{
	if (newSize > capacity_)
	{
		try
		{
			auto *newData = new uint32_t[newSize];

			copyData(newData);

			delete[] data_;
			data_ = newData;
			capacity_ = newSize;
		} catch (const std::bad_alloc &e)
		{
			throw e;
		}
	}
	size_ = newSize;
}
