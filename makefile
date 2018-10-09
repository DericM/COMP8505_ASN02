CC          = g++
CFLAG       = -Wall
PROG_NAME   = Stego

all: $(PROG_NAME)

$(PROG_NAME): dcstego.o EasyBMP.o
	$(CC) $(CFLAGS) -o bin/$(PROG_NAME) build/dcstego.o build/EasyBMP.o

dcstego.o: src/dcstego.cpp
	$(CC) $(CFLAGS) -c src/dcstego.cpp -o build/dcstego.o -ltomcrypt

EasyBMP.o: src/EasyBMP/EasyBMP.cpp
	$(CC) $(CFLAGS) -c src/EasyBMP/EasyBMP.cpp -o build/EasyBMP.o

clean:
	rm -f bin/$(PROG_NAME) build/*.o