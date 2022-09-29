CC:=gcc
INCLUDE:=-lX11
CFLAGS:=-g -Wall
OBJ:=helperFunctions.o screenshot.o
EXE:=screenshot
FILENAME:=outfile.ppm

.PHONY: all run

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(INCLUDE)

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

clean: 
	rm -f $(EXE) *.o *.ppm

run: $(EXE)
	./$(EXE) $(FILENAME)