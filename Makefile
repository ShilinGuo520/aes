
CCFLAGS := -g -fno-builtin

tag:clean main
	echo make

main:main
	gcc $(CCFLAGS) main.c -o main

clean:
	rm -f main
