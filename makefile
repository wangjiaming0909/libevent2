
target = build/a.out
objects = build/$(patsubst %.c,%.o,$(wildcard *.c))
cc = gcc
depend = $(patsubst %.c, %.d, $(wildcard *.c))

build:$(target)

$(target):$(objects)
	@echo linking...
	$(cc) $^ -o $@ -levent
	@echo ok...

$(objects):*.c
	@echo compiling...
	$(cc) -c $^ -o $@ -g 


dep:$(depend)

%.d:%.c
	$(cc) -M $< >$@.$$$$

.PHONY:clean

clean:FORCE
	@echo cleaning...
	$(RM) $(objects) $(target)
FORCE:

#??
print:*.c
	echo *.c
	touch print