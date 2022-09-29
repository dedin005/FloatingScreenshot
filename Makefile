CC:=gcc
INCLUDE:=-lX11
CFLAGS:=-Ofast -s
OBJ:=helperFunctions.o screenshot.o
EXE:=screenshot
FILENAME:=outfile.ppm
PHOTO_DISPLAY:=feh

.PHONY: all run

all: $(EXE)

debug: CFLAGS:=-g -Wall
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(INCLUDE)

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

clean: 
	rm -f $(EXE) *.o *.ppm

run: $(EXE)
	./$(EXE) $(FILENAME) $(PHOTO_DISPLAY)
