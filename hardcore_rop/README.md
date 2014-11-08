Hardcore ROP - 200
==================

### Desc:
This program is obviously broken, but thanks to ASLR, PIE, and NX it's still pretty secure! Right?
NB: This problem is running in a slightly unusual setup to get extra PIE randomness. If you have an exploit that works 100% reliably locally (outside of GDB, which often disables any randomness), but you can't get it to land on our server, feel free to message us for help.

### Hint:
This is a statically linked binary (using musl libc). There is no full libc available for you to return into, but if you can leak a .text section address you can return into main(), randop(), and the chunks of libc that are included. Also, you'll probably need to hunt for ROP gadgets: here is a nice tool for that.

### Vulnerable code:
```c
// PIE, NX, statically linked, with symbols.
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAPLEN (4096*10)

void randop() {
	munmap((void*)0x0F000000, MAPLEN);
	void *buf = mmap((void*)0x0F000000, MAPLEN, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE|MAP_FIXED, 0, 0);
	unsigned seed;
	if(read(0, &seed, 4) != 4) return;
	srand(seed);
	for(int i = 0; i < MAPLEN - 4; i+=3) {
		*(int *)&((char*)buf)[i] = rand();
		if(i%66 == 0) ((char*)buf)[i] = 0xc3;
	}
	mprotect(buf, MAPLEN, PROT_READ|PROT_EXEC);
	puts("ROP time!");
	fflush(stdout);
	size_t x, count = 0;
	do x = read(0, ((char*)&seed)+count, 555-count);
	while(x > 0 && (count += x) < 555 && ((char*)&seed)[count-1] != '\n');
}

int main(int argc, char *argv[]) {
	struct stat st;
	if(argc != 2 || chdir(argv[1]) != 0 || stat("./flag", &st) != 0) {
		puts("oops, problem set up wrong D:");
		fflush(stdout);
		return 1;
	} else {
		puts("yo, what's up?");
		alarm(30); sleep(1);
		randop();
		fflush(stdout);
		return 0;
	}
}
```

### Getting the flag:
```
$ python hardcore_rop.py
hard_as_PIE_amirite?
```
