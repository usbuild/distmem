CC=g++
CFLAGS= -g
TARGET=distmem.exe
OBJ=distmem.o
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
clean:
	-rm $(OBJ) $(TARGET)
