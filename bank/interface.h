#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <stdbool.h>

#include "init.h"
#include "basic_functions.h"

void showHeader(char*);

void showMenu();

void showAccountMenu();

void showSearchForAccountMenu();

void showOperationsMenu();

void showInfo(int*);

void showLine(accountLen_t);

void showHeaderForTable(accountLen_t);

void showAccount(account_t, accountLen_t);

void waitForQToBackToMenu(char*, char*);

void showIfOperationDidNotSucceed(FILE*, char*);

void showIfOperationSucceeded(accountLen_t);

bool askForConfirmation(int***);

bool confirmeOperation(char*, int**, account_t);

bool confirmeOperationForTransfer(char*, int**, account_t, account_t);

void showTable(FILE*, accountLen_t*, void*, int, int*, bool (FILE*, char*, account_t*, int));

void showAllAccounts(int*);

void showSpecificAccounts(int*, char*);

#endif /*__INTERFACE_H__*/
