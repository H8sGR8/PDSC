#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interface.h"

void showHeader(char* header)
{
	system("clear");
	printf("%s\n\n", header);
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

void showAccountMenu()
{
	char* title = "ACCOUNTS MENU";
	showHeader(title);
	printf("1.Show all accounts\n"
		   "2.Search for an account\n"
		   "3.Go back to main menu\n\n"
		   "choose option\n");
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

void waitForQToBackToMenu(char* title, char* message)
{
	while (1) {
		showHeader(title);
		printf("%spress 'q' to go back to accounts menu\n", message);
		if (getchar() == 'q')
			break;
	}
}

void showIfOperationDidNotSucceed(FILE* file, char* title)
{
	if (!file)
		waitForQToBackToMenu(title, "File not found\n");
	else
		waitForQToBackToMenu(title, "Record not found\n");
}

void showIfOperationSucceeded(accountLen_t accountLen)
{
	showLine(accountLen);
	printf("\npress 'q' to go back to search account menu\n");
	while (getchar() != 'q');
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

void showAllAccounts(int* accountStage)
{
	char* title = "ALL ACCOUNTS";
	int printedAccounts = 0;
	accountLen_t accountLen = DEFAULT_ACCOUNT_LEN;
	FILE* file = fopen(ACCOUNTS_FILE, "rb");
	if(file)
	{
		showHeader(title);
		showTable(file, &accountLen, NULL, 0, &printedAccounts, getClientData);
	}
	if(printedAccounts) showIfOperationSucceeded(accountLen);
	else showIfOperationDidNotSucceed(file, title);
	endOperation(file, accountStage);
}

void showSpecificAccounts(int* searchStage, char* title)
{
	char input[STRING_MAX_LENGHT];
	char lastPartOfSentence[strlen(title)];
	int printedAccounts = 0;
	changeUpperIntoLower(title, lastPartOfSentence);
	if (*searchStage == LOOKING_FOR_ACCOUNT_NUMBER)
		getLastPartOfSentence(lastPartOfSentence, lastPartOfSentence, WORDS_FOR_ACCOUNT);
	else
		getLastPartOfSentence(lastPartOfSentence, lastPartOfSentence, WORDS_FOR_OTHER_DATA);
	accountLen_t accountLen = DEFAULT_ACCOUNT_LEN;
	showHeader(title);
	printf("Enter %s\n", lastPartOfSentence);
	fgets(input, STRING_MAX_LENGHT, stdin);
	input[strlen(input) - 1] = '\0';
	FILE* file = fopen(ACCOUNTS_FILE, "rb");
	if(file)
	{
		showHeader(title);
		showTable(file, &accountLen, (char*)input, *searchStage, &printedAccounts, getSpecificClientData);
	}
	if(printedAccounts) showIfOperationSucceeded(accountLen);
	else showIfOperationDidNotSucceed(file, title);
	endOperation(file, searchStage);
}
