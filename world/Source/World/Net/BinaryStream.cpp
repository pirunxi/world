#include "BinaryStream.h"
#include <algorithm>
#include <cstring>




BinaryStream::BinaryStream(int capacity) : BinaryStream(new char[capacity], true, capacity, 0, 0)
{
}

BinaryStream::BinaryStream(char data[], int capacity, int head, int tail) : BinaryStream(data, false, capacity, head, tail)
{
}

BinaryStream::BinaryStream(char data[], bool owner, int capacity, int head, int tail) : _data(data), _capacity(capacity), _head(head), _tail(tail), _owner(owner)
{
}

BinaryStream::BinaryStream() : BinaryStream(Binary::EMPTY_BYTES, 0, 0, 0)
{
	
}

BinaryStream& BinaryStream::GetLocalClearTemp()
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

union Uint32
{
	struct {
		char b0;
		char b1;
		char b2;
		char b3;
	};
	uint32_t i;
	float f;
	uint32_t i1 : 1;
	uint32_t i2 : 2;
	uint32_t i3 : 3;
	uint32_t i4 : 4;
	uint32_t i5 : 5;
	uint32_t i6 : 6;
	uint32_t i7 : 7;
	uint32_t i8 : 8;
};

void BinaryStream::WriteCompactInt(int32_t _x)
{
	uint32_t x = _x;
	if (x < 0x80)
	{
		SureWrite(1);
		_data[_tail++] = x;
		return;
	}

	Uint32 n;
	if (x < 0x4000)
	{
		SureWrite(2);
		n.i = x | 0x8000;
		_data[_tail + 1] = n.b0;
		_data[_tail    ] = n.b1;
		_tail += 2;
	}
	else if (x < 0x200000)
	{
		SureWrite(3);
		n.i = x | 0xc00000;
		_data[_tail + 2] = n.b0;
		_data[_tail + 1] = n.b1;
		_data[_tail    ] = n.b2;
		_tail += 3;
	}
	else if (x < 0x10000000)
	{
		SureWrite(4);
		n.i = x | 0xe0000000;
		_data[_tail + 3] = n.b0;
		_data[_tail + 2] = n.b1;
		_data[_tail + 1] = n.b2;
		_data[_tail] = n.b3;
		_tail += 4;
	}
	else
	{
		SureWrite(5);
		_data[_tail] = (unsigned char)0xf0;
		n.i = x;
		_data[_tail + 4] = n.b0;
		_data[_tail + 3] = n.b1;
		_data[_tail + 2] = n.b2;
		_data[_tail + 1] = n.b3;
		_tail += 5;
	}
}

int32_t BinaryStream::ReadCompactInt()
{
	SureRead(1);
	uint32_t h = (unsigned char)_data[_head];
	if (h < 0x80)
	{
		_head++;
		return h;
	}
	Uint32 n;
	n.i = 0;
	if (h < 0xc0)
	{
		SureRead(2);
		n.b0 = _data[_head + 1];
		n.b1 = _data[_head] & 0x3f;
		_head += 2;
	}
	else if (h < 0xe0)
	{
		SureRead(3);
		n.b0 = _data[_head + 2];
		n.b1 = _data[_head + 1];
		n.b2 = _data[_head] & 0x1f;
		_head += 3;
	}
	else if (h < 0xf0)
	{
		SureRead(4);
		n.b0 = _data[_head + 3];
		n.b1 = _data[_head + 2];
		n.b2 = _data[_head + 1];
		n.b3 = _data[_head] & 0x0f;
		_head += 4;
	}
	else
	{
		SureRead(5);
		n.b0 = _data[_head + 4];
		n.b1 = _data[_head + 3];
		n.b2 = _data[_head + 2];
		n.b3 = _data[_head + 1];
		_head += 5;
	}
	return n.i;
}

void BinaryStream::WriteCompactLong(int64_t _x)
{
	uint64_t x = _x;
	if (x < 0x10000000)
	{
		WriteCompactInt(x);
		return;
	}
	
	uint32_t h = x >> 32;
	Uint32 n;
	if (h < 0x8)
	{
		SureWrite(5);
		n.i = h | 0xf0;
		_data[_tail] = n.b0;
		_tail += 5;
	}
	else if (h < 0x400)
	{
		SureWrite(6);
		n.i = h | 0xf8;
		_data[_tail + 1] = n.b0;
		_data[_tail] = n.b1;
		_tail += 6;
	}
	else if (h < 0x20000)
	{
		SureWrite(7);
		n.i = h | 0xfc;
		_data[_tail + 2] = n.b0;
		_data[_tail + 1] = n.b1;
		_data[_tail] = n.b2;
		_tail += 7;
	}
	else if (h < 0x1000000)
	{
		SureWrite(8);
		n.i = h | 0xfe;
		_data[_tail + 3] = n.b0;
		_data[_tail + 2] = n.b1;
		_data[_tail + 1] = n.b2;
		_data[_tail] = n.b3;
		_tail += 8;
	}
	else
	{
		SureWrite(9);
		_data[_tail] = (unsigned char)0xff;
		n.i = h;
		_data[_tail + 3] = n.b0;
		_data[_tail + 2] = n.b1;
		_data[_tail + 1] = n.b2;
		_data[_tail] = n.b3;
		_tail += 9;
	}

	Uint32 m;
	m.i = x;
	_data[_tail - 1] = m.b0;
	_data[_tail - 2] = m.b1;
	_data[_tail - 3] = m.b2;
	_data[_tail - 4] = m.b3;
}

int64_t BinaryStream::ReadCompactLong()
{
	SureRead(1);
	uint32_t h = (unsigned char)_data[_head];
	if (h < 0xf0)
	{
		return ReadCompactInt();
	}
	Uint32 n;
	n.i = 0;
	if (h < 0xf8)
	{
		n.b0 = _data[_head] & 0x7;
		_head += 5;
	}
	else if (h < 0xfc)
	{
		n.b0 = _data[_head + 1];
		n.b1 = _data[_head] & 0x3;
		_head += 6;
	}
	else if (h < 0xfe)
	{
		n.b0 = _data[_head + 2];
		n.b1 = _data[_head + 1];
		n.b2 = _data[_head] & 0x1;
		_head += 7;
	}
	else if (h < 0xff)
	{
		n.b0 = _data[_head + 3];
		n.b1 = _data[_head + 2];
		n.b2 = _data[_head + 1];
		//n.b3 = _data[_head] & 0x0 = 0;
		_head += 8;
	}
	else
	{
		n.b0 = _data[_head + 4];
		n.b1 = _data[_head + 3];
		n.b2 = _data[_head + 2];
		n.b3 = _data[_head + 1];
		_head += 9;
	}


	Uint32 m;
	m.b0 = _data[_head - 1];
	m.b1 = _data[_head - 2];
	m.b2 = _data[_head - 3];
	m.b3 = _data[_head - 4];
	return (((uint64_t)n.i) << 32) | (m.i);
}

void BinaryStream::WriteSize(int size)
{
	if (size < 0 || size > 0x10000000) throw UnmarshalException();
	WriteCompactInt(size);
}

int BinaryStream::ReadSize()
{
	SureRead(1);
	uint32_t h = (unsigned char)_data[_head];
	if (h < 0x80)
	{
		_head++;
		return h;
	}
	Uint32 n;
	n.i = 0;
	if (h < 0xc0)
	{
		SureRead(2);
		n.b0 = _data[_head + 1];
		n.b1 = _data[_head] & 0x3f;
		_head += 2;
	}
	else if (h < 0xe0)
	{
		SureRead(3);
		n.b0 = _data[_head + 2];
		n.b1 = _data[_head + 1];
		n.b2 = _data[_head] & 0x1f;
		_head += 3;
	}
	else if (h < 0xf0)
	{
		SureRead(4);
		n.b0 = _data[_head + 3];
		n.b1 = _data[_head + 2];
		n.b2 = _data[_head + 1];
		n.b3 = _data[_head] & 0x0f;
		_head += 4;
	}
	else
	{
		throw UnmarshalException();
	}
	return n.i;
}

void BinaryStream::WriteBool(bool x)
{
	SureWrite(1);
	_data[_tail++] = x;
}

bool BinaryStream::ReadBool()
{
	SureRead(1);
	return _data[_head++];
}

void BinaryStream::WriteFloat(float x)
{
	SureWrite(4);
	*((float*)(_data + _tail)) = x;
	_tail += 4;
}

float BinaryStream::ReadFloat()
{
	SureRead(4);
	float x = *((float*)(_data + _head));
	_head += 4;
	return x;
}

void BinaryStream::WriteDouble(double x)
{
	SureWrite(8);
	*((double*)(_data + _tail)) = x;
	_tail += 8;
}

double BinaryStream::ReadDouble()
{
	SureRead(8);
	double x = *((double*)(_data + _head));
	_head += 8;
	return x;
}

void BinaryStream::WriteString(FString & x)
{
}

void BinaryStream::ReadString(FString & x)
{
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
