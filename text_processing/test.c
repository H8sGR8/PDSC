//#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeAll(char* line, char** text, size_t* linesNumber)
{
	int i;
	free(line);
	for (i = *linesNumber; i > 0; i--)
		free(text[i - 1]);
	free(text);
	abort();
}

char* increaseBuforForLine(size_t size, size_t* neededSize, char* line)
{
	char* newbuf;
	if (*neededSize == 0)
		*neededSize = 1;
	else if (*neededSize <= ((size_t)-1) / 2)
		*neededSize = 2 * size;
	else {
		free(line);
		return NULL;
	}
	newbuf = realloc(line, *neededSize);
	if (!newbuf) {
		free(line);
		return NULL;
	}
	return newbuf;
}

char* readLine()
{
	char* line = NULL;
	char c;
	size_t neededSize = 0;
	size_t size = 0;
	while ((c = getchar()) != EOF) {
		if (size >= neededSize &&
			!(line = increaseBuforForLine(size, &neededSize, line)))
			return NULL;
		if (c == '\n')
			break;
		line[size++] = c;
	}
	if ((c == EOF) && (size == 0)) {
		free(line);
		return 0;
	}
	line[size] = '\0';
	return line;
}

char** increaseBuforForText(size_t linesNumber, size_t* linesNeededNumber, char* line,
							char** text)
{
	char** newlines;
	if (linesNumber == 0)
		*linesNeededNumber = 1;
	else if (linesNumber <= ((size_t)-1) / 2 / sizeof(char*)) {
		*linesNeededNumber = 2 * linesNumber;
	}
	else {
		freeAll(line, text, &linesNumber);
	}
	newlines = realloc(text, *linesNeededNumber * sizeof(char*));
	if (!newlines) {
		freeAll(line, text, &linesNumber);
	}
	return newlines;
}

void printReverse(char str[])
{
	int length = strlen(str);

	int i;
	for (i = length - 1; i >= 0; i--) {
		if (str[i] == ' ') {
			str[i] = '\0';
			printf("%s ", &(str[i]) + 1);
		}
	}
	printf("%s\n", str);
}

void printFinalText(size_t linesNumber, char* line, char** text)
{
	int i;
	free(line);
	for (i = linesNumber; i > 0; i--) {
		printReverse(text[i - 1]);
		free(text[i - 1]);
	}
	free(text);
}

int main()
{
	char** text = NULL;
	size_t linesNumber = 0;
	size_t linesNeededNumber = 0;
	char* line;
	while ((line = readLine())) {
		if (linesNumber >= linesNeededNumber)
			text = increaseBuforForText(linesNumber, &linesNeededNumber, line, text);
		text[linesNumber++] = line;
	}
	printFinalText(linesNumber, line, text);
	return 0;
}
