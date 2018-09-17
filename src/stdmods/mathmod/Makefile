SYSTEM_OBJECT_FILES = ./build/main.o ./build/mathmod.o ./build/mathobject.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/mathmod.o: ./src/mathmod.cpp
	g++ ${INCLUDES} ./src/mathmod.cpp -o ./build/mathmod.o ${OBJECT_FILE_FLAGS}
./build/mathobject.o: ./src/mathobject.cpp
	g++ ${INCLUDES} ./src/mathobject.cpp -o ./build/mathobject.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../bin/marble_mathmod.so
clean:
	rm -f ${SYSTEM_OBJECT_FILES}
