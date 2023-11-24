# Annotated script proving self-linker's correctness:

[hsolano@weathertop assignment4]$ make
gcc -Wall -g -pedantic -o self-linker self-linker.c

## Works on shell
[hsolano@weathertop assignment4]$ ./self-linker
helloworld /* desired output */

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
helloworld /* desired output */
[Inferior 1 (process 4123351) exited normally]
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.37-13.fc38.x86_64
(gdb) exit
[hsolano@weathertop assignment4]$ 

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
(gdb) b 21
Breakpoint 1 at 0x401190: file self-linker.c, line 21.
(gdb) r
Starting program: /home/hsolano/cs315/assignment4/self-linker 

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.fedoraproject.org/>
Enable debuginfod for this session? (y or [n]) n
Debuginfod has been disabled.
To make this setting permanent, add 'set debuginfod enabled off' to .gdbinit.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".

Breakpoint 1, main (argc=1, argv=0x7fffffffe0d8) at self-linker.c:21
21		puts("helloworld");
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.37-13.fc38.x86_64
(gdb) disas
Dump of assembler code for function main:
   0x0000000000401146 <+0>:	push   %rbp
   0x0000000000401147 <+1>:	mov    %rsp,%rbp
   0x000000000040114a <+4>:	sub    $0x30,%rsp
   0x000000000040114e <+8>:	mov    %edi,-0x24(%rbp)
   0x0000000000401151 <+11>:	mov    %rsi,-0x30(%rbp)
   0x0000000000401155 <+15>:	mov    $0x1,%esi
   0x000000000040115a <+20>:	mov    $0x402010,%edi
   0x000000000040115f <+25>:	call   0x401040 <dlopen@plt>
   0x0000000000401164 <+30>:	mov    %rax,-0x8(%rbp)
   0x0000000000401168 <+34>:	mov    -0x8(%rbp),%rax
   0x000000000040116c <+38>:	mov    $0x40201a,%esi
   0x0000000000401171 <+43>:	mov    %rax,%rdi
   0x0000000000401174 <+46>:	call   0x401050 <dlsym@plt>
   0x0000000000401179 <+51>:	mov    %rax,-0x10(%rbp)
   0x000000000040117d <+55>:	movq   $0x404000,-0x18(%rbp)
   0x0000000000401185 <+63>:	mov    -0x18(%rbp),%rax
   0x0000000000401189 <+67>:	mov    -0x10(%rbp),%rdx
   0x000000000040118d <+71>:	mov    %rdx,(%rax)
=> 0x0000000000401190 <+74>:	mov    $0x40201f,%edi
   0x0000000000401195 <+79>:	call   0x401030 <puts@plt>
   0x000000000040119a <+84>:	mov    $0x0,%eax
   0x000000000040119f <+89>:	leave
   0x00000000004011a0 <+90>:	ret
End of assembler dump.
(gdb) si
0x0000000000401195	21		puts("helloworld");
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

	Inferior 1 [process 4120349] will be killed.

Quit anyway? (y or n) y
[hsolano@weathertop assignment4]$ exit
exit
