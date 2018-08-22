SYSTEM_OBJECT_FILES = ./build/main.o ./build/networkmod.o ./build/socketobject.o ./build/internetprotocolobject.o ./build/ipv4addressobject.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/networkmod.o: ./src/networkmod.cpp
	g++ ${INCLUDES} ./src/networkmod.cpp -o ./build/networkmod.o ${OBJECT_FILE_FLAGS}
./build/socketobject.o: ./src/socketobject.cpp
	g++ ${INCLUDES} ./src/socketobject.cpp -o ./build/socketobject.o ${OBJECT_FILE_FLAGS}
./build/internetprotocolobject.o: ./src/internetprotocolobject.cpp
	g++ ${INCLUDES} ./src/internetprotocolobject.cpp -o ./build/internetprotocolobject.o ${OBJECT_FILE_FLAGS}
./build/ipv4addressobject.o: ./src/ipv4addressobject.cpp
	g++ ${INCLUDES} ./src/ipv4addressobject.cpp -o ./build/ipv4addressobject.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../bin/marble_networkmod.so
clean:
	rm ${SYSTEM_OBJECT_FILES}
