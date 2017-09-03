// Fill out your copyright notice in the Description page of Project Settings.

#include "Binary.h"
#include "BinaryStream.h"
#include <cstring>

char Binary::EMPTY_BYTES[1];

const char* CloneBytes(const char* data, int size)
{
	if (size == 0)
	{
		return Binary::EMPTY_BYTES;
	}
	else
	{
		char* newData = new char[size];
		std::memcpy(newData, data, size);
		return newData;
	}
}

Binary::Binary(const char* data, int size, bool owner) : _data(owner ? data : CloneBytes(data, size)), _size(size)
{
}

Binary::Binary() : _data(EMPTY_BYTES), _size(0) {}

Binary::~Binary()
{
	if (_size > 0)
	{
		delete[] _data;
	}
}

void Binary::operator = (const Binary& b)
{
	if (_size > 0)
	{
		delete[] _data;
	}
	_size = b._size;
	_data = CloneBytes(b._data, b._size);
}

void Binary::operator=(Binary && b)
{
	if (_size > 0)
	{
		delete[] _data;
	}
	_size = b._size;
	_data = b._data;

	b._size = 0;
	b._data = EMPTY_BYTES;
}

void Binary::Replace(const char * data, int size)
{
	if (_size > 0)
	{
		delete[] _data;
	}
	_size = size;
	_data = CloneBytes(data, size);
}

