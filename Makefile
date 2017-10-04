EXECFILE=demo
PARSER_SOURCES=parser/parser.tab.c parser/lex.yy.c
PARSER_HEADERS=parser/parser.tab.h
SOURCES=$(wildcard *.c) $(wildcard */*.c) $(PARSER_SOURCES)
OBJECTS=$(SOURCES:%.c=%.o)
CFLAGS=$(shell pkg-config --cflags gtk+-3.0) -Wall -Werror -g -ggdb3
LDFLAGS=$(shell pkg-config --libs gtk+-3.0) -lm
BISON=bison
FLEX=flex

.PHONY: clean all

all: $(EXECFILE)

parser/parser.tab.c: parser/parser.y
	$(BISON) -d $^ -o $@

parser/lex.yy.c: parser/tokens.l
	$(FLEX) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

$(EXECFILE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(EXECFILE)
	rm -f $(OBJECTS)
	rm -f $(PARSER_SOURCES) $(PARSER_HEADERS)
