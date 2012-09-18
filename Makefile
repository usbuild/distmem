CXX=g++
CXXFLAGS= -g
TARGET=distmem
OBJ=distmem.o 
$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@
clean:
	-rm $(OBJ) $(TARGET)
