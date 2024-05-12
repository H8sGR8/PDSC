#ifndef __ACCOUNT_OPERATIONS_H__
#define __ACCOUNT_OPERATIONS_H__

#include "init.h"
#include "basic_functions.h"
#include "interface.h"
#include "gather_data.h"

void createAccount(int* operationStage);

void getAccountNumber(char* , char*, char*);

void editAccount(int*);

void getAmountOfMoney(double*, char*, char*);

void getNumberOfInstallments(int*, char*);

void makeDeposit(account_t*, double*);

void makeWithdrawal(account_t*, double*);

void takeLoan(account_t*, double*);

void payDebt(account_t*, int);

void transferMoney(account_t*, account_t*, double*);

void makeOperation(int*, char*, void (account_t*, double*));

void payDebtOperation(int*);

void transferMoneyOperation(int*);

#endif /*__ACCOUNT_OPERATIONS_H__*/
