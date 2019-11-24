CC := gcc
CFLAG := -g
LShell: main.c
		$(CC) $(CFLAG) main.c
		#remove this comment before submission as well as # on next line
		#ulimit -u 3
		./a.out