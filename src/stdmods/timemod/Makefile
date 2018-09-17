SYSTEM_OBJECT_FILES = ./build/main.o ./build/timemod.o ./build/timeobject.o ./build/dateobject.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/timemod.o: ./src/timemod.cpp
	g++ ${INCLUDES} ./src/timemod.cpp -o ./build/timemod.o ${OBJECT_FILE_FLAGS}
./build/timeobject.o: ./src/timeobject.cpp
	g++ ${INCLUDES} ./src/timeobject.cpp -o ./build/timeobject.o ${OBJECT_FILE_FLAGS}
./build/dateobject.o: ./src/dateobject.cpp
	g++ ${INCLUDES} ./src/dateobject.cpp -o ./build/dateobject.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../bin/marble_timemod.so
clean:
	rm -f ${SYSTEM_OBJECT_FILES}
