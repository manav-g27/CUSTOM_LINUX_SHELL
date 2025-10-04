all: shell

shell: src/main.cpp src/parser.cpp src/executor.cpp src/jobs.cpp
	g++ -std=c++17 -Wall src/*.cpp -o shell

clean:
	rm -f shell