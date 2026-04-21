CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -std=c11
LDFLAGS = -lm

TARGETS = regular conversion 2quant 4quant 5quant 6quant 8quant 16quant

all: $(TARGETS)

regular:    regular.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

conversion: conversion.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

2quant:  2quant.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

4quant:  4quant.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

5quant:  5quant.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

6quant:  6quant.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

8quant:  8quant.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

16quant: 16quant.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean
