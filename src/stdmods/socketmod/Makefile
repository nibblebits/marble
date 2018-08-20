SYSTEM_OBJECT_FILES = ./build/main.o ./build/socketmod.o ./build/socketobject.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/socketmod.o: ./src/socketmod.cpp
	g++ ${INCLUDES} ./src/socketmod.cpp -o ./build/socketmod.o ${OBJECT_FILE_FLAGS}
./build/socketobject.o: ./src/socketobject.cpp
	g++ ${INCLUDES} ./src/socketobject.cpp -o ./build/socketobject.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../bin/marble_socketmod.so
clean:
	rm ${SYSTEM_OBJECT_FILES}
