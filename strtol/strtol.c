#include <string.h>
#include <limits.h>
#include <errno.h>

int convertDigit(int c)
{
	if(c >= '0' && c <= '9') return c - '0';
	if(c >= 'a' && c <= 'z') return c - ('a' - 10);;
	if(c >= 'A' && c <= 'Z') return c - ('A' - 10);
	return -1;
}

void setBaseIf0(int* base, int* i, int len, const char* nPtr)
{
	for(int k = 0; k < len; k++)
	{
		if(nPtr[k] >= '1' && nPtr[k] <= '9') 
		{
			*base = 10;
			return;
		}
		else if(nPtr[k] == '0' && nPtr[k + 1] != 'x' && nPtr[k + 1] != 'X')
		{
			*base = 8;
			return;
			
		}
		else if(nPtr[k] == '0' && (nPtr[k + 1] == 'x' || nPtr[k + 1] == 'X'))
		{
			*base = 16;
			return;
		}
		else if(nPtr[k] != ' ' && nPtr[k] != '\n' && nPtr[k] != '\t' && nPtr[k] != '-' && nPtr[k] != '+')
		{
			*i = 0;
			return;
		}
		
	}
}

int setSign(int charAppered, int* signAppered, int nextNum, int base, int** sign, int c, int** j)
{
	if(charAppered != 0 || *signAppered != 0) return -1;
	if(convertDigit(nextNum) != -1)
	{
		**sign = (c == '-') ? -1 : 1;
		*signAppered = 1;
	}else
	{
		**j = 0;
		return -1;
	}
	return 0;
}

void convertIntoNumber(int c, int* sign, int base, long** num, int* charAppered)
{
	if(c != 0 || *charAppered != 0)
	{
		if(**num > (LONG_MAX - c)/base && *sign > 0)
		{
			errno = ERANGE;
			**num = LONG_MAX;
		}
		else if(**num < (LONG_MIN + c)/base && *sign < 0)
		{
			errno = ERANGE;
			**num = LONG_MIN;
		}
		else **num = **num * base + c * *sign;	
		*charAppered = 1;
	}
}

int getChar(const char* nPtr, int c, int* sign, int* j, int base, int* charAppered)
{
	int signAppered = 0;
	c = nPtr[*j];
	if(c == '0' && !(nPtr[*j + 1] == 'x' || nPtr[*j + 1] == 'X')) *charAppered = 1;
	if((c == 'x' || c == 'X') && nPtr[*j - 1] == '0' && convertDigit(nPtr[*j + 1]) < base && convertDigit(nPtr[*j + 1]) >= 0 && *charAppered == 0)
	{
		*charAppered = 1;
		return 0;
	}
	if(c == '-' || c == '+') return setSign(*charAppered, &signAppered, nPtr[*j + 1], base, &sign, c, &j);
	if((c == ' ' || c == '\n' || c == '\t')) return (*charAppered == 0) ? 0 : -1;
	if(convertDigit(c) != -1) return convertDigit(c);
	return -1;
}

int readString(const char* nPtr, int len, int base, long* num, int* i)
{
	int c;
	int charAppered = 0;
	int sign = 1;
	int j;
	for(j = *i; j < len; j++)
	{
		c = getChar(nPtr, c, &sign, &j, base, &charAppered);
		if(c >= 0 && c < base) convertIntoNumber(c, &sign, base, &num, &charAppered);
		else break;
	}
	*i = j;
	return errno;
}

long strtol(const char* nPtr, char** endPtr, int base)
{
	int len = strlen(nPtr);
	long num = 0;
	int i = 0;
	if(base == 0) setBaseIf0(&base, &i, len, nPtr);
	errno = (base >= 0 && base <= 36 && base != 1) ? readString(nPtr, len, base, &num, &i) : EINVAL;
	if(endPtr) *endPtr = (char*)(nPtr + i);
	return num;
}
