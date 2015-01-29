%.exe: %.cpp parser.cpp
	g++ -o $@ $^ -std=c++0x -Wall