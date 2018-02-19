SYSTEM_OBJECT_FILES = ./build/main.o ./build/commonmod.o
INCLUDES = -I ../../../include -I ./include
LINK = -L ../bin/marble_commonmod.so
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14 -g
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/commonmod.o: ./src/commonmod.cpp
	g++ ${INCLUDES} ./src/commonmod.cpp -o ./build/commonmod.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -ldl -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${LINK} ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../bin/marble_commonmod.so
clean:
	rm ${SYSTEM_OBJECT_FILES}
