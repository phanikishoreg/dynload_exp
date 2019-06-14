#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "a.h"

#define MAX_DL 4
#define MAX_FNS 4

typedef int (*app_main_fn_t)(void);

int main(int argc, char **argv)
{
	void *apps[MAX_DL] = { NULL };
	app_main_fn_t appmains[MAX_DL * MAX_FNS] = { NULL };
	int i;

	assert(argc >= 1 && argc <= MAX_DL);

	for (i = 0; i < argc - 1; i++) {
		int j;

#ifdef USE_NS
		apps[i] = dlmopen(LM_ID_NEWLM, argv[i+1], RTLD_LAZY);
#else
		apps[i] = dlopen(argv[i+1], RTLD_LAZY);
#endif
		assert(apps[i]);

		appmains[(i * MAX_FNS)] = (app_main_fn_t)dlsym(apps[i], "main");
		assert(appmains[(i * MAX_FNS)]);
		appmains[(i * MAX_FNS) + 1] = (app_main_fn_t)dlsym(apps[i], "hello_main");
		assert(appmains[(i * MAX_FNS) + 1]);
	}

	for (i = 0; i < (argc - 1) * MAX_FNS; i++) {
		if (!appmains[i]) continue;

		appmains[i]();
	}

	printf("%p %p %p\n", a, b, c);
	a(3);
	b(3);
	c(3);

	for (i = 0; i < (argc - 1); i++) dlclose(apps[i]);

	return 0;
}
