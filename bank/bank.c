#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interface.h"
#include "gather_data.h"
#include "account_operations.h"

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
			showSpecificAccounts(&searchStage, "SEARCH BY AN ID");
			break;
		case (2):
			showSpecificAccounts(&searchStage,"SEARCH BY AN ACCOUNT NUMBER");
			break;
		case (3):
			showSpecificAccounts(&searchStage, "SEARCH BY A NAME");
			break;
		case (4):
			showSpecificAccounts(&searchStage, "SEARCH BY A SURNAME");
			break;
		case (5):
			showSpecificAccounts(&searchStage, "SEARCH BY AN ADRESS");
			break;
		case (6):
			showSpecificAccounts(&searchStage, "SEARCH BY A PESEL");
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
	FILE* file = fopen(ACCOUNTS_FILE, "ab");
	if(!file) 
	{
		printf("File did NOT open\n");
		return (3);
	}
	fclose(file);
	executeMainMenu();
	return (0);
}
