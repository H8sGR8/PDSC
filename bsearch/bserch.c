#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
    int struct_key;
    char *struct_value;
} struct_t;

int compareStruct(const void *num1, const void *num2)
{
    struct_t *key = (struct_t *)num1;
    struct_t *middle = (struct_t *)num2;
    return ((key->struct_key > middle->struct_key) - (key->struct_key < middle->struct_key));
}

int compareDouble(const void *num1, const void *num2)
{
    double *key = (double *)num1;
    double *middle = (double *)num2;
    return ((*key > *middle) - (*key < *middle));
}

int compareInt(const void *num1, const void *num2)
{
    int *key = (int *)num1;
    int *middle = (int *)num2;
    return ((*key > *middle) - (*key < *middle));
}

int compareChar(const void *num1, const void *num2)
{
    char *key = (char *)num1;
    char *middle = (char *)num2;
    return ((*key > *middle) - (*key < *middle));
}

void* bserch(const void* key, const void* array, size_t count, size_t size, int (*compare)(const void*, const void*))
{
	size_t middle = count / 2 - (count%2 == 0);
	int status = compare(key, (void*)((char*)array + middle * size));
	if((count == 1 && status != 0) || count < 1) return NULL;
	switch(status)
	{
		case(0):
			return (void*)((char*)array + middle  * size);
		case(1):
			return bserch(key, (void*)((char*)array + (middle + 1) * size), count - (middle + 1), size, compare);
		case(-1):
			return bserch(key, array, middle, size, compare);
	}
	return NULL;
}
	
void testInt()
{
	int table[] = {0,1,2,3,4,5,6,7,8,9};
	for(int i = -1; i <= 10; i++)
	{
		if(bserch(&i, &table, sizeof(table)/sizeof(table[0]), sizeof(table[0]), compareInt) != NULL){
			int*a = bserch(&i, &table, sizeof(table)/sizeof(table[0]), sizeof(table[0]), compareInt);
			printf("%d  ", *a);
		}else printf("NULL  ");
	}
	printf("\n");
}

void testChar()
{
	char table[] = {'a','b','c','d','e','f','g','h','i','j'};
	for(int i = -1; i <= 10; i++)
	{
		char j = i + 'a';
		if(bserch(&j, &table, sizeof(table)/sizeof(table[0]), sizeof(table[0]), compareChar) != NULL){
			char*a = bserch(&j, &table, sizeof(table)/sizeof(table[0]), sizeof(table[0]), compareChar);
			printf("%c  ", *a);
		}else printf("NULL  ");
	}
	printf("\n");
}

void testDouble()
{
	double table[] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9};
	for(int i = -1; i <= 10; i++)
	{
		double j = 1.0 + (double)i/10;
		if(bserch(&j, &table, sizeof(table)/sizeof(table[0]), sizeof(table[0]), compareDouble) != NULL){
			double*a = bserch(&j, &table, sizeof(table)/sizeof(table[0]), sizeof(table[0]), compareDouble);
			printf("%.1f  ", *a);
		}else printf("NULL  ");
	}
	printf("\n");
}

void testStruct()
{
	struct_t table[] = {{0, "A"}, {1, "B"},   {2, "C"},  {3, "D"}, {4, "E"}, {5, "F"},   {6, "G"}, {7, "H"}, {8, "I"},  {9, "J"}};
	for(int i = -1; i <= 10; i++)
	{
		if(bserch(&i, &table, sizeof(table)/sizeof(table[0]), sizeof(table[0]), compareStruct) != NULL){
			int*a = bserch(&i, &table, sizeof(table)/sizeof(table[0]), sizeof(table[0]), compareStruct);
			printf("%d(%s)  ", *a, table[*a].struct_value);
		}else printf("NULL  ");
	}
	printf("\n");
}
	
int main()
{
	testInt();
	testChar();
	testDouble();
	testStruct();
	return 0;
}
