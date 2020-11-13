#pragma once
#include "BigFloat.h"
#include <string>
#include <math.h>

class QFloat
{
private:
	int data[4] = { 0 };
public:
	QFloat();
	~QFloat();

	unsigned int MulAdd(const unsigned int&, const unsigned int);
	void ScanQFloat();
	void PrintQFloat();

	void BintoDec(string&);
	void DectoBin(string, string&);
	
	unsigned int getBit(int); // Lay bit thu i (0 <= i < 128)
	QFloat operator=(QFloat);
	bool operator==(QFloat);
};

