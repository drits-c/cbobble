PREFIX ?= /usr/local

cbobble: cbobble.c
	$(CC) -Wall -Wextra -O2 -o $@ $<

install: cbobble
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 cbobble $(DESTDIR)$(PREFIX)/bin/cbobble

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/cbobble

clean:
	rm -f cbobble

.PHONY: install uninstall clean
