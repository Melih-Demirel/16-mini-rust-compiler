all: clean flex bison rust

clean:
	rm -f ./microrust
	rm -f ./parser/parser.*
	rm -f ./lexer/lex*

flex: lexer/rust.l
	flex -o ./lexer/lexer.cpp --header-file=./lexer/lexer.hpp ./lexer/rust.l

bison: parser/grammar.y
	bison -o ./parser/parser.cpp ./parser/grammar.y -dv -t -Wcounterexamples

rust: main.cpp ast/*/*.cpp environment/*/*.cpp
	c++ -O3 -DNDEBUG -std=c++17 -o microrust main.cpp lexer/lexer.cpp parser/parser.cpp ast/*/*.cpp environment/*.cpp
