#include "QFloat.h"

QFloat::QFloat()
{
}

QFloat::~QFloat()
{
}

unsigned int QFloat::MulAdd(const unsigned int& mul, const unsigned int add) {
	unsigned long long temp = 0;// tranh tran so
	// xu ly du lieu theo he co so (mul) de dua vao thanh cac block luu trong data[]
	temp = (unsigned long long)data[3] * (unsigned long long)mul + add;
	data[3] = temp;

	temp = (unsigned long long)data[2] * (unsigned long long)mul + (temp >> 32);
	data[2] = temp;
	//temp >> 32 de:
	temp = (unsigned long long)data[1] * (unsigned long long)mul + (temp >> 32);
	data[1] = temp;

	temp = (unsigned long long)data[0] * (unsigned long long)mul + (temp >> 32);
	data[0] = temp;

	return temp >> 32;
}

void QFloat::ScanQFloat()
{
	int mode;
	cout << "Nhap mode (2, 10): ";
	cin >> mode;
	string inp;
	
	if (mode == 10)
	{
		cin.ignore();
		getline(cin, inp);
		DectoBin(inp, inp);
	}
	else if (mode == 2)
	{
		do
		{
			cin.ignore();
			getline(cin, inp);
		} while (inp.size() > 128);
	}

	string::iterator it = inp.begin();
	while (it != inp.end())
		if (*it >= '0' && *it <= '1')
		{
			if (MulAdd(2, *it - '0'))
			{
				//errno = ERANGE;
				//return NULL;
			}
			it++;
		}
}

void QFloat::PrintQFloat()
{
	string ketqua;
	BintoDec(ketqua);
	cout << ketqua << endl;
}

unsigned int QFloat::getBit(int i)
{
	return (data[i / 32] >> (31 - i % 32)) & 1;
}

void QFloat::BintoDec(string& ketqua)
{
	ketqua.clear();
	bool normal = true;
	// Phan dau
	unsigned int sign = this->getBit(0);
	// Phan mu
	int E = 0;
	for (int i = 1; i < 16; i++)
	{
		E = E + pow(2, 15 - i) * this->getBit(i);
	}
	if (E == 0) // TH Zero va Denor
	{
		for (int i = 16; i < 128; i++)
		{
			if (this->getBit(i))
			{
				normal = false;
				E = 1;
				break;
			}
		}
		if (normal)
		{
			if (sign)
				ketqua = "-";
			ketqua = ketqua + "0.0";
			return;
		}
	}
	bool inf = true;
	for (int i = 1; i < 16; i++) // TH Inf va NaN
	{
		if (!this->getBit(i))
		{
			inf = false;
			break;
		}
	}
	if (inf)
	{
		bool NaN = false;
		for (int i = 16; i < 128; i++)
		{
			if (this->getBit(i))
			{
				NaN = true;
				break;
			}
		}
		if (NaN)
			ketqua = "NaN";
		else
		{
			if (sign)
				ketqua = "-";
			ketqua = ketqua + "Infinity";
		}
	}
	E = E - 16383; // 16383 = 2^(k-1) - 1, k = 15
	// Phan tri
	BigFloat temp;
	BigFloat anthr_temp;
	BigFloat result;
	BigFloat two;
	two.number.push_back('2'); two.exponent++;
	if (E >= 0)
	{
		int i;
		int mu = 0;
		if (E <= 112)
			i = 15 + E;
		else
			i = 15 + 112;
		BigFloat dummy;
		for (int j = i; j > 15; E--, j--)
		{
			if (this->getBit(j))
			{
				dummy = dummy.upPow(mu, two);
				dummy.exponent++;
				temp = temp + dummy;
			}
			mu++;
		}
		if (normal)
		{
			dummy = dummy.upPow(mu, two);
			dummy.exponent++;
			temp = temp + temp.upPow(mu, two);
		}
		mu = -1;
		for (int j = i + 1; j < 128; j++)
		{
			anthr_temp = anthr_temp + anthr_temp.downPow(mu);
			mu--;
		}
	}
	else if (E < 0)
	{
		int mu = -1;
		for (int i = 16; i < 128; i++)
		{
			if (this->getBit(i))
				temp = temp + temp.downPow(mu);
			mu--;
		}
		if (normal)
		{
			result.number.push_back('1'); result.exponent++;
		}
	}
	result = result + temp + anthr_temp;
	result.deleteZero();
	if (sign)
		ketqua = "-";
	for (int i = 0; i < result.number.size(); i++)
	{
		if (i == result.exponent)
		{
			if (result.exponent == 0)
				ketqua = ketqua + '0';
			ketqua = ketqua + ".";
		}
		ketqua = ketqua + result.number[i];
	}
	ketqua = ketqua + " * 2^" + to_string(E);
}

void QFloat::DectoBin(string dec, string &ketqua)
{
	int sign = 0;
	bool zero = false;
	bool zero_zero = false;
	vector<char> Dec;
	copy(dec.begin(), dec.end(), back_inserter(Dec));
	if (Dec[0] == '-')
		sign = 1;
	int i, point = 0;

	// Phan nguyen
	string temp;
	for ((sign) ? i = 1 : i = 0; i < Dec.size(); i++)
	{
		if (Dec[sign] == '0'); // Kiem tra so dau tien co phai bang 0
		{
			zero = true;
			temp = temp + Dec[i];
			if (Dec[sign + 2] == '0' && Dec.size() == 3)
				zero_zero = true;
			break;
		}
		if (Dec[i] == '.')
		{
			point = i;
			break;
		}
		else
			temp = temp + Dec[i];
	}
	// TH so la so 0.0
	if (zero_zero)
	{
		ketqua = sign + '0';
		for (int i = sign; i < 128; i++)
			ketqua += '0';
	}
	// Phan thap phan
	string anthr_temp = "0.";

	for (int j = i + 1; j < Dec.size(); j++)
	{
		anthr_temp += Dec[j];
	}
	// Chuyen hoa 1: Phan nguyen
	int E = 0;
	BigFloat left;
	if (!zero)
	{
		left.Nhap(temp);
		temp.clear();
		int r;
		bool check = true;
		do
		{
			left.DividebyTwo(left, r);
			temp.insert(0, 1, r + '0');
			if (left.number[0] == '0')
				check = false;
		} while (check);
		temp.erase(0, 1);
		E = temp.size() + 16383; // 16383 = 2^(k-1) - 1, k = 15
	}
	// Chuyen hoa 2: Phan thap phan
	vector<BigFloat> storage;
	bool first_time = true;
	int same = 0;
	bool complete = false;
	int compare = 1;
	BigFloat right; right.Nhap(anthr_temp);
	anthr_temp.clear();
	do
	{
		BigFloat two;
		two.number.push_back('2'); two.exponent++;
		right = right * two;
		if (!first_time)
		{
			for (int i = 0; i < storage.size(); i++)
			{
				if (right == storage[i])
				{
					same = i;
					break;
				}
			}
		}
		if (same)
			break;
		compare = right.compareOne();
		if (compare == 0)
			anthr_temp += '0';
		else if (compare == 1)
		{
			anthr_temp += '1';
			complete = true;
			break;
		}
		else
		{
			anthr_temp += '1';
			right.number[0] = '0';
		}
		storage.push_back(right);
		first_time = false;
	} while (compare == 0 || compare == 2);
	int j = same;
	int save = 0;
	for (int i = 0; i < 112 - left.number.size() - right.number.size(); i++)
	{
		if (complete)
			anthr_temp += '0';
		else
		{
			if (j == storage.size())
				j = same;
			compare = storage[j].compareOne();
			if (compare == 0)
				anthr_temp += '0';
			else if (compare == 2)
				anthr_temp += '1';
			j++;
			save = j;
		}
	}
	if (zero)
	{
		for (int i = 0; i < anthr_temp.size(); i++)
		{
			E--;
			if (anthr_temp[i] == '1')
			{
				anthr_temp.erase(0, 1);
				break;
			}
			else
				anthr_temp.erase(0, 1);
		}
		for (int i = 0; i < (E * -1); i++)
		{
			if (complete)
				anthr_temp += '0';
			else
			{
				if (save == storage.size())
					save = 0;
				compare = storage[save].compareOne();
				if (compare == 0)
					anthr_temp += '0';
				else if (compare == 2)
					anthr_temp += '1';
				save++;
			}
		}
		E = E + 16383; // 16383 = 2^(k-1) - 1, k = 15
	}
	// Phan tri
	string result;
	if (zero)
		result = anthr_temp;
	else
		result = temp + anthr_temp;

	// Lam tron len cho phan tri
	char remainder = '0';
	// Neu la so duong
	if (!sign)
	{
		if (result[result.size()-1] == '0')
			result[result.size()-1] == '1';
		else if (result[result.size()-1] == '1')
		{
			result[result.size()-1] == '0';
			remainder = '1';
		}
		for (int i = result.size() - 2; i >= 0; i--)
		{
			if (result[i] == '0' && remainder == '0')
			{
				result[i] = '0';
			}
			else if (result[i] == '0' && remainder == '1')
			{
				result[i] == '1';
				remainder = '0';
			}
			else if (result[i] == '1' && remainder == '0')
				result[i] == '1';
			else if (result[i] == '1' && remainder == '1')
			{
				result[i] == '0';
				remainder = '1';
			}
		}
		remainder = '0';
	}
	// Neu la so am
	else
	{
		if (result[result.size() - 1] == '0')
		{
			result[result.size() - 1] == '1';
			remainder = '1';
		}
		else if (result[result.size() - 1] == '1')
			result[result.size() - 1] == '0';
		for (int i = result.size() - 2; i >= 0; i--)
		{
			if (result[i] == '0' && remainder == '0')
			{
				result[i] = '0';
			}
			else if (result[i] == '0' && remainder == '1')
			{
				result[i] == '1';
				remainder = '1';
			}
			else if (result[i] == '1' && remainder == '0')
				result[i] == '1';
			else if (result[i] == '1' && remainder == '1')
			{
				result[i] == '0';
				remainder = '0';
			}
		}
	}
	string Mu = "000000000000000";
	int k = 1;
	while (E)
	{
		Mu[Mu.size() - k] = (E % 2) + '0';
		E /= 2;
		k++;
	}
	ketqua = sign + '0';
	ketqua += Mu;
	ketqua += result;
}

QFloat QFloat::operator=(QFloat that)
{
	if (this != &that)
	{
		for (int i = 0; i < 4; i++)
			this->data[i] = that.data[i];
	}
	return *this;
}

bool QFloat::operator==(QFloat that)
{
	if (this == &that)
		return true;
	for (int i = 0; i < 4; i++)
	{
		if (this->data[i] != that.data[i])
			return false;
	}
	return true;
}

