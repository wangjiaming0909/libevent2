
target = a.out
objects = $(patsubst %.c, %.o, $(wildcard *.c))
cc = gcc


build:$(target)

$(target):$(objects)
	$(cc) $(objects) -o $(target)

$(objects):*.c
	$(cc) -c *.c -o $(objects) -g

clean:
	rm $(objects) $(target)