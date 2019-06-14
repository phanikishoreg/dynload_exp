#include <stdio.h>
#include "a.h"

int hello_main(void)
{
	printf("Hello!\n");

	return 0;
}

int main(void)
{
	printf("Hello %p %p %p!\n", a, b, c);
	a(1);
	b(1);
	c(1);

	return hello_main();
}


