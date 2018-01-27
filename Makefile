SYSTEM_OBJECT_FILES = ./build/system/token.o ./build/system/splitter.o ./build/system/interpreter.o ./build/system/lexer.o ./build/system/parser.o ./build/system/node.o ./build/system/expnode.o ./build/system/varnode.o ./build/system/literalnode.o ./build/system/identifiernode.o ./build/system/debug.o ./build/system/scope.o ./build/system/variable.o ./build/system/fcnode.o ./build/system/keywordnode.o ./build/system/nodefactory.o ./build/system/tokenfactory.o ./build/system/einode.o ./build/system/value.o ./build/system/function.o ./build/system/nativefunction.o ./build/system/functionsystem.o ./build/system/stringnode.o ./build/system/negnode.o ./build/system/statement.o ./build/system/ifstmtnode.o ./build/system/bodynode.o ./build/system/castnode.o ./build/system/arraynode.o ./build/system/newnode.o ./build/system/array.o ./build/system/object.o ./build/system/logger.o ./build/system/posinfo.o ./build/system/class.o ./build/system/csystem.o ./build/system/fnode.o ./build/system/inode.o ./build/system/writtenfunction.o ./build/system/retnode.o ./build/system/classnode.o ./build/system/validator.o ./build/system/scopehandler.o ./build/system/systemhandler.o ./build/system/evaluatingnode.o ./build/system/datatype.o ./build/system/groupedfunction.o ./build/system/singlefunction.o ./build/system/vartype.o ./build/system/exceptions/systemexception.o ./build/system/trynode.o ./build/system/thrownode.o ./build/system/exceptionobject.o ./build/system/whilenode.o ./build/system/breakable.o ./build/system/breaknode.o ./build/system/continuenode.o ./build/system/dowhilenode.o ./build/system/fornode.o ./build/system/listnode.o ./build/system/modulesystem.o ./build/system/module.o ./build/system/includenode.o ./build/system/purefunction.o ./build/system/includeoncenode.o ./build/system/misc.o
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
./build/system/modulesystem.o: ./src/system/modulesystem.cpp
	g++ -I ./include ./src/system/modulesystem.cpp -o ./build/system/modulesystem.o ${OBJECT_FILE_FLAGS}
./build/system/module.o: ./src/system/module.cpp
	g++ -I ./include ./src/system/module.cpp -o ./build/system/module.o ${OBJECT_FILE_FLAGS}
./build/system/lexer.o: ./src/system/lexer.cpp
	g++ -I ./include ./src/system/lexer.cpp -o ./build/system/lexer.o ${OBJECT_FILE_FLAGS}
./build/system/parser.o: ./src/system/parser.cpp
	g++ -I ./include ./src/system/parser.cpp -o ./build/system/parser.o ${OBJECT_FILE_FLAGS}
./build/system/node.o: ./src/system/node.cpp
	g++ -I ./include ./src/system/node.cpp -o ./build/system/node.o ${OBJECT_FILE_FLAGS}	
./build/system/expnode.o: ./src/system/expnode.cpp
	g++ -I ./include ./src/system/expnode.cpp -o ./build/system/expnode.o ${OBJECT_FILE_FLAGS}	
./build/system/varnode.o: ./src/system/varnode.cpp
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
./build/system/class.o: ./src/system/class.cpp
	g++ -I ./include ./src/system/class.cpp -o ./build/system/class.o ${OBJECT_FILE_FLAGS}	
./build/system/csystem.o: ./src/system/csystem.cpp
	g++ -I ./include ./src/system/csystem.cpp -o ./build/system/csystem.o ${OBJECT_FILE_FLAGS}	
./build/system/fnode.o: ./src/system/fnode.cpp
	g++ -I ./include ./src/system/fnode.cpp -o ./build/system/fnode.o ${OBJECT_FILE_FLAGS}	
./build/system/inode.o: ./src/system/inode.cpp
	g++ -I ./include ./src/system/inode.cpp -o ./build/system/inode.o ${OBJECT_FILE_FLAGS}	
./build/system/writtenfunction.o: ./src/system/writtenfunction.cpp
	g++ -I ./include ./src/system/writtenfunction.cpp -o ./build/system/writtenfunction.o ${OBJECT_FILE_FLAGS}	
./build/system/retnode.o: ./src/system/retnode.cpp
	g++ -I ./include ./src/system/retnode.cpp -o ./build/system/retnode.o ${OBJECT_FILE_FLAGS}	
./build/system/classnode.o: ./src/system/classnode.cpp
	g++ -I ./include ./src/system/classnode.cpp -o ./build/system/classnode.o ${OBJECT_FILE_FLAGS}	
./build/system/validator.o: ./src/system/validator.cpp
	g++ -I ./include ./src/system/validator.cpp -o ./build/system/validator.o ${OBJECT_FILE_FLAGS}	
./build/system/scopehandler.o: ./src/system/scopehandler.cpp
	g++ -I ./include ./src/system/scopehandler.cpp -o ./build/system/scopehandler.o ${OBJECT_FILE_FLAGS}	
./build/system/systemhandler.o: ./src/system/systemhandler.cpp
	g++ -I ./include ./src/system/systemhandler.cpp -o ./build/system/systemhandler.o ${OBJECT_FILE_FLAGS}
./build/system/evaluatingnode.o: ./src/system/evaluatingnode.cpp
	g++ -I ./include ./src/system/evaluatingnode.cpp -o ./build/system/evaluatingnode.o ${OBJECT_FILE_FLAGS}	
./build/system/datatype.o: ./src/system/datatype.cpp
	g++ -I ./include ./src/system/datatype.cpp -o ./build/system/datatype.o ${OBJECT_FILE_FLAGS}		
./build/system/groupedfunction.o: ./src/system/groupedfunction.cpp
	g++ -I ./include ./src/system/groupedfunction.cpp -o ./build/system/groupedfunction.o ${OBJECT_FILE_FLAGS}	
./build/system/singlefunction.o: ./src/system/singlefunction.cpp
	g++ -I ./include ./src/system/singlefunction.cpp -o ./build/system/singlefunction.o ${OBJECT_FILE_FLAGS}	
./build/system/vartype.o: ./src/system/vartype.cpp
	g++ -I ./include ./src/system/vartype.cpp -o ./build/system/vartype.o ${OBJECT_FILE_FLAGS}	
./build/system/exceptions/systemexception.o: ./src/system/exceptions/systemexception.cpp
	g++ -I ./include ./src/system/exceptions/systemexception.cpp -o ./build/system/exceptions/systemexception.o ${OBJECT_FILE_FLAGS}	
./build/system/trynode.o: ./src/system/trynode.cpp
	g++ -I ./include ./src/system/trynode.cpp -o ./build/system/trynode.o ${OBJECT_FILE_FLAGS}	
./build/system/thrownode.o: ./src/system/thrownode.cpp
	g++ -I ./include ./src/system/thrownode.cpp -o ./build/system/thrownode.o ${OBJECT_FILE_FLAGS}	
./build/system/exceptionobject.o: ./src/system/exceptionobject.cpp
	g++ -I ./include ./src/system/exceptionobject.cpp -o ./build/system/exceptionobject.o ${OBJECT_FILE_FLAGS}	
./build/system/whilenode.o: ./src/system/whilenode.cpp
	g++ -I ./include ./src/system/whilenode.cpp -o ./build/system/whilenode.o ${OBJECT_FILE_FLAGS}	
./build/system/breakable.o: ./src/system/breakable.cpp
	g++ -I ./include ./src/system/breakable.cpp -o ./build/system/breakable.o ${OBJECT_FILE_FLAGS}	
./build/system/breaknode.o: ./src/system/breaknode.cpp
	g++ -I ./include ./src/system/breaknode.cpp -o ./build/system/breaknode.o ${OBJECT_FILE_FLAGS}	
./build/system/continuenode.o: ./src/system/continuenode.cpp
	g++ -I ./include ./src/system/continuenode.cpp -o ./build/system/continuenode.o ${OBJECT_FILE_FLAGS}		
./build/system/dowhilenode.o: ./src/system/dowhilenode.cpp
	g++ -I ./include ./src/system/dowhilenode.cpp -o ./build/system/dowhilenode.o ${OBJECT_FILE_FLAGS}
./build/system/fornode.o: ./src/system/fornode.cpp
	g++ -I ./include ./src/system/fornode.cpp -o ./build/system/fornode.o ${OBJECT_FILE_FLAGS}		
./build/system/listnode.o: ./src/system/listnode.cpp
	g++ -I ./include ./src/system/listnode.cpp -o ./build/system/listnode.o ${OBJECT_FILE_FLAGS}
./build/system/includenode.o: ./src/system/includenode.cpp
	g++ -I ./include ./src/system/includenode.cpp -o ./build/system/includenode.o ${OBJECT_FILE_FLAGS}
./build/system/purefunction.o: ./src/system/purefunction.cpp
	g++ -I ./include ./src/system/purefunction.cpp -o ./build/system/purefunction.o ${OBJECT_FILE_FLAGS}
./build/system/includeoncenode.o: ./src/system/includeoncenode.cpp
	g++ -I ./include ./src/system/includeoncenode.cpp -o ./build/system/includeoncenode.o ${OBJECT_FILE_FLAGS}
./build/system/misc.o: ./src/system/misc.cpp
	g++ -I ./include ./src/system/misc.cpp -o ./build/system/misc.o ${OBJECT_FILE_FLAGS}
modules:
	cd ./src/stdmods && $(MAKE) all
	
standalone: system
	cd bin; \
	g++ -g -I ../include ../src/standalone/main.cpp ./${SYSTEM_LIB_LOCAL_FILENAME}  -ldl -std=c++14 -o ./marble;

standalone-modules: standalone modules
	cp -a ./src/stdmods/bin/. ./bin/mods

apache2: system
	sudo cp ${SYSTEM_LIB_FILE_LOCATION} /usr/lib/${SYSTEM_LIB_LOCAL_FILENAME}
	g++ -fPIC -shared -std=c++14 -I /usr/include/apache2 -I /usr/include/apr-1.0 -I ./include ./src/apache/mod_marble.cpp /usr/lib/${SYSTEM_LIB_LOCAL_FILENAME} -o ./bin/mod_marble.so
apache2-install: apache2
	sudo apxs -i -a -n mod_marble ./bin/mod_marble.so
	sudo cp -a ./src/stdmods/bin/. /usr/lib/marble
clean:
	rm ${SYSTEM_OBJECT_FILES}
	cd ./src/stdmods && $(MAKE) clean
