CC = g++
CFLAGS = $(shell sdl2-config --cflags) -Iheader 
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf 

# The source files are inside the src directory
SRC = src/main.c src/display.c src/event.c src/struct.c src/crud.c src/input.c

# The object files correspond to the source files (inside build/)
OBJ = $(SRC:src/%.c=build/%.o)

# The target program
TARGET = build/program

# Ensure the build directory exists
all: build_dir $(TARGET)

# Create the build directory if it doesn't exist
build_dir:
	mkdir -p build

# Link object files to create the final program
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile each source file into an object file
build/%.o: src/%.c | build_dir
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up the build directory
clean:
	rm -rf build/

