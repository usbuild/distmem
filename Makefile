CXX=g++
CXXFLAGS= -g -I.
TARGET=distmem
OBJ=distmem.o domain.o server.o
$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@
clean:
	-rm $(OBJ) $(TARGET)
