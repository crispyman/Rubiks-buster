MPICC = mpicc
MPICCFLAGS = -std=gnu11 -O2 -c -Wall -Wno-unused-variable -Wno-unused
.c.o:
	$(MPICC) $(MPICCFLAGS) $< -o $@

all: RUBIKS

test:
	./RUBIKS

RUBIKS: RUBIKS.o RUBIKS_SEQENTIAL.o RUBIKS_PARALLEL.o HELPERS.o
	$(MPICC) RUBIKS.o RUBIKS_SEQENTIAL.o RUBIKS_PARALLEL.o HELPERS.o -o RUBIKS

RUBIKS.o: RUBIKS.c HELPERS.h

RUBIKS_SEQENTIAL.o: RUBIKS_SEQENTIAL.c HELPERS.h

RUBIKS_PARALLEL.o: RUBIKS_PARALLEL.c HELPERS.h

HELPERS.o: HELPERS.c HELPERS.h

clean:
	rm -rf RUBIKS *.o
