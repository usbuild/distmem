CXX=g++
CXXFLAGS= -g -I.
TARGET=distmem
OBJ=distmem.o domain.o server.o connection.o handler.o
$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@
clean:
	-rm $(OBJ) $(TARGET)
