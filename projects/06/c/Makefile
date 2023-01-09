CC := g++
.PHONY: clean
all: assembler

assembler: assembler.cpp
	${CC} assembler.cpp -o ass
	./ass next.asm

clean:
	rm ass