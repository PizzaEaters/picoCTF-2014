data = open('book.txt').read().split('\n\n')

for t in [(1, 9, 4), (4, 2, 8), (4, 8, 3), (7, 1, 5), (8, 10, 1)]:
	print data[t[0]-1].split('\n')[t[1]-1].split(' ')[t[2]-1],
