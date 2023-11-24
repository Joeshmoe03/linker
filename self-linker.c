#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <gnu/lib-names.h>

extern void *_GLOBAL_OFFSET_TABLE_;

int main(int argc, char *argv[]) {

	/* This is where the GOT table starts, our handle, and lib + our future got_entry (for now a placeholder
	* and our symbol to resolve */
	void** GOTtable = (void**)&_GLOBAL_OFFSET_TABLE_;
	void *handle;
	char* lib = LIBC_SO;
	char* sym = "puts";
	void* GOTentry;

	/* If dlopen fails, exit with error, else continue */
	handle = dlopen(lib, RTLD_LAZY);
	if(!handle) {
		perror("dlopen");
		exit(1);
	}

	/* We pass our handle to dlsym to resolve symbol of puts as an address */
	GOTentry = dlsym(handle, sym);
	if(GOTentry == NULL) {
		perror("dlsym");
		exit(1);
	}

	puts("Hello world!");
	puts("Second message!");
	return 0;
}
