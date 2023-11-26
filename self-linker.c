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
	void* symaddress;
	
	/* If dlopen fails, exit with error, else continue */
	handle = dlopen(lib, RTLD_LAZY);
	if(!handle) {
		perror("dlopen");
		exit(1);
	}

	/* We pass our handle to dlsym to resolve symbol of puts as an address */
	symaddress = dlsym(handle, sym);
	if(symaddress == NULL) {
		perror("dlsym");
		exit(1);
	}

	/* Hardcode explanation: Running gdb self-linker, if do "print (&_GLOBAL_OFFSET_TABLE)[3]", we can clearly see (void *) 0x... <puts>,
 	*  signifying that this is the corresponding GOT entry for puts. What I can then do is manually change the address at that entry such
 	*  that the symbol address I previously obtained from dlsym is now shoved there before the actual runtime linker does it. */	
	GOTtable[3] = symaddress;

	/* If I run "$ make", then "$ gdb self-linker", and "break 39" and run, before even calling puts, if I "print GOTtable[3]", 
 	*  I should see the libc address of puts, meaning I successfully put it in the GOT manually */
	puts("Hello world!");
	puts("Hello again!");
	dlclose(handle);
	return 0;
}
