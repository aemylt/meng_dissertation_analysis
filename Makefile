CMPHLIB=-L/usr/local/lib/libcmph.la -lcmph
GMPLIB=-L/gmp_install/lib -lgmp
CC=gcc
CARGS=-O3 -Wall

aho-corasick:
	$(CC) $(CARGS) -D AHO_CORASICK_TEST stream_test.c -o aho_corasick_test $(CMPHLIB)

dict-matching:
	$(CC) $(CARGS) stream_test.c -o dict_matching_test $(CMPHLIB) $(GMPLIB)