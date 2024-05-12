#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "gather_data.h"

void generateAccountNumber(account_t* account)
{
	char accountNumber[ACCOUNT_NUMBER_LENGHT] = "PL----------\0";
	char letter;
	int ranNum;
	srand(time(NULL));
	FILE* file = fopen(ACCOUNTS_FILE, "rb");
	if(file)
	{
		do {
			for (int i = 2; i < ACCOUNT_NUMBER_LENGHT - 1; i++) {
				ranNum = rand() % 10;
				letter = '0' + ranNum;
				accountNumber[i] = letter;
			}
		} while (getSpecificClientData(file, (char*)accountNumber, account, LOOKING_FOR_ACCOUNT_NUMBER));
		strcpy(account->accountNumber, accountNumber);
		fclose(file);
	}
}

void getName(char* title, account_t* account)
{
	char input[STRING_MAX_LENGHT];
	int tries = 0;
	do {
		showHeader(title);
		if (tries)
			printf("Name must contain only letters\n");
		printf("Enter your name\n");
		fgets(input, STRING_MAX_LENGHT, stdin);
		tries++;
	} while (!isCorrectString(input) || strlen(input) < MINIMAL_LEN_OF_INPUT);
	strcpy(account->name, input);
	account->name[strlen(account->name) - 1] = '\0';
}

void getSurname(char* title, account_t* account)
{
	
	char input[STRING_MAX_LENGHT];
	int tries = 0;
	do {
		showHeader(title);
		if (tries)
			printf("Surame must contain only letters\n");
		printf("Enter your surname\n");
		fgets(input, STRING_MAX_LENGHT, stdin);
		tries++;
	} while (!isCorrectString(input) || strlen(input) < MINIMAL_LEN_OF_INPUT);
	strcpy(account->surname, input);
	account->surname[strlen(account->surname) - 1] = '\0';
}

void getAdress(char* title, account_t* account)
{
	char input[STRING_MAX_LENGHT];
	do
	{
		showHeader(title);
		printf("Enter your adress\n");
		fgets(input, STRING_MAX_LENGHT, stdin);
	}while(strlen(input) < MINIMAL_LEN_OF_INPUT);
	strcpy(account->adress, input);
	account->adress[strlen(account->adress) - 1] = '\0';
}

void getPesel(char* title, account_t* account)
{
	int tries = 0;
	do {
		showHeader(title);
		if (tries)
			printf("Pesel must contain only digits and have 11 of them\n");
		printf("Enter your pesel\n");
		scanf("%s", account->pesel);
		while (getchar() != '\n');
		tries++;
	} while (strlen(account->pesel) != PESEL_LENGHT ||
			 !isNumber(account->pesel));
}

void getAccountBalance(char* title, account_t* account)
{
	int check = 0;
	do {
		showHeader(title);
		printf("Enter your account balance\n");
		check = scanf("%lf", &account->balance);
		while(getchar() != '\n');
	} while (account->balance < 0 || account->balance > MAX_MONEY_AMOUNT ||
			 check == 0);
}

void getAccountLoan(char* title, account_t* account)
{
	int check = 0;
	do {
		showHeader(title);
		printf("Enter your current loan\n");
		check = scanf("%lf", &account->loan);
		while(getchar() != '\n');
	} while (account->loan < 0 || account->loan > MAX_MONEY_AMOUNT ||
			 check == 0);
}

void gatherPersonalData(char* title, account_t* account)
{
	getName(title, account);
	getSurname(title, account);
	getAdress(title, account);
	getPesel(title, account);
}

void gatherDataForAccount(char* title, account_t* account, long long id)
{
	generateAccountNumber(account);
	gatherPersonalData(title, account);
	getAccountBalance(title, account);
	getAccountLoan(title, account);
	account->id = id;
	account->installment = account->loan * INTEREST;
}
