EXECFILE=demo
CFLAGS=$(shell pkg-config --cflags gtk+-3.0) -Wall
LDFLAGS=$(shell pkg-config --libs gtk+-3.0) -lm

install: $(EXECFILE)

$(EXECFILE):main.c gpgraphwidget2d.[ch]
	$(CC) $(CFLAGS) -ggdb3 $^ $(LDFLAGS) -o $(EXECFILE)

clean:
	rm $(EXECFILE)
