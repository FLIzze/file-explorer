CC = g++
CFLAGS = $(shell sdl2-config --cflags) -Iheader   # Add the header directory for includes
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf

# The source files are now inside the src directory
SRC = src/main.c src/display.c src/event.c src/read.c

# The object files correspond to the source files
OBJ = $(SRC:src/%.c=build/%.o)

# The target will be created in the build directory
TARGET = build/program

# Create the target (program) by linking the object files
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile each source file into an object file
build/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up generated files (objects and the target)
clean:
	rm -f build/*.o $(TARGET)

