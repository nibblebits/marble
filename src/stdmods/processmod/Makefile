SYSTEM_OBJECT_FILES = ./build/main.o ./build/processmod.o ./build/shellobject.o ./build/shellexecutionobject.o ./build/shellpermission.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/processmod.o: ./src/processmod.cpp
	g++ ${INCLUDES} ./src/processmod.cpp -o ./build/processmod.o ${OBJECT_FILE_FLAGS}
./build/shellobject.o: ./src/shellobject.cpp
	g++ ${INCLUDES} ./src/shellobject.cpp -o ./build/shellobject.o ${OBJECT_FILE_FLAGS}
./build/shellexecutionobject.o: ./src/shellexecutionobject.cpp
	g++ ${INCLUDES} ./src/shellexecutionobject.cpp -o ./build/shellexecutionobject.o ${OBJECT_FILE_FLAGS}
./build/shellpermission.o: ./src/shellpermission.cpp
	g++ ${INCLUDES} ./src/shellpermission.cpp -o ./build/shellpermission.o ${OBJECT_FILE_FLAGS}

standalone: module
	g++ -fPIC -shared -std=c++14  -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../bin/marble_processmod.so
clean:
	rm ${SYSTEM_OBJECT_FILES}
