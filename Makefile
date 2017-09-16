SYSTEM_OBJECT_FILES = ./build/system/token.o ./build/system/splitter.o ./build/system/interpreter.o ./build/system/lexer.o ./build/system/parser.o ./build/system/node.o ./build/system/expnode.o ./build/system/varnode.o ./build/system/literalnode.o ./build/system/identifiernode.o ./build/system/debug.o ./build/system/scope.o ./build/system/variable.o ./build/system/fcnode.o ./build/system/keywordnode.o ./build/system/nodefactory.o ./build/system/tokenfactory.o ./build/system/einode.o ./build/system/value.o ./build/system/function.o ./build/system/nativefunction.o ./build/system/functionsystem.o ./build/system/stringnode.o ./build/system/negnode.o ./build/system/statement.o ./build/system/ifstmtnode.o ./build/system/bodynode.o ./build/system/castnode.o ./build/system/arraynode.o ./build/system/newnode.o ./build/system/array.o ./build/system/object.o
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14 -g
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
	g++ -I ./include ./src/system/debug.cpp -o ./build/system/debug.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/scope.cpp -o ./build/system/scope.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/variable.cpp -o ./build/system/variable.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/keywordnode.cpp -o ./build/system/keywordnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/fcnode.cpp -o ./build/system/fcnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/nodefactory.cpp -o ./build/system/nodefactory.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/tokenfactory.cpp -o ./build/system/tokenfactory.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/einode.cpp -o ./build/system/einode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/value.cpp -o ./build/system/value.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/function.cpp -o ./build/system/function.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/nativefunction.cpp -o ./build/system/nativefunction.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/functionsystem.cpp -o ./build/system/functionsystem.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/negnode.cpp -o ./build/system/negnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/stringnode.cpp -o ./build/system/stringnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/statement.cpp -o ./build/system/statement.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/ifstmtnode.cpp -o ./build/system/ifstmtnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/bodynode.cpp -o ./build/system/bodynode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/castnode.cpp -o ./build/system/castnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/arraynode.cpp -o ./build/system/arraynode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/newnode.cpp -o ./build/system/newnode.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/array.cpp -o ./build/system/array.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/object.cpp -o ./build/system/object.o ${OBJECT_FILE_FLAGS}
	g++ ${SYSTEM_OBJECT_FILES} -g -o ${SYSTEM_LIB_FILE_LOCATION} -shared -Wl,-rpath,'./lib'-export-dynamic
standalone: system
	cd bin; \
	g++ -g -I ../include ../src/standalone/main.cpp ./${SYSTEM_LIB_LOCAL_FILENAME}  -std=c++14 -o ./marble;
clean:
	rm ${SYSTEM_OBJECT_FILES}2
