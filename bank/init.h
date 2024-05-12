#ifndef __INIT_H__
#define __INIT_H__

#include <stddef.h>


#define ACCOUNTS_FILE "accounts.dat"
#define MAX_MONEY_AMOUNT 9999999999999
#define PESEL_LENGHT 11
#define STRING_MAX_LENGHT 50
#define ACCOUNT_NUMBER_LENGHT 13
#define INTEREST (5.0 / 100)
#define ACCOUNT_ELEMENTS 9
#define CONSTANT_ELEMENTS_IN_TABLE 28
#define DEFAULT_ACCOUNT_LEN {2, 12, 4, 7, 6, 11, 7, 4, 11}
#define POINT_AND_AFTER 3
#define WORDS_FOR_ACCOUNT 3
#define WORDS_FOR_OTHER_DATA 2
#define ADDITIONAL_INSTALLMENTS 2
#define MINIMAL_LEN_OF_INPUT 2
#define LOOKING_FOR_ACCOUNT_NUMBER 2
#define DECIMAL_SYSYEM 10

typedef struct {
	long long id;
	char accountNumber[ACCOUNT_NUMBER_LENGHT];
	char name[STRING_MAX_LENGHT];
	char surname[STRING_MAX_LENGHT];
	char adress[STRING_MAX_LENGHT];
	char pesel[PESEL_LENGHT];
	double balance;
	double loan;
	double installment;
} account_t;

typedef struct {
	int idLen;
	int accountNumberLen;
	int nameLen;
	int surnameLen;
	int adressLen;
	int peselLen;
	int balanceLen;
	int loanLen;
	int installmentLen;
	int wholeLen;
} accountLen_t;

#endif /*__INIT_H__*/
