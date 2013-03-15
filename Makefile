CXX=g++
CXXFLAGS= -g -I.
TARGET=distmem
OBJ=distmem.cpp domain.cpp server.cpp connection.cpp handler.cpp
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@
release: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -O2 -o $(TARGET)
clean:
	-rm $(TARGET)
