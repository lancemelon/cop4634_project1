CXX      = g++
CXXFLAGS = -g -Wall
OBJS     = myshell.o parse.o param.o
TARGET   = myshell

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

myshell.o: myshell.cpp parse.hpp param.hpp
	$(CXX) $(CXXFLAGS) -c myshell.cpp

parse.o: parse.cpp parse.hpp param.hpp
	$(CXX) $(CXXFLAGS) -c parse.cpp

param.o: param.cpp param.hpp
	$(CXX) $(CXXFLAGS) -c param.cpp

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
