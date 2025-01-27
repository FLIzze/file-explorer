CC = g++
CFLAGS = $(shell sdl2-config --cflags)  
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf

SRC = main.c   
OBJ = main.o
TARGET = build

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ): $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

