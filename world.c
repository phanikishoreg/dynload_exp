#include <stdio.h>
#include "a.h"

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

	return hello_main();
}


