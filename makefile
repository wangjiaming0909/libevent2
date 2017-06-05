
target = a.out
objects = $(patsubst %.c, %.o, $(wildcard *.c))

build:$(target)

$(target):$(objects)
	gcc $(objects) -o $(target)

$(objects):*.c
	gcc -c *.c -o $(objects) -g

clean:
	rm $(objects) $(target)