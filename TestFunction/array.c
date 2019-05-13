#include "pch.h"
#include <stdio.h>
char * f1(void)
{
	char a[3] = { 'a','b','c' };
	return &a;
}

char f2(char *a)
{
	for (int i = 0; i < 3; i++)
	{
		printf("%c", *(a + i));
	}
	return "a";
}

int main(void)
{
	f2(f1());
	return 0;
}
