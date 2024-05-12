#ifndef __BASIC_FUNCTIONS_H__
#define __BASIC_FUNCTIONS_H__

#include <stdbool.h>

#include "init.h"

bool isInFile(int, account_t*, char*);

bool getClientData(FILE*, char*, account_t*, int);

bool getSpecificClientData(FILE*, char*, account_t*, int);

void saveToFile(FILE*, account_t);

void updateFile(FILE*, account_t);

long long getNextId();

int checkNumberOfDigitsBeforePoint(long long, double);

void setTableWidth(account_t, accountLen_t*);

void endOperation(FILE* file, int* stage);

void changeUpperIntoLower(char*, char*);

void getLastPartOfSentence(char*, char*, int);
						   
int chooseOption(int);

bool isNumber(char*);

bool isCorrectString(char*);

#endif /*__BASIC_FUNCTIONS_H__*/
