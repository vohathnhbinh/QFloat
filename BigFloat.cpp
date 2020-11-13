#include "BigFloat.h"



BigFloat::BigFloat()
{
}


BigFloat::~BigFloat()
{
}

void BigFloat::calSize()
{
	mant_size = number.size() - exponent;
	char_size = number.size() - mant_size;
}

void BigFloat::addZero(BigFloat &a)
{
	int mant_dstce = this->mant_size - a.mant_size; // Khoang cach so thap phan giua hai so
	if (this->mant_size < a.mant_size)
	{
		mant_dstce *= -1;
		while (mant_dstce)
		{
			this->number.push_back('0');
			mant_dstce--;
		}
	}
	else if (this->mant_size > a.mant_size)
	{
		while (mant_dstce)
		{
			a.number.push_back('0');
			mant_dstce--;
		}
	}

	int char_dstce = this->char_size - a.char_size; // Khoang cach so nguyen giua hai so
	if (this->char_size < a.char_size)
	{
		char_dstce *= -1;
		while (char_dstce)
		{
			this->number.insert(this->number.begin(), '0');
			char_dstce--;
		}
	}
	else if (this->char_size > a.char_size)
	{
		while (char_dstce)
		{
			a.number.insert(a.number.begin(), '0');
			char_dstce--;
		}
	}
}

void BigFloat::deleteZero()
{
	while (number.empty() == 0 && number[0] == '0')
	{
		number.erase(number.begin());
		exponent--;
	}
	while (number.empty() == 0 && number[number.size() - 1] == '0' && number.size() > exponent)
	{

	}
}

void BigFloat::Nhap(string str)
{
	copy(str.begin(), str.end(), back_inserter(number));

	for (int i = 0; i < number.size(); i++)
	{
		if (number[i] == '.')
		{
			exponent = i;
			number.erase(number.begin() + i);
			break;
		}
		else
			exponent++;
	}
}
void BigFloat::Xuat()
{
	if (exponent == 0)
		cout << '0';
	for (int i = 0; i < exponent; i++)
		cout << number[i];
	cout << '.';
	if (exponent == number.size())
		cout << '0';
	else
	{
		for (int i = exponent; i < number.size(); i++)
			cout << number[i];
	}
	cout << '\n';
}

BigFloat BigFloat::operator+(BigFloat a)
{
	this->calSize(); a.calSize();
	this->addZero(a);
	BigFloat result;
	this->exponent > a.exponent ? result.exponent = this->exponent : result.exponent = a.exponent;
	result.number.push_back('0');
	int temp = 0;
	for (int i = this->number.size() - 1; i >= 0; i--)
	{
		temp += (this->number[i] - '0') + (a.number[i] - '0');
		result.number.insert(result.number.begin(), (temp % 10) + '0');
		temp /= 10;
	}
	result.number.pop_back();
	// Con lai phan nho van chua cong het
	while (temp)
	{
		result.number.insert(result.number.begin(), (temp % 10) + '0');
		temp /= 10;
		result.exponent++;
	}

	return result;
}

BigFloat BigFloat::operator*(BigFloat a)
{
	this->calSize(); a.calSize();
	BigFloat result;
	int temp = 0;
	for (int i = a.number.size() - 1; i >= 0; i--)
	{
		BigFloat anthr_temp;
		int k = i;
		anthr_temp.number.push_back('0');
		for (int j = this->number.size() - 1; j >= 0; j--)
		{
			temp += (a.number[i] - '0') * (this->number[j] - '0');
			anthr_temp.number.insert(anthr_temp.number.begin(), (temp % 10) + '0');
			temp /= 10;
			anthr_temp.exponent++;
		}
		anthr_temp.number.pop_back();
		// Con lai phan nho van chua cong het
		while (temp)
		{
			anthr_temp.number.insert(anthr_temp.number.begin(), (temp % 10) + '0');
			temp /= 10;
			anthr_temp.exponent++;
		}

		while (k != a.number.size() - 1)
		{
 			anthr_temp.number.push_back('0');
			anthr_temp.exponent++;
			k++;
		}
		result = result + anthr_temp;
		temp = 0;
	}
	result.mant_size = this->mant_size + a.mant_size;
	result.exponent = result.number.size() - result.mant_size;
	result.deleteZero();
	return result;
}

void BigFloat::DividebyTwo(BigFloat& result, int& r)
{
	int temp = 0;
	for (int i = 0; i < number.size(); i++)
	{
		temp = temp * 10 + number[i] - '0';
		if (temp < 2 && i + 1 < number.size())
		{
			i++;
			temp = temp * 10 + number[i] - '0';
		}
		result.number.push_back(temp / 2 + '0');
		temp %= 2;
	}
	if (number[number.size() - 1] % 2 == 0)
		r = 0;
	else
		r = 1;
}

void BigFloat::SplDividebyTwo(BigFloat& result)
{
	int temp = 0;
	for (int i = 0; i < number.size(); i++)
	{
		temp = temp * 10 + number[i] - '0';
		result.number.push_back(temp / 2 + '0');
		temp %= 2;

		if (i == number.size() - 1)
		{
			temp = temp * 10;
			result.number.push_back(temp / 2 + '0');
		}
	}
}

// Xai xong phai tang exponent
BigFloat BigFloat::upPow(int i, BigFloat n)
{
	BigFloat one; one.number.push_back('1');
	if (i == 0)
	{
		return one;
	}
	return n * upPow(--i, n);
}

BigFloat BigFloat::downPow(int i)
{
	BigFloat temp; temp.number.push_back('1');
	BigFloat anthr_temp;
	for (; i < 0; i++)
	{
		temp.SplDividebyTwo(anthr_temp); // anthr_temp = temp / 2;
		temp = anthr_temp;
		anthr_temp.number.clear();
		anthr_temp.exponent = 0;
	}
	temp.exponent++;
	return temp;
}

int BigFloat::compareOne()
{
	if (number[0] == '0')
		return 0;
	else if (number[0] == '1')
	{
		for (int i = 1; i < number.size(); i++)
		{
			if (number[i] != '0')
				return 2;
		}
		return 1;
	}
}

bool BigFloat::operator==(BigFloat a)
{
	if (this->number.size() != a.number.size())
		return false;
	for (int i = 0; i < this->number.size(); i++)
	{
		if (this->number[i] != a.number[i])
			return false;
	}
	return true;
}


