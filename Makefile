default: sprexer

install:
	mv sprexer /usr/bin/sprexer

sprexer:
	c++ main.cpp parser.cpp element.cpp -Os -o sprexer -llexbor-html -llexbor-dom -std=c++17
