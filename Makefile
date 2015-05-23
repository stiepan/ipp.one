dictionary: dictionary.o parse.o trie.o
	cc -o dictionary dictionary.o parse.o trie.o

debug: dictionary.dbg.o parse.dbg.o trie.dbg.o
	cc -g -o dictionary.dbg dictionary.dbg.o parse.dbg.o trie.dbg.o

dictionary.o: dictionary.c trie.h parse.h
	cc -c -o dictionary.o dictionary.c

parse.o: parse.c
	cc -c -o parse.o parse.c

trie.o: trie.c
	cc -c -o trie.o trie.c

dictionary.dbg.o: dictionary.c trie.h parse.h
	cc -g -D ASSERT -c -o dictionary.dbg.o dictionary.c

parse.dbg.o: parse.c
	cc -g -D ASSERT -c -o parse.dbg.o parse.c

trie.dbg.o: trie.c
	cc -g -D ASSERT -c -o trie.dbg.o trie.c

.PHONY: clean
clean:
	@-rm -f *.o
	@-rm -f dictionary
	@-rm -f dictionary.dbg
	@echo "Usunięto"
