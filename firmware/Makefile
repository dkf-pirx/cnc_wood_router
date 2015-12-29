IDIR = sources


CC = /opt/msp430-toolchain/bin/msp430-gcc
CFLAGS = -I$(IDIR) -mmcu=msp430f2617 -std=c99
LDFLAGS = -T $(IDIR)/msp430f2617.ld

ODIR = obj
LDIR = .
SRC = sources

_DEPS = main.h AppDef.h comm.h config.h WaveForms.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o AppInit.o comm.o isr.o Task0.o Task1.o Task2.o Task3.o Task4.o Task5.o Task6.o Task7.o   
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -l sources  -O0 -ggdb -Wa,-ahldn -gdwarf-2 -g3


debug: $(OBJ)
	$(CC) -o cnc.out $^ $(CFLAGS) $(LDFLAGS) -O0 -ggdb -Wa,-ahldn -gdwarf-2 -g3

release: $(OBJ)
	$(CC) -o cnc.out $^ $(CFLAGS) $(LDFLAGS) -O3


.PHONY: clean
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
