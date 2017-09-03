#pragma once
#include <stdint.h>
#include "Binary.h"

class UnmarshalException {};
extern class FString;

class BinaryStream
{
public:
	BinaryStream(int initCapacity);
	BinaryStream(char data[], int capacity, int head, int tail);
	BinaryStream(char data[], bool owner, int capacity, int head, int tail);
	BinaryStream();

	void operator =(const BinaryStream&) = delete;
	BinaryStream(const BinaryStream&) = delete;

	static BinaryStream& GetLocalClearTemp();

	void Wrap(char data[], int capacity, int head, int tail);
	void Wrap(BinaryStream bs, int head, int tail);

	char* Reserve(int size)
	{
		SureWrite(size);
		return _data + _tail;
	}
	void Clear();
	void Append(char data[], int size);
	void Append(char data[], int pos, int size);
	void AppendWithoutCopy(int size) { _tail += size; }
	void Skip(int size) { _head += size;  }

	char* GetDataHead() { return _data + _head; }
	int size() { return _tail - _head; }
	bool empty() { return _tail == _head;  }


	void WriteCompactInt(int32_t x);
	int32_t ReadCompactInt();

	void WriteCompactLong(int64_t x);
	int64_t ReadCompactLong();


	void WriteSize(int size);
	int ReadSize();


	void WriteBool(bool x);
	bool ReadBool();

	void WriteInt(int32_t x) { WriteCompactInt(x); }
	int32_t ReadInt() { return ReadCompactInt(); }

	void WriteLong(int64_t x) { WriteCompactLong(x); }
	int64_t ReadLong() { return ReadCompactLong(); }

	void WriteFloat(float x);
	float ReadFloat();

	void WriteDouble(double x);
	double ReadDouble();

	void WriteString(FString& x);
	void ReadString(FString& x);

private:
	int _head;
	int _tail;
	int _capacity;
	char* _data;
	bool _owner;


	int ChooseNewCapacity(int oldSize, int newSize);

	void SureRead(int size)
	{
		if (_tail - _head < size)
			throw UnmarshalException();
	}

	void SureWrite(int size)
	{
		if (_tail + size > _capacity)
		{
			SureWriteNotCheck(size);
		}
	}

	void SureWriteNotCheck(int size);
};
