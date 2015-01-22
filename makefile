%.exe: %.cpp
	g++ -o $@ $^ -Wall
