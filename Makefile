default: sprexer

install:
	mv sprexer /usr/bin/sprexer

sprexer:
	c++ html_lexer/main.cpp html_lexer/parser.cpp html_lexer/element.cpp -Os -o sprexer -llexbor-html -llexbor-dom -std=c++17
