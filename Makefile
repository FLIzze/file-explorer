CC = g++
CFLAGS = $(shell sdl2-config --cflags) -Iheader 
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf 

SRC = src/main.c src/draw.c src/event.c src/struct.c src/crud.c src/input.c src/sdl_init.c

OBJ = $(SRC:src/%.c=build/%.o)

TARGET = build/program

all: build_dir $(TARGET)

build_dir:
	mkdir -p build

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

build/%.o: src/%.c | build_dir
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf build/
