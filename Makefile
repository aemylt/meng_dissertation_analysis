CMPHLIB=-L/usr/local/lib/libcmph.la -lcmph
CC=gcc
CARGS=-O3 -Wall

aho-corasick:
	$(CC) $(CARGS) stream_test.c -o aho_corasick_test $(CMPHLIB)