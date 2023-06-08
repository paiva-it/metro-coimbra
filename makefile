# Compiler to use
CC = clang

# Compiler flags, e.g. -g for debug, -O2 for optimization, -Wall for warnings
CFLAGS =-fsanitize=address -g -Wall

# Name of the final executable
TARGET = myapp

# List of all the object files
OBJS = main.o void-list/void-list.o paragem/paragem.o linha/linha.o  dynamic-array/dynamic-array.o  code/code.o 

# Default target
all: $(TARGET)

# Linking the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compiling the source files into object files
main.o: main.c main.h void-list/void-list.h paragem/paragem.h linha/linha.h  dynamic-array/dynamic-array.h  code/code.h 
	$(CC) $(CFLAGS) -c main.c -o main.o

void-list/void-list.o: void-list/void-list.c void-list/void-list.h
	$(CC) $(CFLAGS) -c void-list/void-list.c -o void-list/void-list.o

paragem/paragem.o: paragem/paragem.c paragem/paragem.h
	$(CC) $(CFLAGS) -c paragem/paragem.c -o paragem/paragem.o

linha/linha.o: linha/linha.c linha/linha.h void-list/void-list.h
	$(CC) $(CFLAGS) -c linha/linha.c -o linha/linha.o

dynamic-array/dynamic-array.o: dynamic-array/dynamic-array.c dynamic-array/dynamic-array.h void-list/void-list.h
	$(CC) $(CFLAGS) -c dynamic-array/dynamic-array.c -o dynamic-array/dynamic-array.o

code/code.o: code/code.c code/code.h
	$(CC) $(CFLAGS) -c code/code.c -o code/code.o

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)