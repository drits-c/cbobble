PREFIX ?= /usr/local

cbobble: cbobble.c
	$(CC) -Wall -Wextra -O2 -o $@ $<

install: cbobble
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 cbobble $(DESTDIR)$(PREFIX)/bin/cbobble
	install -d $(DESTDIR)$(PREFIX)/share/man/man1
	install -m 644 cbobble.1 $(DESTDIR)$(PREFIX)/share/man/man1/cbobble.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/cbobble
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/cbobble.1

clean:
	rm -f cbobble

.PHONY: install uninstall clean
