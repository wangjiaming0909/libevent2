
target = a.out
objects = $(patsubst %.c, %.o, $(wildcard *.c))
cc = gcc


build:$(target)

$(target):$(objects)
	$(cc) $(objects) -o $(target) -levent

$(objects):*.c
	$(cc) -c *.c -o $(objects) -g

.PHONY:clean

clean:
	$(RM) $(objects) $(target)