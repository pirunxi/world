// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class Binary
{
public:

	static char EMPTY_BYTES[];

	Binary(const char* data, int size, bool owner = false);
	Binary();

	void operator = (const Binary& b);
	void operator = (Binary&& b);

	Binary(const Binary&) = delete;

	virtual ~Binary();

	const char* data() { return _data;  }
	int size() { return _size;  }

	void Replace(const char* data, int size);

private:
	const char* _data;
	int _size;
};
