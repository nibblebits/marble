SYSTEM_OBJECT_FILES = ./build/main.o ./build/mysqlmod.o ./build/mysqldriver.o ./build/mysqlconnection.o
INCLUDES = -I ../../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14 `mysql_config --cflags --libs`
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/mysqlmod.o: ./src/mysqlmod.cpp
	g++ ${INCLUDES} ./src/mysqlmod.cpp -o ./build/mysqlmod.o ${OBJECT_FILE_FLAGS}
./build/mysqldriver.o: ./src/mysqldriver.cpp
	g++ ${INCLUDES} ./src/mysqldriver.cpp -o ./build/mysqldriver.o ${OBJECT_FILE_FLAGS}
./build/mysqlconnection.o: ./src/mysqlconnection.cpp
	g++ ${INCLUDES} ./src/mysqlconnection.cpp -o ./build/mysqlconnection.o  ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14 -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -o ../../bin/marble_mysqlmod.so `mysql_config --cflags --libs`
clean:
	rm ${SYSTEM_OBJECT_FILES}
