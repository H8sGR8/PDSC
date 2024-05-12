#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "account_operations.h"

void createAccount(int* operationStage)
{
	char* title = "CREATE ACCOUNT";
	account_t account;
	long long id = getNextId();
	FILE* file = fopen(ACCOUNTS_FILE, "rb+");
	if (file && id) {
		gatherDataForAccount(title, &account, id);
		if (!confirmeOperation(title, &operationStage, account))
			return;
		fseek(file, 0, SEEK_END);
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

void editAccount(int* operationStage)
{
	char* title = "EDIT AN ACCOUNT";
	char accountNumber[ACCOUNT_NUMBER_LENGHT];
	account_t account;
	getAccountNumber(accountNumber, title, "edit an account");
	while(getchar() != '\n');
	FILE* file = fopen(ACCOUNTS_FILE, "rb+");
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
	showIfOperationDidNotSucceed(file, title);
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
	FILE* file = fopen(ACCOUNTS_FILE, "rb+");
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
	showIfOperationDidNotSucceed(file, title);
	endOperation(file, operationStage);
}

void payDebtOperation(int* operationStage)
{
	char* title = "PAY A DEBT";
	char accountNumber[ACCOUNT_NUMBER_LENGHT];
	int numberOfInstallments = 0;
	account_t account;
	getAccountNumber(accountNumber, title, "pay a debt");
	FILE* file = fopen(ACCOUNTS_FILE, "rb+");
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
	showIfOperationDidNotSucceed(file, title);
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
	FILE* file = fopen(ACCOUNTS_FILE, "rb+");
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
	showIfOperationDidNotSucceed(file, title);
	endOperation(file, operationStage);
}
