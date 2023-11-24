# Authors: Hedavam Solano and Josef Liem
# Known Bugs: None that we could identify

# Annotated script proving self-linker's correctness:

[hsolano@weathertop assignment4]$ make
gcc -Wall -g -pedantic -o self-linker self-linker.c

## Works on shell
[hsolano@weathertop assignment4]$ ./self-linker
Hello world! /* desired output */

## Works on gdb
[hsolano@weathertop assignment4]$ gdb self-linker
GNU gdb (GDB) Fedora Linux 13.2-6.fc38
Copyright (C) 2023 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-redhat-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from self-linker...
(gdb) r
Starting program: /home/hsolano/cs315/assignment4/self-linker 

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.fedoraproject.org/>
Enable debuginfod for this session? (y or [n]) n
Debuginfod has been disabled.
To make this setting permanent, add 'set debuginfod enabled off' to .gdbinit.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".
Hello world! /* desired output */
[Inferior 1 (process 4168831) exited normally]
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.37-13.fc38.x86_64
(gdb) exit

## Using gdb to test program's correctness:
We place a breakpoint before our program's puts() call and run until we hit it.
We then disassemble (disas) and execute (si) instructions until we reach the puts@plt () call.
Once reached, we disassemble (disas) the puts@plt () call, and grab the entry within the GOT that pertains to puts()
Under "normal" circumstances, this would point to the second instruction of the puts PLT entry since the GOT entry before puts() is initially called is virtually empty.
But, since we implememnted our self-linker, we can see using x/gx command that our GOT entry pertaning to puts() points somewhere else.
After stepping through one more instruction, we can see that this value is indeed the "real" address of puts() in libc.


[hsolano@weathertop assignment4]$ gdb self-linker
GNU gdb (GDB) Fedora Linux 13.2-6.fc38
Copyright (C) 2023 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-redhat-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from self-linker...
(gdb) break 42
Breakpoint 1 at 0x40120a: file self-linker.c, line 42.
(gdb) run
Starting program: /home/hsolano/cs315/assignment4/self-linker 

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.fedoraproject.org/>
Enable debuginfod for this session? (y or [n]) n
Debuginfod has been disabled.
To make this setting permanent, add 'set debuginfod enabled off' to .gdbinit.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".

Breakpoint 1, main (argc=1, argv=0x7fffffffe0d8) at self-linker.c:42
42		puts("Hello world!");
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.37-13.fc38.x86_64
(gdb) disas
Dump of assembler code for function main:
   0x0000000000401166 <+0>:	push   %rbp
   0x0000000000401167 <+1>:	mov    %rsp,%rbp
   0x000000000040116a <+4>:	sub    $0x50,%rsp
   0x000000000040116e <+8>:	mov    %edi,-0x44(%rbp)
   0x0000000000401171 <+11>:	mov    %rsi,-0x50(%rbp)
   0x0000000000401175 <+15>:	movq   $0x2e73,-0x8(%rbp)
   0x000000000040117d <+23>:	movq   $0x402010,-0x10(%rbp)
   0x0000000000401185 <+31>:	movq   $0x40201a,-0x18(%rbp)
   0x000000000040118d <+39>:	mov    -0x10(%rbp),%rax
   0x0000000000401191 <+43>:	mov    $0x1,%esi
   0x0000000000401196 <+48>:	mov    %rax,%rdi
   0x0000000000401199 <+51>:	call   0x401040 <dlopen@plt>
   0x000000000040119e <+56>:	mov    %rax,-0x20(%rbp)
   0x00000000004011a2 <+60>:	cmpq   $0x0,-0x20(%rbp)
   0x00000000004011a7 <+65>:	jne    0x4011bd <main+87>
   0x00000000004011a9 <+67>:	mov    $0x40201f,%edi
   0x00000000004011ae <+72>:	call   0x401050 <perror@plt>
   0x00000000004011b3 <+77>:	mov    $0x1,%edi
   0x00000000004011b8 <+82>:	call   0x401070 <exit@plt>
   0x00000000004011bd <+87>:	mov    -0x18(%rbp),%rdx
   0x00000000004011c1 <+91>:	mov    -0x20(%rbp),%rax
   0x00000000004011c5 <+95>:	mov    %rdx,%rsi
   0x00000000004011c8 <+98>:	mov    %rax,%rdi
   0x00000000004011cb <+101>:	call   0x401060 <dlsym@plt>
   0x00000000004011d0 <+106>:	mov    %rax,-0x28(%rbp)
   0x00000000004011d4 <+110>:	cmpq   $0x0,-0x28(%rbp)
   0x00000000004011d9 <+115>:	jne    0x4011ef <main+137>
   0x00000000004011db <+117>:	mov    $0x402026,%edi
   0x00000000004011e0 <+122>:	call   0x401050 <perror@plt>
   0x00000000004011e5 <+127>:	mov    $0x1,%edi
   0x00000000004011ea <+132>:	call   0x401070 <exit@plt>
   0x00000000004011ef <+137>:	movl   $0x404000,-0x2c(%rbp)
   0x00000000004011f6 <+144>:	mov    -0x2c(%rbp),%eax
   0x00000000004011f9 <+147>:	cltq
   0x00000000004011fb <+149>:	mov    %rax,-0x38(%rbp)
   0x00000000004011ff <+153>:	mov    -0x38(%rbp),%rax
   0x0000000000401203 <+157>:	mov    -0x28(%rbp),%rdx
   0x0000000000401207 <+161>:	mov    %rdx,(%rax)
=> 0x000000000040120a <+164>:	mov    $0x40202c,%edi
   0x000000000040120f <+169>:	call   0x401030 <puts@plt>
   0x0000000000401214 <+174>:	mov    $0x0,%eax
   0x0000000000401219 <+179>:	leave
   0x000000000040121a <+180>:	ret
End of assembler dump.
(gdb) si
0x000000000040120f	42		puts("Hello world!");
(gdb) si
0x0000000000401030 in puts@plt ()
(gdb) disas
Dump of assembler code for function puts@plt:
=> 0x0000000000401030 <+0>:	jmp    *0x2fca(%rip)        # 0x404000 <puts@got.plt>
   0x0000000000401036 <+6>:	push   $0x0
   0x000000000040103b <+11>:	jmp    0x401020
End of assembler dump.
(gdb) x/gx 0x404000
0x404000 <puts@got.plt>:	0x00007ffff7e55100
(gdb) si
0x00007ffff7e55100 in puts () from /lib64/libc.so.6
(gdb) exit
A debugging session is active.

	Inferior 1 [process 4169943] will be killed.

Quit anyway? (y or n) y