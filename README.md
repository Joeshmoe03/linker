# Authors: Hedavam Solano and Josef Liem
# Known Bugs: None that we could identify

# Annotated script proving self-linker's correctness:

[jliem@weathertop assignment4]$ make
gcc -Wall -g -pedantic -fpie -o self-linker self-linker.c

## Works on shell

[jliem@weathertop assignment4]$ ./self-linker
Hello world!
Hello again!

## Works on gdb
[jliem@weathertop assignment4]$ gdb self-linker
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
Starting program: /home/jliem/CS315/assignment4/self-linker

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.fedoraproject.org/>
Enable debuginfod for this session? (y or [n]) n
Debuginfod has been disabled.
To make this setting permanent, add 'set debuginfod enabled off' to .gdbinit.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".
Hello world!
Hello again!
[Inferior 1 (process 893061) exited normally]
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.37-13.fc38.x86_64
(gdb) q

## Using gdb to test program's correctness:
We place a breakpoint before our program's puts() call and run until we hit it.
We then disassemble (disas) and execute (si) instructions until we reach the puts@plt () call.
Once reached, we disassemble (disas) the puts@plt () call, and grab the entry within the GOT that pertains to puts()
Under "normal" circumstances, this would point to the second instruction of the puts PLT entry since the GOT entry before puts() is initially called is virtually empty.
But, since we implememnted our self-linker, we can see using x/gx command that our GOT entry pertaning to puts() points somewhere else.
After stepping through one more instruction, we can see that this value is indeed the "real" address of puts() in libc.

[jliem@weathertop assignment4]$ gdb self-linker
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
(gdb) b 39
Breakpoint 1 at 0x401221: file self-linker.c, line 39.
(gdb) r
Starting program: /home/jliem/CS315/assignment4/self-linker

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.fedoraproject.org/>
Enable debuginfod for this session? (y or [n]) n
Debuginfod has been disabled.
To make this setting permanent, add 'set debuginfod enabled off' to .gdbinit.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".

Breakpoint 1, main (argc=1, argv=0x7fffffffe108) at self-linker.c:39
39              puts("Hello world!");
Missing separate debuginfos, use: dnf debuginfo-install glibc-2.37-13.fc38.x86_64
(gdb) disas
Dump of assembler code for function main:
   0x0000000000401176 <+0>:     push   %rbp
   0x0000000000401177 <+1>:     mov    %rsp,%rbp
   0x000000000040117a <+4>:     sub    $0x40,%rsp
   0x000000000040117e <+8>:     mov    %edi,-0x34(%rbp)
   0x0000000000401181 <+11>:    mov    %rsi,-0x40(%rbp)
   0x0000000000401185 <+15>:    lea    0x2e5c(%rip),%rax        # 0x403fe8
   0x000000000040118c <+22>:    mov    %rax,-0x8(%rbp)
   0x0000000000401190 <+26>:    lea    0xe79(%rip),%rax        # 0x402010
   0x0000000000401197 <+33>:    mov    %rax,-0x10(%rbp)
   0x000000000040119b <+37>:    lea    0xe78(%rip),%rax        # 0x40201a
   0x00000000004011a2 <+44>:    mov    %rax,-0x18(%rbp)
   0x00000000004011a6 <+48>:    mov    -0x10(%rbp),%rax
   0x00000000004011aa <+52>:    mov    $0x1,%esi
   0x00000000004011af <+57>:    mov    %rax,%rdi
   0x00000000004011b2 <+60>:    call   0x401040 <dlopen@plt>
   0x00000000004011b7 <+65>:    mov    %rax,-0x20(%rbp)
   0x00000000004011bb <+69>:    cmpq   $0x0,-0x20(%rbp)
   0x00000000004011c0 <+74>:    jne    0x4011db <main+101>
   0x00000000004011c2 <+76>:    lea    0xe56(%rip),%rax        # 0x40201f
   0x00000000004011c9 <+83>:    mov    %rax,%rdi
   0x00000000004011cc <+86>:    call   0x401050 <perror@plt>
   0x00000000004011d1 <+91>:    mov    $0x1,%edi
   0x00000000004011d6 <+96>:    call   0x401070 <exit@plt>
   0x00000000004011db <+101>:   mov    -0x18(%rbp),%rdx
   0x00000000004011df <+105>:   mov    -0x20(%rbp),%rax
   0x00000000004011e3 <+109>:   mov    %rdx,%rsi
   0x00000000004011e6 <+112>:   mov    %rax,%rdi
   0x00000000004011e9 <+115>:   call   0x401060 <dlsym@plt>
   0x00000000004011ee <+120>:   mov    %rax,-0x28(%rbp)
   0x00000000004011f2 <+124>:   cmpq   $0x0,-0x28(%rbp)
   0x00000000004011f7 <+129>:   jne    0x401212 <main+156>
   0x00000000004011f9 <+131>:   lea    0xe26(%rip),%rax        # 0x402026
   0x0000000000401200 <+138>:   mov    %rax,%rdi
   0x0000000000401203 <+141>:   call   0x401050 <perror@plt>
   0x0000000000401208 <+146>:   mov    $0x1,%edi
   0x000000000040120d <+151>:   call   0x401070 <exit@plt>
   0x0000000000401212 <+156>:   mov    -0x8(%rbp),%rax
   0x0000000000401216 <+160>:   lea    0x18(%rax),%rdx
   0x000000000040121a <+164>:   mov    -0x28(%rbp),%rax
   0x000000000040121e <+168>:   mov    %rax,(%rdx)
=> 0x0000000000401221 <+171>:   lea    0xe04(%rip),%rax        # 0x40202c
   0x0000000000401228 <+178>:   mov    %rax,%rdi
   0x000000000040122b <+181>:   call   0x401030 <puts@plt>
   0x0000000000401230 <+186>:   lea    0xe02(%rip),%rax        # 0x402039
   0x0000000000401237 <+193>:   mov    %rax,%rdi
   0x000000000040123a <+196>:   call   0x401030 <puts@plt>
   0x000000000040123f <+201>:   mov    -0x20(%rbp),%rax
   0x0000000000401243 <+205>:   mov    %rax,%rdi
   0x0000000000401246 <+208>:   call   0x401080 <dlclose@plt>
   0x000000000040124b <+213>:   mov    $0x0,%eax
--Type <RET> for more, q to quit, c to continue without paging--
   0x0000000000401250 <+218>:   leave
   0x0000000000401251 <+219>:   ret
End of assembler dump.
(gdb) si
0x0000000000401228      39              puts("Hello world!");
(gdb) si
0x000000000040122b      39              puts("Hello world!");
(gdb) si
0x0000000000401030 in puts@plt ()
(gdb) disas
Dump of assembler code for function puts@plt:
=> 0x0000000000401030 <+0>:     jmp    *0x2fca(%rip)        # 0x404000 <puts@got.plt>
   0x0000000000401036 <+6>:     push   $0x0
   0x000000000040103b <+11>:    jmp    0x401020
End of assembler dump.
(gdb) x/gx 0x404000
0x404000 <puts@got.plt>:        0x00007ffff7e55100
(gdb) si
0x00007ffff7e55100 in puts () from /lib64/libc.so.6
(gdb) exit
A debugging session is active.

        Inferior 1 [process 891895] will be killed.

Quit anyway? (y or n) y
[jliem@weathertop assignment4]$
