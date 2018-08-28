SYSTEM_OBJECT_FILES = ./build/main.o ./build/cryptomod.o ./build/cryptographyobject.o ./build/sha256.o ./build/sha1.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/cryptomod.o: ./src/cryptomod.cpp
	g++ ${INCLUDES} ./src/cryptomod.cpp -o ./build/cryptomod.o ${OBJECT_FILE_FLAGS}
./build/cryptographyobject.o: ./src/cryptographyobject.cpp
	g++ ${INCLUDES} ./src/cryptographyobject.cpp -o ./build/cryptographyobject.o ${OBJECT_FILE_FLAGS}
./build/sha256.o: ./src/dep/sha256.cpp
	g++ ${INCLUDES} ./src/dep/sha256.cpp -o ./build/sha256.o ${OBJECT_FILE_FLAGS}
./build/sha1.o: ./src/dep/sha1.cpp
	g++ ${INCLUDES} ./src/dep/sha1.cpp -o ./build/sha1.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14  -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -ljsoncpp -o ../bin/marble_cryptomod.so
clean:
	rm ${SYSTEM_OBJECT_FILES}
