all: bank

bank: bank.o basic_functions.o interface.o gather_data.o account_operations.o
	gcc -fsanitize=undefined -g $^ -o $@ -lm

.c.o:
	gcc -fsanitize=undefined -g -Wall -pedantic -c $<
	
bank.o: bank.c

basic_functions.o: basic_functions.c basic_functions.h

interface.o: interface.c interface.h

gather_data.o: gather_data.c gather_data.h

account_operations.o: account_operations.c account_operations.h

.PHONY: clean
clean:
	rm bank *.o
