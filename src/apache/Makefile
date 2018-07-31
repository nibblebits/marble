SYSTEM_OBJECT_FILES = ./build/webmod.o ./build/responseobject.o ./build/cookiepermission.o ./build/requestargumentsobject.o ./build/multipartfileobject.o
INCLUDES = -I ../../include -I ./include -I /usr/include/apr-1.0 -I /usr/include/apache2
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14 -g
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

all: ${SYSTEM_OBJECT_FILES}
./build/webmod.o: ./src/webmod.cpp
	g++ ${INCLUDES} ./src/webmod.cpp -o ./build/webmod.o ${OBJECT_FILE_FLAGS}

./build/responseobject.o: ./src/responseobject.cpp
	g++ ${INCLUDES} ./src/responseobject.cpp -o ./build/responseobject.o ${OBJECT_FILE_FLAGS}


./build/cookiepermission.o: ./src/cookiepermission.cpp
	g++ ${INCLUDES} ./src/cookiepermission.cpp -o ./build/cookiepermission.o ${OBJECT_FILE_FLAGS}


./build/requestargumentsobject.o: ./src/requestargumentsobject.cpp
	g++ ${INCLUDES} ./src/requestargumentsobject.cpp -o ./build/requestargumentsobject.o ${OBJECT_FILE_FLAGS}


./build/multipartfileobject.o: ./src/multipartfileobject.cpp
	g++ ${INCLUDES} ./src/multipartfileobject.cpp -o ./build/multipartfileobject.o ${OBJECT_FILE_FLAGS}

clean:
	rm ${SYSTEM_OBJECT_FILES}
