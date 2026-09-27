CXX = g++
CXXFLAGS = -g -Wall

# Target executable
myshell: myshell.o parse.o param.o
	$(CXX) $(CXXFLAGS) -o myshell myshell.o parse.o param.o

# Object file rules
myshell.o: myshell.cpp parse.hpp param.hpp
	$(CXX) $(CXXFLAGS) -c myshell.cpp

parse.o: parse.cpp parse.hpp param.hpp
	$(CXX) $(CXXFLAGS) -c parse.cpp

param.o: param.cpp param.hpp
	$(CXX) $(CXXFLAGS) -c param.cpp

# Clean up build files
clean:
	rm -f *.o myshell