#pragma once

class UnmarshalException {};

class BinaryStream
{
public:
	BinaryStream(int initCapacity);
	BinaryStream(char data[], int capacity, int head, int tail);
	BinaryStream(char data[], bool owner, int capacity, int head, int tail);
	BinaryStream();

	static BinaryStream GetLocalClearTemp();

	void Wrap(char data[], int capacity, int head, int tail);
	void Wrap(BinaryStream bs, int head, int tail);


	void Clear();
	void Append(char data[], int size);
	void Append(char data[], int pos, int size);

private:
	int _head;
	int _tail;
	int _capacity;
	char* _data;
	bool _owner;


	static char EMPTY_BYTES[];


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
