#ifndef __GATHER_DATA_H__
#define __GATHER_DATA_H__

#include "init.h"
#include "basic_functions.h"
#include "interface.h"

void generateAccountNumber(account_t*);

void getName(char*, account_t*);

void getSurname(char*, account_t*);

void getAdress(char*, account_t*);

void getPesel(char*, account_t*);

void getAccountBalance(char*, account_t*);

void getAccountLoan(char*, account_t*);

void gatherPersonalData(char*, account_t*);

void gatherDataForAccount(char*, account_t*, long long);

#endif /*__GATHER_DATA_H__*/
