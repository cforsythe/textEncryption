//####################################################################################################################################################
//
//		Super cool long number class "arbritary_uint"
//
//		Usage:
//			
//			Type:	
//				#include "auint.h" 
//				in your main cpp file, other includes like iostream are NOT necessary, they are already included here!
//
//			Initializing:
//				arbritary_uint prime("139"), generator("3"), private_key("12"), public_key;
//				public_key.from_String("2342315346766765634534645765765875");
//
//			Calculation:
//				Possible native operations (all participants must be "arbritary_uint"):
//					+,-,*,/,+=,-=,/=,*=,%,%=
//					I.e.:	Message = (K_inv*C2) % prime;
//
//				Additional methods (all participants must be "arbritary_uint"):
//					a = a^x % p						...		a.powAssignUnderMod(x, p);
//					K_inv = K inverse_mod p			...		K_inv = inverse_modulo(K,p);
//				
//				Print the value:
//					arbritary_uint prime("139");
//					cout << prime.toString();
//						or
//					string s = prime.toString();
//					cout << s;
//
//
//#####################################################################################################################################################


// typical ELGAMAL start:
//arbritary_uint prime("139"), generator("3"),private_key, public_key, u("44"), kk;
//private_key.randomize(120);
//
//
//printf("\n");
//printf("\ngenerator:     %s", generator.toString().c_str());
//printf("\nprime:         %s", prime.toString().c_str());
//printf("\nprivate_key:   %s", private_key.toString().c_str());
//
//public_key = generator;
//public_key.powAssignUnderMod(private_key, prime);
//
//cout << endl;
//printf("public key:   %s", public_key.toString().c_str());
//cout << endl;



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <time.h> 
#include <stack> 
#include <sstream>  
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>



using namespace std;
typedef unsigned long long int uint64;
struct uint128
{
	uint64 Hi;
	uint64 Lo;
};

struct aint_
{
int *b32;
int len;//in 32 bit words
};



class arbritary_uint
{
public:
	
	bool isOdd()
		{
		return (digits[0] & 0x1) != 0;
		}
	
	vector<unsigned int> digits;
	bool negative;
~arbritary_uint()
	{
	}

arbritary_uint()
	{
	digits.push_back(0);
	negative = false;
	}
void clear()
	{
	digits.clear();
	digits.push_back(0);
	negative = false;
	}
 void from_Ascii(const char* str)
	{
		digits.clear();
	if (strlen(str) <= 0)
		{
		digits.push_back(0);
		return;
		}
		

	for (int ii = 0; ii < strlen(str); ii+=2)
		{
		int s=0;
		
		BYTE *pb = (BYTE*)&s;

		pb[0] = str[ii];
		pb[1] = str[ii+1];
		if(pb[0]==0)
			break;
		
		digits.push_back(s);
		int z;
		z = 0;
		}
	negative = false;	
	}
void to_Ascii(char* str)
	{
	if (strlen(str) <= 0)
		return;
	int sii = 0;
	for (int ii = 0; ii < digits.size(); ii++)
		{
		int i = digits[ii];
		BYTE *pb = (BYTE*)&i;
		str[sii++] = pb[0];
		str[sii++] = pb[1];
		}
	str[sii++]=0;
	}
void randomize(int digit_anz)
	{
	char nums[] = { 0,1,2,3,4,5,6,7,8,9 };

	digits.push_back(0);
	negative = false;
	arbritary_uint ten;
	ten.digits[0] = 10;

	for (int ii = 0; ii < digit_anz; ii++)
		{
		int r = rand() % 10;
		if (r<0 || r>9) r = 0;
		arbritary_uint digit;
		digit.digits[0] = nums[r];
		*this *= ten;
		*this += digit;
		}
	}
void addWithoutSign(arbritary_uint& c, const arbritary_uint& a, const arbritary_uint& b)
	{
	int sum_n_carry = 0;
	int n = (int)a.digits.size();
	if (n < (int)b.digits.size())
		{
		n = b.digits.size();
		}
	c.digits.resize(n);
	for (int i = 0; i < n; ++i)
		{
		unsigned short a_digit = 0;
		unsigned short b_digit = 0;
		if (i < (int)a.digits.size())
			{
			a_digit = a.digits[i];
			}
		if (i < (int)b.digits.size())
			{
			b_digit = b.digits[i];
			}
		sum_n_carry += a_digit + b_digit;
		c.digits[i] = (sum_n_carry & 0xFFFF);
		sum_n_carry >>= 16;
		}
	if (sum_n_carry != 0)
		{
		putCarryInfront(c, sum_n_carry);
		}
	while (c.digits.size() > 1 && c.digits.back() == 0)
		{
		c.digits.pop_back();
		}
	//std::cout << a.toString() << " + " << b.toString() << " == " << c.toString() << std::endl;
	}
void subWithoutSign(arbritary_uint& c, const arbritary_uint& a, const arbritary_uint& b)
	{
	int sub_n_borrow = 0;
	int n = a.digits.size();
	if (n < (int)b.digits.size())
		n = (int)b.digits.size();
	c.digits.resize(n);
	for (int i = 0; i < n; ++i)
		{
		unsigned short a_digit = 0;
		unsigned short b_digit = 0;
		if (i < (int)a.digits.size())
			a_digit = a.digits[i];
		if (i < (int)b.digits.size())
			b_digit = b.digits[i];
		sub_n_borrow += a_digit - b_digit;
		if (sub_n_borrow >= 0)
			{
			c.digits[i] = sub_n_borrow;
			sub_n_borrow = 0;
			}
		else
			{
			c.digits[i] = 0x10000 + sub_n_borrow;
			sub_n_borrow = -1;
			}
		}
	while (c.digits.size() > 1 && c.digits.back() == 0)
		{
		c.digits.pop_back();
		}
	//std::cout << a.toString() << " - " << b.toString() << " == " << c.toString() << std::endl;
	}
int cmpWithoutSign(const arbritary_uint& a, const arbritary_uint& b)
	{
	int n = (int)a.digits.size();
	if (n < (int)b.digits.size())
		n = (int)b.digits.size();
	//std::cout << "cmp(" << a.toString() << ", " << b.toString() << ") == ";
	for (int i = n - 1; i >= 0; --i)
		{
		unsigned short a_digit = 0;
		unsigned short b_digit = 0;
		if (i < (int)a.digits.size())
			a_digit = a.digits[i];
		if (i < (int)b.digits.size())
			b_digit = b.digits[i];
		if (a_digit < b_digit)
			{
			//std::cout << "-1" << std::endl;
			return -1;
			}
		else if (a_digit > b_digit)
			{
			//std::cout << "+1" << std::endl;
			return +1;
			}
		}
	//std::cout << "0" << std::endl;
	return 0;
	}

void from_int(unsigned int ui)
	{
	digits.push_back(0);
	negative = false;
	digits[0] = ui;
	}
void multByDigitWithoutSign(arbritary_uint& c, const arbritary_uint& a, unsigned short b)
	{
	unsigned int mult_n_carry = 0;
	c.digits.clear();
	c.digits.resize(a.digits.size());
	for (int i = 0; i < (int)a.digits.size(); ++i)
		{
		unsigned short a_digit = 0;
		unsigned short b_digit = b;
		if (i < (int)a.digits.size())
			a_digit = a.digits[i];
		mult_n_carry += a_digit * b_digit;
		c.digits[i] = (mult_n_carry & 0xFFFF);
		mult_n_carry >>= 16;
		}
	if (mult_n_carry != 0)
		{
		putCarryInfront(c, mult_n_carry);
		}
	//std::cout << a.toString() << " x " << b << " == " << c.toString() << std::endl;
	}
void shiftLeftByBase(arbritary_uint& b, const arbritary_uint& a, int times)
	{
	b.digits.resize(a.digits.size() + times);
	for (int i = 0; i < times; ++i)
		{
		b.digits[i] = 0;
		}
	for (int i = 0; i < (int)a.digits.size(); ++i)
		{
		b.digits[i + times] = a.digits[i];
		}
	}
void shiftRight(arbritary_uint& a)
	{
	//std::cout << "shr " << a.toString() << " == ";
	for (int i = 0; i < (int)a.digits.size(); ++i)
		{
		a.digits[i] >>= 1;
		if (i + 1 < (int)a.digits.size())
			{
			if ((a.digits[i + 1] & 0x1) != 0)
				{
				a.digits[i] |= 0x8000;
				}
			}
		}
	//std::cout << a.toString() << std::endl;
	}

void shiftLeft(arbritary_uint& a)
	{
	bool lastBit = false;
	for (int i = 0; i < (int)a.digits.size(); ++i)
		{
		bool bit = (a.digits[i] & 0x8000) != 0;
		a.digits[i] <<= 1;
		if (lastBit)
			a.digits[i] |= 1;
		lastBit = bit;
		}
	if (lastBit)
		{
		a.digits.push_back(1);
		}
	}

void putCarryInfront(arbritary_uint& a, unsigned short carry)
	{
	arbritary_uint b;
	b.negative = a.negative;
	b.digits.resize(a.digits.size() + 1);
	b.digits[a.digits.size()] = carry;
	for (int i = 0; i < (int)a.digits.size(); ++i)
		{
		b.digits[i] = a.digits[i];
		}
	a.digits.swap(b.digits);
	}

void divideWithoutSign(arbritary_uint& c, arbritary_uint& d, const arbritary_uint& a, const arbritary_uint& b)
	{
	c.digits.clear();
	c.digits.push_back(0);
	arbritary_uint two("2");
	arbritary_uint e = b;
	arbritary_uint f("1");
	arbritary_uint g = a;
	arbritary_uint one("1");
	while (cmpWithoutSign(g, e) >= 0)
		{
		shiftLeft(e);
		shiftLeft(f);
		}
	shiftRight(e);
	shiftRight(f);
	while (cmpWithoutSign(g, b) >= 0)
		{
		g -= e;
		c += f;
		while (cmpWithoutSign(g, e) < 0)
			{
			shiftRight(e);
			shiftRight(f);
			}
		}
	e = c;
	e *= b;
	f = a;
	f -= e;
	d = f;
	}
arbritary_uint(const arbritary_uint& other)
	{
	digits = other.digits;
	negative = other.negative;
	}
arbritary_uint(const char* str){
	from_String(str);
}
void from_String(const char* str)
	{
	digits.push_back(0);
	negative = false;
	arbritary_uint ten;
	ten.digits[0] = 10;
	const char* c = str;
	bool make_negative = false;
	if (*c == '-')
		{
		make_negative = true;
		++c;
		}
	while (*c != 0)
		{
		arbritary_uint digit;
		digit.digits[0] = *c - '0';
		*this *= ten;
		*this += digit;
		++c;
		}
	negative = make_negative;
	}

arbritary_uint& operator=(const arbritary_uint& other)
	{
	if (this == &other) // handle self assignment
		return *this;
	digits = other.digits;
	negative = other.negative;
	return *this;
	}
arbritary_uint& operator+=(const arbritary_uint& other)
	{
	arbritary_uint result;
	result = *this + other;
	negative = result.negative;
	digits.swap(result.digits);
	return *this;
	}
arbritary_uint& operator-=(const arbritary_uint& other)
	{
	arbritary_uint neg_other = other;
	neg_other.negative = !neg_other.negative;
	return *this += neg_other;
	}
arbritary_uint& operator*=(const arbritary_uint& rhs)
	{
	arbritary_uint result;
	result = *this * rhs;
	negative = result.negative;
	digits.swap(result.digits);
	return *this;
	}
arbritary_uint& operator/=(const arbritary_uint& other)
	{
	arbritary_uint result, tmp;
	divideWithoutSign(result, tmp, *this, other);
	result.negative = (negative != other.negative);
	negative = result.negative;
	digits.swap(result.digits);
	return *this;
	}
arbritary_uint operator+(const arbritary_uint& rhs)
	{
	arbritary_uint result;
	if (negative)
		{
		if (rhs.negative)
			{
			result.negative = true;
			addWithoutSign(result, *this, rhs);
			}
		else
			{
			int a = cmpWithoutSign(*this, rhs);
			if (a < 0)
				{
				result.negative = false;
				subWithoutSign(result, rhs, *this);
				}
			else if (a > 0)
				{
				result.negative = true;
				subWithoutSign(result, *this, rhs);
				}
			else
				{
				result.negative = false;
				result.digits.clear();
				result.digits.push_back(0);
				}
			}
		}
	else
		{
		if (rhs.negative)
			{
			int a = cmpWithoutSign(*this, rhs);
			if (a < 0)
				{
				result.negative = true;
				subWithoutSign(result, rhs, *this);
				}
			else if (a > 0)
				{
				result.negative = false;
				subWithoutSign(result, *this, rhs);
				}
			else
				{
				result.negative = false;
				result.digits.clear();
				result.digits.push_back(0);
				}
			}
		else
			{
			result.negative = false;
			addWithoutSign(result, *this, rhs);
			}
		}
	return result;
	}
arbritary_uint operator-(const arbritary_uint& rhs)
	{
	arbritary_uint neg_other = rhs;
	neg_other.negative = !neg_other.negative;
	return *this + neg_other;
	}
arbritary_uint operator*(const arbritary_uint& rhs)
	{
	arbritary_uint result;
	for (int i = 0; i < (int)digits.size(); ++i)
		{
		arbritary_uint mult, shift, add;
		multByDigitWithoutSign(mult, rhs, digits[i]);

		shiftLeftByBase(shift, mult, i);

		addWithoutSign(add, result, shift);

		result = add;
		}
	if (negative != rhs.negative)
		{
		result.negative = true;
		}
	else
		{
		result.negative = false;
		}
	return result;
	}
arbritary_uint operator/(const arbritary_uint& rhs)
	{
	arbritary_uint result, tmp;
	divideWithoutSign(result, tmp, *this, rhs);
	result.negative = (negative != rhs.negative);
	return result;
	}
arbritary_uint operator%(const arbritary_uint& other)
	{
	arbritary_uint c, d;
	divideWithoutSign(c, d, *this, other);
	return d;
	}
arbritary_uint& operator%=(const arbritary_uint& other)
	{
	arbritary_uint c, d;
	divideWithoutSign(c, d, *this, other);
	*this = d;
	return *this;
	}
bool operator>(const arbritary_uint& other)
	{
	if (negative)
		{
		if (other.negative)
			{
			return cmpWithoutSign(*this, other) < 0;
			}
		else
			{
			return false;
			}
		}
	else
		{
		if (other.negative)
			{
			return true;
			}
		else
			{
			return cmpWithoutSign(*this, other) > 0;
			}
		}
	}
bool operator<(arbritary_uint& rhs)
	{
	arbritary_uint res = *this;
	return (rhs > res);
	}
bool operator==(const arbritary_uint& rhs)
	{
	if (cmpWithoutSign(*this, rhs) == 0)return true;
	return false;
	}
arbritary_uint& inverse_modulo(arbritary_uint a, arbritary_uint modulo)
	{
	arbritary_uint b0 = modulo, t, q;
	arbritary_uint x0("0"), x1("1");
	arbritary_uint const_1("1"), const_0("0");

	if (modulo == const_1)
		{
		negative = const_1.negative;
		digits.swap(const_1.digits);
		}
	int loops = 0;
	int xx0[10]; int xx1[10]; int tt[10]; int mmodulo[10];
	while (a > const_1)
		{
		if (modulo == const_0)
			{
			negative = const_0.negative;
			digits.swap(const_0.digits);
			}
		q = a / modulo;
		t = modulo;
		modulo = a % modulo;
		a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
		
		loops++;
		}
	if (x1 < const_0) x1 += b0;

	negative = x1.negative;
	digits.swap(x1.digits);
	return *this;
	}


arbritary_uint& powAssignUnderMod(const arbritary_uint& exponent, const arbritary_uint& modulus)
	{
	arbritary_uint zero("0");
	arbritary_uint one("1");
	arbritary_uint e = exponent;
	arbritary_uint base = *this;
	*this = one;
	while (cmpWithoutSign(e, zero) != 0)
		{
		//std::cout << e.toString() << " : " << toString() << " : " << base.toString() << std::endl;
		if (e.isOdd())
			{
			*this *= base;
			*this %= modulus;
			}
		shiftRight(e);
		base *= arbritary_uint(base);
		base %= modulus;
		}
	return *this;
	}
std::string toString()
	{
	ostringstream ost;
	if (negative)
		ost << "-";
	arbritary_uint tmp = *this;
	arbritary_uint zero("0");
	arbritary_uint ten("10");
	tmp.negative = false;
	std::stack<char> s;
	while (cmpWithoutSign(tmp, zero) != 0)
		{
		arbritary_uint tmp2, tmp3;
		divideWithoutSign(tmp2, tmp3, tmp, ten);
		s.push((char)(tmp3.digits[0] + '0'));
		tmp = tmp2;
		}
	while (!s.empty())
		{
		ost << s.top();
		s.pop();
		}
	/*
	for (int i = digits.size()-1; i >= 0; --i) {
	os << digits[i];
	if (i != 0) {
	os << ",";
	}
	}
	*/
	return ost.str();
	}

};

int mul_inv(int a, int b)
	{
	int b0 = b, t, q;
	int x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1)
		{
		q = a / b;
		t = b;
		b = a % b;
		a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
		}
	if (x1 < 0) x1 += b0;
	return x1;
	}