SYSTEM_OBJECT_FILES = ./build/system/token.o ./build/system/splitter.o ./build/system/interpreter.o
OBJECT_FILE_FLAGS = -c -g -fPIC
SYSTEM_LIB_FILE_LOCATION = ./bin/system.so
system:
	g++ -I ./include ./src/system/token.cpp -o ./build/system/token.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/splitter.cpp -o ./build/system/splitter.o ${OBJECT_FILE_FLAGS}
	g++ -I ./include ./src/system/interpreter.cpp -o ./build/system/interpreter.o ${OBJECT_FILE_FLAGS}
	ld ${SYSTEM_OBJECT_FILES} -o ${SYSTEM_LIB_FILE_LOCATION}  -r
standalone: system
	g++ -I ./include ./src/standalone/main.cpp ${SYSTEM_LIB_FILE_LOCATION} -o ./bin/marble
web:

clean:
	rm ${SYSTEM_OBJECT_FILES}
