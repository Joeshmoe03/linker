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
	int symgotpltval;
	void *symgotpltaddr;

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

	/* Hardcode/magic number explanation: We obtained the PLT entry for puts using the following commands: 
	 # 404000 <puts@GLIBC_2.2.5> running objdump -j .plt -d self-linker, 
	 same as # 0x404000 <puts@got.plt> from gdb using breakpoints and disassembling 
	 This is the same result of as doing symgotpltaddr = (void *)0x404000;
	 */
	symgotpltval = 0x404000;
	symgotpltaddr = (void*)((__intptr_t)(GOTtable) + (symgotpltval - (__intptr_t)(GOTtable)));
	*(void **)symgotpltaddr = symaddress;
	puts("Hello world!");
	puts("Second message");
	return 0;
}
