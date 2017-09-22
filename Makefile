SYSTEM_OBJECT_FILES = ./build/system/token.o ./build/system/splitter.o ./build/system/interpreter.o ./build/system/lexer.o ./build/system/parser.o ./build/system/node.o ./build/system/expnode.o ./build/system/varnode.o ./build/system/literalnode.o ./build/system/identifiernode.o ./build/system/debug.o ./build/system/scope.o ./build/system/variable.o ./build/system/fcnode.o ./build/system/keywordnode.o ./build/system/nodefactory.o ./build/system/tokenfactory.o ./build/system/einode.o ./build/system/value.o ./build/system/function.o ./build/system/nativefunction.o ./build/system/functionsystem.o ./build/system/stringnode.o ./build/system/negnode.o ./build/system/statement.o ./build/system/ifstmtnode.o ./build/system/bodynode.o ./build/system/castnode.o ./build/system/arraynode.o ./build/system/newnode.o ./build/system/array.o ./build/system/object.o ./build/system/logger.o ./build/system/posinfo.o
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14 -g
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ./bin/${SYSTEM_LIB_LOCAL_FILENAME}

system: ${SYSTEM_OBJECT_FILES}
	g++ ${SYSTEM_OBJECT_FILES} -g -o ${SYSTEM_LIB_FILE_LOCATION} -shared -Wl,-rpath,'./lib'-export-dynamic
	
./build/system/token.o: ./src/system/token.cpp
	g++ -I ./include ./src/system/token.cpp -o ./build/system/token.o ${OBJECT_FILE_FLAGS}
./build/system/splitter.o: ./src/system/splitter.cpp
	g++ -I ./include ./src/system/splitter.cpp -o ./build/system/splitter.o ${OBJECT_FILE_FLAGS}
./build/system/interpreter.o: ./src/system/interpreter.cpp
	g++ -I ./include ./src/system/interpreter.cpp -o ./build/system/interpreter.o ${OBJECT_FILE_FLAGS}
./build/system/lexer.o: ./src/system/lexer.cpp
	g++ -I ./include ./src/system/lexer.cpp -o ./build/system/lexer.o ${OBJECT_FILE_FLAGS}
./build/system/parser.o: ./src/system/parser.cpp
	g++ -I ./include ./src/system/parser.cpp -o ./build/system/parser.o ${OBJECT_FILE_FLAGS}
./build/system/node.o: ./src/system/node.cpp
	g++ -I ./include ./src/system/node.cpp -o ./build/system/node.o ${OBJECT_FILE_FLAGS}	
./build/system/expnode.o: ./src/system/expnode.cpp
	g++ -I ./include ./src/system/expnode.cpp -o ./build/system/expnode.o ${OBJECT_FILE_FLAGS}	
./build/system/varnode.o: ./src/system/expnode.cpp
	g++ -I ./include ./src/system/varnode.cpp -o ./build/system/varnode.o ${OBJECT_FILE_FLAGS}
./build/system/literalnode.o: ./src/system/literalnode.cpp
	g++ -I ./include ./src/system/literalnode.cpp -o ./build/system/literalnode.o ${OBJECT_FILE_FLAGS}
./build/system/identifiernode.o: ./src/system/identifiernode.cpp
	g++ -I ./include ./src/system/identifiernode.cpp -o ./build/system/identifiernode.o ${OBJECT_FILE_FLAGS}
./build/system/debug.o: ./src/system/debug.cpp
	g++ -I ./include ./src/system/debug.cpp -o ./build/system/debug.o ${OBJECT_FILE_FLAGS}
./build/system/scope.o: ./src/system/scope.cpp
	g++ -I ./include ./src/system/scope.cpp -o ./build/system/scope.o ${OBJECT_FILE_FLAGS}
./build/system/variable.o: ./src/system/variable.cpp
	g++ -I ./include ./src/system/variable.cpp -o ./build/system/variable.o ${OBJECT_FILE_FLAGS}
./build/system/keywordnode.o: ./src/system/keywordnode.cpp
	g++ -I ./include ./src/system/keywordnode.cpp -o ./build/system/keywordnode.o ${OBJECT_FILE_FLAGS}
./build/system/fcnode.o: ./src/system/fcnode.cpp
	g++ -I ./include ./src/system/fcnode.cpp -o ./build/system/fcnode.o ${OBJECT_FILE_FLAGS}
./build/system/nodefactory.o: ./src/system/nodefactory.cpp
	g++ -I ./include ./src/system/nodefactory.cpp -o ./build/system/nodefactory.o ${OBJECT_FILE_FLAGS}
./build/system/tokenfactory.o: ./src/system/tokenfactory.cpp
	g++ -I ./include ./src/system/tokenfactory.cpp -o ./build/system/tokenfactory.o ${OBJECT_FILE_FLAGS}
./build/system/einode.o: ./src/system/einode.cpp
	g++ -I ./include ./src/system/einode.cpp -o ./build/system/einode.o ${OBJECT_FILE_FLAGS}
./build/system/value.o: ./src/system/value.cpp
	g++ -I ./include ./src/system/value.cpp -o ./build/system/value.o ${OBJECT_FILE_FLAGS}
./build/system/function.o: ./src/system/function.cpp
	g++ -I ./include ./src/system/function.cpp -o ./build/system/function.o ${OBJECT_FILE_FLAGS}
./build/system/nativefunction.o: ./src/system/nativefunction.cpp
	g++ -I ./include ./src/system/nativefunction.cpp -o ./build/system/nativefunction.o ${OBJECT_FILE_FLAGS}
./build/system/functionsystem.o: ./src/system/functionsystem.cpp
	g++ -I ./include ./src/system/functionsystem.cpp -o ./build/system/functionsystem.o ${OBJECT_FILE_FLAGS}
./build/system/negnode.o: ./src/system/negnode.cpp
	g++ -I ./include ./src/system/negnode.cpp -o ./build/system/negnode.o ${OBJECT_FILE_FLAGS}
./build/system/stringnode.o: ./src/system/stringnode.cpp
	g++ -I ./include ./src/system/stringnode.cpp -o ./build/system/stringnode.o ${OBJECT_FILE_FLAGS}
./build/system/statement.o: ./src/system/statement.cpp
	g++ -I ./include ./src/system/statement.cpp -o ./build/system/statement.o ${OBJECT_FILE_FLAGS}
./build/system/ifstmtnode.o: ./src/system/ifstmtnode.cpp
	g++ -I ./include ./src/system/ifstmtnode.cpp -o ./build/system/ifstmtnode.o ${OBJECT_FILE_FLAGS}
./build/system/bodynode.o: ./src/system/bodynode.cpp
	g++ -I ./include ./src/system/bodynode.cpp -o ./build/system/bodynode.o ${OBJECT_FILE_FLAGS}
./build/system/castnode.o: ./src/system/castnode.cpp
	g++ -I ./include ./src/system/castnode.cpp -o ./build/system/castnode.o ${OBJECT_FILE_FLAGS}
./build/system/arraynode.o: ./src/system/arraynode.cpp
	g++ -I ./include ./src/system/arraynode.cpp -o ./build/system/arraynode.o ${OBJECT_FILE_FLAGS}
./build/system/newnode.o: ./src/system/newnode.cpp
	g++ -I ./include ./src/system/newnode.cpp -o ./build/system/newnode.o ${OBJECT_FILE_FLAGS}
./build/system/array.o: ./src/system/array.cpp
	g++ -I ./include ./src/system/array.cpp -o ./build/system/array.o ${OBJECT_FILE_FLAGS}
./build/system/object.o: ./src/system/object.cpp
	g++ -I ./include ./src/system/object.cpp -o ./build/system/object.o ${OBJECT_FILE_FLAGS}
./build/system/logger.o: ./src/system/logger.cpp
	g++ -I ./include ./src/system/logger.cpp -o ./build/system/logger.o ${OBJECT_FILE_FLAGS}
./build/system/posinfo.o: ./src/system/posinfo.cpp
	g++ -I ./include ./src/system/posinfo.cpp -o ./build/system/posinfo.o ${OBJECT_FILE_FLAGS}	
standalone: system
	cd bin; \
	g++ -g -I ../include ../src/standalone/main.cpp ./${SYSTEM_LIB_LOCAL_FILENAME}  -std=c++14 -o ./marble;
apache2: system
	sudo cp ${SYSTEM_LIB_FILE_LOCATION} /usr/lib/${SYSTEM_LIB_LOCAL_FILENAME}
	g++ -fPIC -shared -std=c++14 -I /usr/include/apache2 -I /usr/include/apr-1.0 -I ./include ./src/apache/mod_marble.cpp /usr/lib/${SYSTEM_LIB_LOCAL_FILENAME} -o ./bin/mod_marble.so
apache2-install: apache2
	sudo apxs -i -a -n mod_marble ./bin/mod_marble.so
clean:
	rm ${SYSTEM_OBJECT_FILES}
