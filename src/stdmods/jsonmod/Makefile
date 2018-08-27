SYSTEM_OBJECT_FILES = ./build/main.o ./build/jsonmod.o ./build/jsonobject.o ./build/jsonvaluesobject.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/jsonmod.o: ./src/jsonmod.cpp
	g++ ${INCLUDES} ./src/jsonmod.cpp -o ./build/jsonmod.o ${OBJECT_FILE_FLAGS}
./build/jsonobject.o: ./src/jsonobject.cpp
	g++ ${INCLUDES} ./src/jsonobject.cpp -o ./build/jsonobject.o ${OBJECT_FILE_FLAGS}
./build/jsonvaluesobject.o: ./src/jsonvaluesobject.cpp
	g++ ${INCLUDES} ./src/jsonvaluesobject.cpp -o ./build/jsonvaluesobject.o ${OBJECT_FILE_FLAGS}

standalone: module
	g++ -fPIC -shared -std=c++14  -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -ljsoncpp -o ../bin/marble_jsonmod.so
clean:
	rm ${SYSTEM_OBJECT_FILES}
