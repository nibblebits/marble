SYSTEM_OBJECT_FILES = ./build/main.o ./build/sessionmod.o ./build/sessionobject.o ./build/filesessionobject.o ./build/sessionvaluesobject.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/sessionmod.o: ./src/sessionmod.cpp
	g++ ${INCLUDES} ./src/sessionmod.cpp -o ./build/sessionmod.o ${OBJECT_FILE_FLAGS}
./build/sessionobject.o: ./src/sessionobject.cpp
	g++ ${INCLUDES} ./src/sessionobject.cpp -o ./build/sessionobject.o ${OBJECT_FILE_FLAGS}
./build/filesessionobject.o: ./src/drivers/filesessionobject.cpp
	g++ ${INCLUDES} ./src/drivers/filesessionobject.cpp -o ./build/filesessionobject.o ${OBJECT_FILE_FLAGS}
./build/sessionvaluesobject.o: ./src/sessionvaluesobject.cpp
	g++ ${INCLUDES} ./src/sessionvaluesobject.cpp -o ./build/sessionvaluesobject.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -ljsoncpp -o ../bin/marble_sessionmod.so 
clean:
	rm -f ${SYSTEM_OBJECT_FILES}
