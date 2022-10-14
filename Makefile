CC:=gcc
INCLUDE:=-lX11 -lpng

CFLAGS:=-Ofast -s
LDFLAGS:=-DPNG

OBJ:=helperFunctions.o screenshot.o
EXE:=screenshot

FILENAME:=outfile.png
PHOTO_DISPLAY:=feh

.PHONY: all
all: $(EXE)

.PHONY: debug
debug: CFLAGS:=-g -Wall
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LDFLAGS) $(INCLUDE)

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(EXE) *.o *.ppm *.png

.PHONY: run
run: png

ppm: FILENAME:=outfile.ppm 
ppm: LDFLAGS:=-DPPM
ppm: clean $(EXE)
	./$(EXE) $(FILENAME) $(PHOTO_DISPLAY)

png: FILENAME:=outfile.png
png: clean $(EXE)
	./$(EXE) $(FILENAME) $(PHOTO_DISPLAY)
