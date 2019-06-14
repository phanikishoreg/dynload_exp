#include <stdio.h>
#include "a.h"

extern int whatever(int);

int hello_main(void)
{
	printf("World!\n");

	return 0;
}

int main(void)
{
	printf("World %p %p %p!\n", a, b, c);
	a(2);
	b(2);
	c(2);
	whatever(2);

	return hello_main();
}


