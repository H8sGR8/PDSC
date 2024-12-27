#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic_functions.h"

bool isInFile(int lookingFor, account_t* account, char* STypeInput)
{
	long long LLTypeInput;
	if(lookingFor == 1)
		LLTypeInput = strtoll(STypeInput, NULL, DECIMAL_SYSYEM);
	switch (lookingFor) {
		case (1):
			if (account->id == LLTypeInput)
				return true;
			break;
		case (2):
			if (!strcmp(account->accountNumber, STypeInput))
				return true;
			break;
		case (3):
			if (!strcmp(account->name, STypeInput))
				return true;
			break;
		case (4):
			if (!strcmp(account->surname, STypeInput))
				return true;
			break;
		case (5):
			if (!strcmp(account->adress, STypeInput))
				return true;
			break;
		case (6):
			if (!strcmp(account->pesel, STypeInput))
				return true;
			break;
	}
	return false;
}

bool getClientData(FILE* file, char* input, account_t* account, int lookingFor)
{
	return (fread(account, sizeof(*account), 1, file)) ? true : false;
}

bool getSpecificClientData(FILE* file, char* STypeInput, account_t* account, int lookingFor)
{
	while (!feof(file))
	{
		if(getClientData(file, NULL, account, 0) && isInFile(lookingFor, account, STypeInput)) 
			return true;
	}
	return false;	
}

void saveToFile(FILE* file, account_t account)
{
	fwrite(&account, sizeof(account), 1, file);
}

void updateFile(FILE* file, account_t account)
{
	fseek(file, (account.id - 1) * sizeof(account), SEEK_SET);
	saveToFile(file, account);
}

long long getNextId()
{
	long long numOfLines = 1;
	FILE* file = fopen(ACCOUNTS_FILE, "rb");
	account_t account;
	if (!file)
		return 0;
	while (!feof(file)) {
		if (getClientData(file, NULL, &account, 0))
			numOfLines += 1;
	}
	fclose(file);
	return numOfLines;
}

int checkNumberOfDigitsBeforePoint(long long num1, double num2)
{
	long long newNum = (num1 == -1) ? (long long)num2 : num1;
	int digitsBeforePoint = 1;
	while (newNum > 9) {
		newNum /= 10;
		digitsBeforePoint++;
	}
	return digitsBeforePoint;
}

void setTableWidth(account_t account, accountLen_t* accountLen)
{
	if (checkNumberOfDigitsBeforePoint(account.id, -1) > accountLen->idLen)
		accountLen->idLen = checkNumberOfDigitsBeforePoint(account.id, -1);
	if ((int)strlen(account.name) > accountLen->nameLen)
		accountLen->nameLen = (int)strlen(account.name);
	if ((int)strlen(account.surname) > accountLen->surnameLen)
		accountLen->surnameLen = (int)strlen(account.surname);
	if ((int)strlen(account.adress) > accountLen->adressLen)
		accountLen->adressLen = (int)strlen(account.adress);
	if (checkNumberOfDigitsBeforePoint(-1, account.balance) >
		accountLen->balanceLen - POINT_AND_AFTER)
		accountLen->balanceLen =
			checkNumberOfDigitsBeforePoint(-1, account.balance) + POINT_AND_AFTER;
	if (checkNumberOfDigitsBeforePoint(-1, account.loan) >
		accountLen->loanLen - POINT_AND_AFTER)
		accountLen->loanLen =
			checkNumberOfDigitsBeforePoint(-1, account.loan) + POINT_AND_AFTER;
	if (checkNumberOfDigitsBeforePoint(-1, account.installment) >
		accountLen->installmentLen - POINT_AND_AFTER)
		accountLen->installmentLen =
			checkNumberOfDigitsBeforePoint(-1, account.installment) + POINT_AND_AFTER;
	accountLen->wholeLen = CONSTANT_ELEMENTS_IN_TABLE + accountLen->idLen +
						   accountLen->accountNumberLen + accountLen->nameLen +
						   accountLen->surnameLen + accountLen->adressLen +
						   accountLen->peselLen + accountLen->balanceLen +
						   accountLen->loanLen + accountLen->installmentLen;
}

void endOperation(FILE* file, int* stage)
{
	if(file) fclose(file);
	*stage = 0;
}

void changeUpperIntoLower(char* upperCase, char lowerCase[])
{
	for (int i = 0; i < strlen(upperCase); i++) {
		lowerCase[i] =
			(isalpha(upperCase[i])) ? tolower(upperCase[i]) : upperCase[i];
	}
	lowerCase[strlen(upperCase)] = '\0';
}

void getLastPartOfSentence(char sentence[], char lastPartOfSentence[],
						   int wantedNumOfWords)
{
	int numOfWords = 0;
	for (int i = strlen(sentence) - 1; i >= 0; i--) {
		if (sentence[i] == ' ')
			numOfWords++;
		if (numOfWords == wantedNumOfWords) {
			for (int j = i + 1; j < strlen(sentence); j++) {
				lastPartOfSentence[j - (i + 1)] = sentence[j];
			}
			lastPartOfSentence[strlen(sentence) - (i + 1)] = '\0';
			return;
		}
	}
}

int chooseOption(int numberOfOptions)
{
	int option;
	int check;
	check = scanf("%d", &option);
	while (getchar() != '\n')
		;
	return (check == 0 || !(option >= 1 && option <= numberOfOptions)) ? 0 : option;
}

bool isNumber(char array[])
{
	for (int i = 0; i < strlen(array); i++) {
		if (!isdigit(array[i]))
			return false;
	}
	return true;
}

bool isCorrectString(char array[])
{
	for (int i = 0; i < strlen(array) - 1; i++) {
		if (!isalpha(array[i]))
			return false;
	}
	return true;
}
