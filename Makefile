CXX=clang++
CXXFLAGS= -g -I.
TARGET=distmem
OBJ=distmem.cpp domain.cpp server.cpp connection.cpp handler.cpp
test: test.cpp btree.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@
clean:
	-rm test $(TARGET)
