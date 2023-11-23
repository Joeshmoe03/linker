/* self-linker.c */
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <gnu/lib-names.h>


int main(int argc, char *argv[]) {
	extern void *_GLOBAL_OFFSET_TABLE_;
	int (*putsreturn)(char *);//function prototype for puts

	/* code modifying GOT table for puts, before it's actually called */

	void *handle;

	handle = dlopen(LIBM_SO, RTLD_LAZY);

	// putsreturn = (int (*)(int)) dlsym(handle, "puts");
	*(void **) (&putsreturn) = dlsym(handle, "puts");

	printf("%p\n",handle);
	printf("%p\n",_GLOBAL_OFFSET_TABLE_);
	printf("%d\n", (*putsreturn)("helloworld"));

	if(!handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

}

