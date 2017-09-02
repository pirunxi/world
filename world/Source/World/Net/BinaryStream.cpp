#include "BinaryStream.h"
#include <algorithm>
#include <cstring>


char BinaryStream::EMPTY_BYTES[1];

BinaryStream::BinaryStream(int capacity) : BinaryStream(new char[capacity], true, capacity, 0, 0)
{
}

BinaryStream::BinaryStream(char data[], int capacity, int head, int tail) : BinaryStream(data, false, capacity, head, tail)
{
}

BinaryStream::BinaryStream(char data[], bool owner, int capacity, int head, int tail) : _data(data), _capacity(capacity), _head(head), _tail(tail), _owner(owner)
{
}

BinaryStream::BinaryStream() : BinaryStream(EMPTY_BYTES, 0, 0, 0)
{
	
}

BinaryStream BinaryStream::GetLocalClearTemp()
{
	thread_local BinaryStream LocalBS;
	LocalBS.Clear();
	return LocalBS;
}

void BinaryStream::Wrap(char data[], int capacity, int head, int tail)
{
	if (_owner)
	{
		delete[] _data;
	}
	_owner = false;
	_data = data;
	_capacity = capacity;
	_head = head;
	_tail = tail;
}

void BinaryStream::Wrap(BinaryStream bs, int head, int tail)
{
	if (_owner)
	{
		delete[] _data;
	}
	_owner = false;
	_data = bs._data;
	_capacity = bs._capacity;
	_head = head;
	_tail = tail;
}

void BinaryStream::Clear()
{
	_head = _tail = 0;
}

void BinaryStream::Append(char data[], int size)
{
	SureWrite(size);
	std::memcpy(_data + _tail, data, size);
	_tail += size;
}

void BinaryStream::Append(char data[], int pos, int size)
{
	SureWrite(size);
	std::memcpy(_data + _tail, data + pos, size);
	_tail += size;
}


void BinaryStream::SureWriteNotCheck(int size)
{
	int curSize = _tail - _head;
	int needSize = curSize + size;
	if (needSize <= _capacity)
	{
		std::memcpy(_data, _data + _head, curSize);
	}
	else
	{

		int newCapacity = ChooseNewCapacity(_capacity, needSize);
		char* newData = new char[newCapacity];
		if (curSize > 0)
		{
			std::memcpy(newData, _data + _head, curSize);
		}
		if (_owner)
		{
			delete[] _data;
		}
		else
		{
			_owner = true;
		}
		_data = newData;
		_capacity = newCapacity;
	}
	_head = 0;
	_tail = curSize;
}

int BinaryStream::ChooseNewCapacity(int originSize, int needSize)
{
	int newSize = std::max(originSize, 12);
	while (newSize < needSize)
	{
		newSize = newSize * 3 / 2;
	}
	return newSize;
}
