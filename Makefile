# Copyright 2022 Elena Dulgheru <elenadulgheru03@gmail.com>

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=my_octave

build: $(TARGETS)

my_octave: my_octave.c strassen.c
	$(CC) $(CFLAGS) my_octave.c strassen.c -o my_octave

pack:
	zip -FSr 314CA_ElenaDulgheru_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
