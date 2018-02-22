SYSTEM_OBJECT_FILES = ./build/main.o ./build/commonmod.o ./build/commonmod_outputstream.o
INCLUDES = -I ../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/commonmod.o: ./src/commonmod.cpp
	g++ ${INCLUDES} ./src/commonmod.cpp -o ./build/commonmod.o ${OBJECT_FILE_FLAGS}
./build/commonmod_outputstream.o: ./src/commonmod_outputstream.cpp
	g++ ${INCLUDES} ./src/commonmod_outputstream.cpp -o ./build/commonmod_outputstream.o ${OBJECT_FILE_FLAGS}
standalone: module
	ld -r ${SYSTEM_OBJECT_FILES} -o ./bin/marble_commonmod.o
clean:
	rm ${SYSTEM_OBJECT_FILES}
