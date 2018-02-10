SYSTEM_OBJECT_FILES = ./build/webmod.o
INCLUDES = -I ../../include -I ./include -I /usr/include/apr-1.0 -I /usr/include/apache2
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14 -g
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

all: ${SYSTEM_OBJECT_FILES}
./build/webmod.o: ./src/webmod.cpp
	g++ ${INCLUDES} ./src/webmod.cpp -o ./build/webmod.o ${OBJECT_FILE_FLAGS}
clean:
	rm ${SYSTEM_OBJECT_FILES}
