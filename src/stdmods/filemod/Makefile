SYSTEM_OBJECT_FILES = ./build/main.o ./build/filemod.o ./build/filemod_fileoutputstream.o ./build/filemod_fileinputstream.o ./build/filemod_file.o ./build/filemod_directory.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/filemod.o: ./src/filemod.cpp
	g++ ${INCLUDES} ./src/filemod.cpp -o ./build/filemod.o ${OBJECT_FILE_FLAGS}
./build/filemod_fileoutputstream.o: ./src/filemod_fileoutputstream.cpp
	g++ ${INCLUDES} ./src/filemod_fileoutputstream.cpp -o ./build/filemod_fileoutputstream.o ${OBJECT_FILE_FLAGS}
./build/filemod_fileinputstream.o: ./src/filemod_fileinputstream.cpp
	g++ ${INCLUDES} ./src/filemod_fileinputstream.cpp -o ./build/filemod_fileinputstream.o ${OBJECT_FILE_FLAGS}
./build/filemod_file.o: ./src/filemod_file.cpp
	g++ ${INCLUDES} ./src/filemod_file.cpp -o ./build/filemod_file.o ${OBJECT_FILE_FLAGS}
./build/filemod_directory.o: ./src/filemod_directory.cpp
	g++ ${INCLUDES} ./src/filemod_directory.cpp -o ./build/filemod_directory.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../bin/marble_filemod.so
clean:
	rm -f ${SYSTEM_OBJECT_FILES}
