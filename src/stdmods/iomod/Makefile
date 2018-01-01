SYSTEM_OBJECT_FILES = ./build/main.o ./build/iomod.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
	cp $(SYSTEM_LIB_FILE_LOCATION) ./libmarble.so
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/iomod.o: ./src/iomod.cpp
	g++ ${INCLUDES} ./src/iomod.cpp -o ./build/iomod.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 ${INCLUDES} ./src/main.cpp ./libmarble.so -o ../bin/marble_iomod.so
clean:
	rm ${SYSTEM_OBJECT_FILES}
