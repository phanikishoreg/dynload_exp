#include <stdio.h>
#include "a.h"

#define MEM_SZ 8192

extern int whatever(int a);
extern void more_mem(void);

extern void *base;
extern unsigned bound;

static inline void
incr_bound(void)
{
	bound++;
}

static inline void
check_bound(void)
{
	static int x = 'a';

	if (base) { 
		x++;
		incr_bound();
		if (x % 2) more_mem();
	}
}

int hello_main(void)
{
	printf("%p:%p %p:%u\n", &base, base, &bound, bound);

	printf("Hello!\n");
	check_bound();
	printf("Hello!\n");
	check_bound();
	printf("Hello!\n");
	check_bound();
	printf("Hello!\n");
	check_bound();
	printf("Hello!\n");
	check_bound();
	printf("Hello!\n");
	check_bound();
	printf("Hello!\n");
	check_bound();

	return 0;
}

int main(void)
{
	printf("Hello %p %p %p!\n", a, b, c);
	a(1);
	b(1);
	c(1);
	whatever(1);

	return hello_main();
}


