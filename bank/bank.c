#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MONEY_AMOUNT 9999999999999
#define PESEL_LENGHT 11
#define STRING_MAX_LENGHT 50
#define ACCOUNT_NUMBER_LENGHT 13
#define INTEREST 5 / 100
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

void showHeader(char* header)
{
	system("clear");
	printf("%s\n\n", header);
}

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
		getClientData(file, NULL, account, 0);
		if(isInFile(lookingFor, account, STypeInput)) 
			return true;
	}
	return false;	
}

long long getNextId()
{
	long long numOfLines = 1;
	FILE* file = fopen("accounts.dat", "rb");
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

void showLine(accountLen_t accountLen)
{
	printf("  ");
	for (int i = 0; i < accountLen.wholeLen; i++)
		printf("-");
	printf("\n");
}

void showHeaderForTable(accountLen_t accountLen)
{
	showLine(accountLen);
	printf("  | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s "
		   "|\n",
		   accountLen.idLen, "ID", accountLen.accountNumberLen,
		   "NUMBER", accountLen.nameLen, "NAME",
		   accountLen.surnameLen, "SURNAME", accountLen.adressLen,
		   "ADRESS", accountLen.peselLen, "PESEL",
		   accountLen.balanceLen, "BALANCE", accountLen.loanLen,
		   "LOAN", accountLen.installmentLen, "INSTALLMENT");
}

void showAccount(account_t account, accountLen_t accountLen)
{
	showLine(accountLen);
	printf("  | %*lld | %*s | %*s | %*s | %*s | %*s | %*.2lf | %*.2lf | %*.2lf "
		   "|\n",
		   accountLen.idLen, account.id, accountLen.accountNumberLen,
		   account.accountNumber, accountLen.nameLen, account.name,
		   accountLen.surnameLen, account.surname, accountLen.adressLen,
		   account.adress, accountLen.peselLen, account.pesel,
		   accountLen.balanceLen, account.balance, accountLen.loanLen,
		   account.loan, accountLen.installmentLen, account.installment);
}

void showTable(FILE* file, accountLen_t* accountLen, void* input, int lookingFor, int* printedAccounts, bool (*wantedRecord)(FILE*, char*, account_t*, int))
{
	account_t account;
	int foundedAccountsIds[getNextId() - 1];
	while (!feof(file))
		if (wantedRecord(file, input, &account, lookingFor)) {
			setTableWidth(account, accountLen);
			foundedAccountsIds[*printedAccounts] = account.id;
			*printedAccounts = *printedAccounts + 1;
		}
	showHeaderForTable(*accountLen);
	for(int i = 0; i < *printedAccounts; i++) {
		fseek(file, (foundedAccountsIds[i] - 1) * sizeof(account), SEEK_SET);
		getClientData(file, NULL, &account, 0);
		showAccount(account, *accountLen);
	}
}

bool confirmeOperation(char* title, int** stage, account_t account)
{
	char c;
	accountLen_t accountLen = DEFAULT_ACCOUNT_LEN;
	setTableWidth(account, &accountLen);
	showHeader(title);
	showHeaderForTable(accountLen);
	showAccount(account, accountLen);
	showLine(accountLen);
	printf("Are all data correct?\nPress 'y' if yes\n");
	c = getchar(); 
	if (c == 'y' || c == 'Y') {
		while (getchar() == '\0');
		return true;
	}
	**stage = 0;
	while (getchar() == '\0');
	return false;
}

void waitForQToBackToMenu(char* title, char* message)
{
	while (1) {
		showHeader(title);
		printf("%spress 'q' to go back to accounts menu\n", message);
		if (getchar() == 'q')
			break;
	}
}

void operationDidNotSucceed(FILE* file, char* title)
{
	if (!file)
		waitForQToBackToMenu(title, "File not found\n");
	else
		waitForQToBackToMenu(title, "Record not found\n");
}

void operationSucceeded(accountLen_t accountLen)
{
	showLine(accountLen);
	printf("\npress 'q' to go back to search account menu\n");
	while (getchar() != 'q');
}

void endOperation(FILE* file, int* stage)
{
	if(file) fclose(file);
	*stage = 0;
}

void saveToFile(FILE* file, account_t account)
{
	fwrite(&account, sizeof(account), 1, file);
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

void showMenu()
{
	char* title = "MAIN MENU";
	showHeader(title);
	printf("1.Show accounts\n"
		   "2.Accounts operations\n"
		   "3.Info\n"
		   "4.Quit\n\n"
		   "choose option\n");
}

int chooseOption(int numberOfOptions)
{
	int option;
	int check;
	check = scanf("%d", &option);
	while (getchar() != '\n')
		;
	return (check == 0 || !(option >= 1 && option <= numberOfOptions)) ? 0
																	   : option;
}

void showAccountMenu()
{
	char* title = "ACCOUNTS MENU";
	showHeader(title);
	printf("1.Show all accounts\n"
		   "2.Search for an account\n"
		   "3.Go back to main menu\n\n"
		   "choose option\n");
}

void showAllAccounts(int* accountStage)
{
	char* title = "ALL ACCOUNTS";
	int printedAccounts = 0;
	accountLen_t accountLen = DEFAULT_ACCOUNT_LEN;
	FILE* file = fopen("accounts.dat", "rb");
	if(file)
	{
		showHeader(title);
		showTable(file, &accountLen, NULL, 0, &printedAccounts, getClientData);
	}
	if(printedAccounts) operationSucceeded(accountLen);
	else operationDidNotSucceed(file, title);
	endOperation(file, accountStage);
}

void showSearchForAccountMenu()
{
	char* title = "SEARCH ACCOUNT MENU";
	showHeader(title);
	printf("1.Search by an id\n"
		   "2.Search by an account number\n"
		   "3.Search by a name\n"
		   "4.Search by a surname\n"
		   "5.Search by an adress\n"
		   "6.Search by a pesel\n"
		   "7.Go back to accounts menu\n\n"
		   "choose option\n");
}


void searchInFile(int* searchStage, int lookingFor, char* title)
{
	char input[STRING_MAX_LENGHT];
	char lastPartOfSentence[strlen(title)];
	int printedAccounts = 0;
	changeUpperIntoLower(title, lastPartOfSentence);
	if (lookingFor == LOOKING_FOR_ACCOUNT_NUMBER)
		getLastPartOfSentence(lastPartOfSentence, lastPartOfSentence, WORDS_FOR_ACCOUNT);
	else
		getLastPartOfSentence(lastPartOfSentence, lastPartOfSentence, WORDS_FOR_OTHER_DATA);
	accountLen_t accountLen = DEFAULT_ACCOUNT_LEN;
	showHeader(title);
	printf("Enter %s\n", lastPartOfSentence);
	fgets(input, STRING_MAX_LENGHT, stdin);
	input[strlen(input) - 1] = '\0';
	FILE* file = fopen("accounts.dat", "rb");
	if(file)
	{
		showHeader(title);
		showTable(file, &accountLen, (char*)input, lookingFor, &printedAccounts, getSpecificClientData);
	}
	if(printedAccounts) operationSucceeded(accountLen);
	else operationDidNotSucceed(file, title);
	endOperation(file, searchStage);
}

void executeSearchForAccountMenu(int* accountStage)
{
	int searchStage = 0;
	while (*accountStage) {
		switch (searchStage) {
		case (0):
			showSearchForAccountMenu();
			searchStage = chooseOption(7);
			break;
		case (1):
			searchInFile(&searchStage, searchStage, "SEARCH BY AN ID");
			break;
		case (2):
			searchInFile(&searchStage, searchStage,
						 "SEARCH BY AN ACCOUNT NUMBER");
			break;
		case (3):
			searchInFile(&searchStage, searchStage, "SEARCH BY A NAME");
			break;
		case (4):
			searchInFile(&searchStage, searchStage, "SEARCH BY A SURNAME");
			break;
		case (5):
			searchInFile(&searchStage, searchStage, "SEARCH BY AN ADRESS");
			break;
		case (6):
			searchInFile(&searchStage, searchStage, "SEARCH BY A PESEL");
			break;
		case (7):
			*accountStage = 0;
			break;
		}
	}
}

void executeAccoutMenu(int* stage)
{
	int accountStage = 0;
	while (*stage) {
		switch (accountStage) {
		case (0):
			showAccountMenu();
			accountStage = chooseOption(3);
			break;
		case (1):
			showAllAccounts(&accountStage);
			break;
		case (2):
			executeSearchForAccountMenu(&accountStage);
			break;
		case (3):
			*stage = 0;
			break;
		}
	}
}

void showOperationsMenu()
{
	char* title = "OPERATIONS MENU";
	showHeader(title);
	printf("1.Create account\n"
			"2.Edit account\n"
		   "3.Make a deposit\n"
		   "4.Make a withdrawal\n"
		   "5.Take a loan\n"
		   "6.Pay a debt\n"
		   "7.Make a money transfer\n"
		   "8.Go back to main menu\n\n"
		   "choose option\n");
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

void generateAccountNumber(account_t* account)
{
	char accountNumber[ACCOUNT_NUMBER_LENGHT] = "PL----------\0";
	char letter;
	int ranNum;
	srand(time(NULL));
	FILE* file = fopen("accounts.dat", "rb");
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

void createAccount(int* operationStage)
{
	char* title = "CREATE ACCOUNT";
	account_t account;
	long long id = getNextId();
	FILE* file = fopen("accounts.dat", "ab+");
	if (file && id) {
		gatherDataForAccount(title, &account, id);
		if (!confirmeOperation(title, &operationStage, account))
			return;
		saveToFile(file, account);
	}
	if (!file)
		waitForQToBackToMenu(title, "File not found\n");
	else
		waitForQToBackToMenu(title, "");
	endOperation(file, operationStage);
}

void getAccountNumber(char accountNumber[ACCOUNT_NUMBER_LENGHT], char* title,
					  char* operationName)
{

	showHeader(title);
	printf("Enter account number you want use to %s\n", operationName);
	scanf("%s", accountNumber);
	accountNumber[ACCOUNT_NUMBER_LENGHT - 1] = '\0';
}

void updateFile(FILE* file, account_t account)
{
	fseek(file, (account.id - 1) * sizeof(account), SEEK_SET);
	saveToFile(file, account);
}

void editAccount(int* operationStage)
{
	char* title = "EDIT AN ACCOUNT";
	char accountNumber[ACCOUNT_NUMBER_LENGHT];
	account_t account;
	getAccountNumber(accountNumber, title, "edit an account");
	while(getchar() != '\n');
	FILE* file = fopen("accounts.dat", "rb+");
	if(file && getSpecificClientData(file, (char*)accountNumber, &account, LOOKING_FOR_ACCOUNT_NUMBER))
	{
		gatherPersonalData(title, &account);
		if (!confirmeOperation(title, &operationStage, account))
			return;
		updateFile(file, account);
		waitForQToBackToMenu(title, "");
		endOperation(file, operationStage);
		return;
	}
	operationDidNotSucceed(file, title);
	endOperation(file, operationStage);
}

void getAmountOfMoney(double* moneyAmount, char* title, char* operationVerb)
{
	int check = 0;
	do
	{
		showHeader(title);
		printf("How much do you want to %s?\n", operationVerb);
		check = scanf("%lf", moneyAmount);
		while(getchar() != '\n');
	}while(!check);
}

void getNumberOfInstallments(int* numberOfInstallments, char* title)
{
	int check = 0;
	do
	{
		showHeader(title);
		printf("How many installments do you want to pay?\n");
		check = scanf("%d", numberOfInstallments);
		while(getchar() != '\n');
	}while(!check);
}

void makeDeposit(account_t* account, double* addToBalance)
{

	account->balance = (account->balance > MAX_MONEY_AMOUNT - *addToBalance)
						   ? MAX_MONEY_AMOUNT
						   : account->balance + *addToBalance;
}

void makeWithdrawal(account_t* account, double* substractFromBalance)
{
	account->balance = (account->balance < *substractFromBalance)
						   ? 0
						   : account->balance - *substractFromBalance;
}

void takeLoan(account_t* account, double* loan)
{
	if (account->balance > MAX_MONEY_AMOUNT - *loan ||
		account->loan > MAX_MONEY_AMOUNT - (*loan + *loan * ADDITIONAL_INSTALLMENTS * INTEREST))
		*loan = (account->balance > account->loan + *loan * ADDITIONAL_INSTALLMENTS * INTEREST)
					? MAX_MONEY_AMOUNT - account->balance
					: MAX_MONEY_AMOUNT - (account->loan + *loan * ADDITIONAL_INSTALLMENTS * INTEREST);
	account->balance += *loan;
	account->installment += *loan * INTEREST;
	account->loan += *loan + *loan * ADDITIONAL_INSTALLMENTS * INTEREST;
}

void payDebt(account_t* account, int numberOfInstallments)
{
	for(int i = 0; i < numberOfInstallments; i++)
	{
		if (account->balance > account->installment) {
			account->balance -= account->installment;
			account->loan -= account->installment;
		}
		if (account->loan < account->installment)
			account->installment = account->loan;
	}
}

void transferMoney(account_t* account1, account_t* account2,
				   double* moneyToTransfer)
{
	if (account1->balance < *moneyToTransfer ||
		account2->balance > MAX_MONEY_AMOUNT - *moneyToTransfer) {
		*moneyToTransfer =
			(account1->balance < MAX_MONEY_AMOUNT - account2->balance)
				? account1->balance
				: MAX_MONEY_AMOUNT - account2->balance;
	}
	account1->balance -= *moneyToTransfer;
	account2->balance += *moneyToTransfer;
}

void makeOperation(int* operationStage, char* title,
				   void (*operation)(account_t*, double*))
{
	char accountNumber[ACCOUNT_NUMBER_LENGHT];
	char operationName[strlen(title)];
	char lastPartOfSentence[strlen(title)];
	changeUpperIntoLower(title, operationName);
	getLastPartOfSentence(operationName, lastPartOfSentence, 1);
	double chngeOfBalance;
	account_t account;
	getAccountNumber(accountNumber, title, operationName);
	FILE* file = fopen("accounts.dat", "rb+");
	if (file && getSpecificClientData(file, (char*)accountNumber, &account, LOOKING_FOR_ACCOUNT_NUMBER)) {
		getAmountOfMoney(&chngeOfBalance, title, lastPartOfSentence);
		operation(&account, &chngeOfBalance);
		if (!confirmeOperation(title, &operationStage, account))
			return;
		updateFile(file, account);
		waitForQToBackToMenu(title, "");
		endOperation(file, operationStage);
		return;
	}
	operationDidNotSucceed(file, title);
	endOperation(file, operationStage);
}

void payDebtOperation(int* operationStage)
{
	char* title = "PAY A DEBT";
	char accountNumber[ACCOUNT_NUMBER_LENGHT];
	int numberOfInstallments = 0;
	account_t account;
	getAccountNumber(accountNumber, title, "pay a debt");
	FILE* file = fopen("accounts.dat", "rb+");
	if (file && getSpecificClientData(file, (char*)accountNumber, &account, LOOKING_FOR_ACCOUNT_NUMBER)) {
		getNumberOfInstallments(&numberOfInstallments, title);
		payDebt(&account, numberOfInstallments);
		if (!confirmeOperation(title, &operationStage, account))
			return;
		updateFile(file, account);
		waitForQToBackToMenu(title, "");
		endOperation(file, operationStage);
		return;
	}
	operationDidNotSucceed(file, title);
	endOperation(file, operationStage);
}

void transferMoneyOperation(int* operationStage)
{
	char* title = "MAKE A MONEY TRANSFER";
	char accountNumber[ACCOUNT_NUMBER_LENGHT];
	long long id1 = 0;
	long long id2 = 0;
	double moneyToTransfer;
	account_t account1;
	account_t account2;
	FILE* file = fopen("accounts.dat", "rb+");
	if(file)
	{
		getAccountNumber(accountNumber, title, "transfer money from");
		if (getSpecificClientData(file, (char*)accountNumber, &account1, LOOKING_FOR_ACCOUNT_NUMBER))
			id1 = account1.id;
		rewind(file);
		getAccountNumber(accountNumber, title, "transfer money to");
		if (getSpecificClientData(file, (char*)accountNumber, &account2, LOOKING_FOR_ACCOUNT_NUMBER))
			id2 = account2.id;
		if (id1 != 0 && id2 != 0) {
			getAmountOfMoney(&moneyToTransfer, title, "transfer");
			transferMoney(&account1, &account2, &moneyToTransfer);
			if (!confirmeOperation(title, &operationStage, account1))
				return;
			if (!confirmeOperation(title, &operationStage, account2))
				return;
			updateFile(file, account1);
			updateFile(file, account2);
			waitForQToBackToMenu(title, "");
			endOperation(file, operationStage);
			return;
		}
	}
	operationDidNotSucceed(file, title);
	endOperation(file, operationStage);
}

void executeOperationsMenu(int* stage)
{
	int operationStage = 0;
	while (*stage) {
		switch (operationStage) {
		case (0):
			showOperationsMenu();
			operationStage = chooseOption(8);
			break;
		case (1):
			createAccount(&operationStage);
			break;
		case (2):
			editAccount(&operationStage);
			break;
		case (3):
			makeOperation(&operationStage, "MAKE A DEPOSIT", makeDeposit);
			break;
		case (4):
			makeOperation(&operationStage, "MAKE A WITHDRAWAL", makeWithdrawal);
			break;
		case (5):
			makeOperation(&operationStage, "MAKE A LOAN", takeLoan);
			break;
		case (6):
			payDebtOperation(&operationStage);
			break;
		case (7):
			transferMoneyOperation(&operationStage);
			break;
		case (8):
			*stage = 0;
			break;
		}
	}
}

void showInfo(int* stage)
{
	char* title = "INFO";
	showHeader(title);
	printf("Banking system created for Programing and Data Structures in "
		   "C.\nTo show all accounts or search for particular account choose "
		   "'Show accounts' \nby pressing 2 in menu options.\nTo make "
		   "operations on one or many accounts choose 'Accounts operations' "
		   "\nby pressing 3 in menu options to get possible operations.\n\n");
	printf("press enter to go back to menu\n");
	if (getchar() == '\n')
		*stage = 0;
}

void executeMainMenu()
{
	int stage = 0;
	int execute = 1;
	while (execute) {
		switch (stage) {
		case (0):
			showMenu();
			stage = chooseOption(4);
			break;
		case (1):
			executeAccoutMenu(&stage);
			break;
		case (2):
			executeOperationsMenu(&stage);
			break;
		case (3):
			showInfo(&stage);
			break;
		case (4):
			system("clear");
			execute = 0;
		}
	}
}

int main()
{
	FILE* file = fopen("accounts.dat", "ab");
	if(!file) 
	{
		printf("File did NOT open\n");
		return (3);
	}
	fclose(file);
	executeMainMenu();
	return (0);
}
