SYSTEM_OBJECT_FILES = ./build/system/token.o ./build/system/splitter.o ./build/system/interpreter.o ./build/system/lexer.o ./build/system/parser.o ./build/system/node.o ./build/system/expnode.o ./build/system/varnode.o ./build/system/literalnode.o ./build/system/identifiernode.o
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++11 -g
SYSTEM_LIB_LOCAL_FILENAME = marblelib.so
SYSTEM_LIB_FILE_LOCATION = ./bin/${SYSTEM_LIB_LOCAL_FILENAME}
system:
	g++ -I ./include ./src/system/token.cpp -o ./build/system/token.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/splitter.cpp -o ./build/system/splitter.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/interpreter.cpp -o ./build/system/interpreter.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/lexer.cpp -o ./build/system/lexer.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/parser.cpp -o ./build/system/parser.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/node.cpp -o ./build/system/node.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/expnode.cpp -o ./build/system/expnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/varnode.cpp -o ./build/system/varnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/literalnode.cpp -o ./build/system/literalnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/identifiernode.cpp -o ./build/system/identifiernode.o ${OBJECT_FILE_FLAGS}

	g++ ${SYSTEM_OBJECT_FILES} -g -o ${SYSTEM_LIB_FILE_LOCATION}  -shared -Wl,-rpath,'./lib'-export-dynamic
standalone: system
	cd bin; \
	g++ -g -I ../include ../src/standalone/main.cpp ./${SYSTEM_LIB_LOCAL_FILENAME} -o ./marble;
clean:
	rm ${SYSTEM_OBJECT_FILES}
