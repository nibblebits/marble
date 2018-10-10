SYSTEM_OBJECT_FILES = ./build/main.o ./build/encodermod.o ./build/encoderobject.o ./build/base64.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/encodermod.o: ./src/encodermod.cpp
	g++ ${INCLUDES} ./src/encodermod.cpp -o ./build/encodermod.o ${OBJECT_FILE_FLAGS}
./build/encoderobject.o: ./src/encoderobject.cpp
	g++ ${INCLUDES} ./src/encoderobject.cpp -o ./build/encoderobject.o ${OBJECT_FILE_FLAGS}
./build/base64.o: ./src/base64/base64.cpp
	g++ ${INCLUDES} -I ./include/base64 ./src/base64/base64.cpp -o ./build/base64.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../bin/marble_encodermod.so
clean:
	rm -f ${SYSTEM_OBJECT_FILES}
