# change application name here (executable output name)
TARGET=switchboard

# compiler
CC=gcc 
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

LIB=-L/usr/local/lib/ 
INC=-I/usr/local/include -lbcm2835


PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe 

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc -rdynamic
LDFLAGS=$(PTHREAD) $(GTKLIB) 

OBJS=main.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIB) $(INC)
   
main.o: src/main.c
	$(CC) -c $(CCFLAGS) $(LIB) src/main.c $(GTKLIB) -o main.o
    
clean:
	rm -f *.o $(TARGET)
    
