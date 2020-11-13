#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class BigFloat
{
public:
	std::vector<char> number;
	int exponent = 0; // So mu/Vi tri cua dau cham thap phan
	int char_size = 0; // Characteristic: phan nguyen
	int mant_size = 0; // Mantissa: phan thap phan

	BigFloat();
	~BigFloat();
	void calSize(); // Tinh toan chieu dai cua phan nguyen va phan thap phan
	void addZero(BigFloat&); // San bang chieu dai cua hai chuoi cho de tinh toan
	void deleteZero(); // Xoa nhung so 0 du thua

	void Nhap(string);
	void Xuat();

	BigFloat operator+(BigFloat);
	BigFloat operator*(BigFloat);
	void DividebyTwo(BigFloat&, int&); // Dung de lay bit cua phan nguyen cua so
	void SplDividebyTwo(BigFloat&); // Dung de tinh phan thap phan cua so

	BigFloat upPow(int, BigFloat); // n^i (i >= 0)
	BigFloat downPow(int); // 2^i (i < 0)

	int compareOne();
	bool operator==(BigFloat);
};
