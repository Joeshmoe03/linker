/* self-linker.c */
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <gnu/lib-names.h>

int main(int argc, char *argv[]) {
	//extern void *_GLOBAL_OFFSET_TABLE_;
	void *handle;
	void *putslibcaddr;
	void *putsgotplt;

	/* Code modifying GOT table for puts, before it's actually caled */
	handle = dlopen(LIBC_SO, RTLD_LAZY);
	putslibcaddr = dlsym(handle, "puts");

	/* Hardcode/magic number explanation: blah blah blah  
	 # 404000 <puts@GLIBC_2.2.5> running objdump -j .plt -d self-linker, 
	 same as # 0x404000 <puts@got.plt> from gdb using breakpoints and disassembling */
	putsgotplt = (void *)0x404000;
	//void *toChange = (void*)((__intptr_t)(_GLOBAL_OFFSET_TABLE_) + 0x1f8); //yep got us to the right place, whatever is saved at 
	*(void **)putsgotplt = putslibcaddr;

	/* puts call */
	puts("helloworld");
}

