SYSTEM_OBJECT_FILES = ./build/main.o ./build/networkmod.o ./build/socketobject.o ./build/udpsocketobject.o ./build/tcpsocketserverobject.o ./build/socketaddressobject.o ./build/tcpsocketobject.o ./build/internetprotocolobject.o ./build/ipv4addressobject.o ./build/curlobject.o ./build/networkpermission.o ./build/mailobject.o ./build/smtpmailobject.o ./build/sendmailobject.o
INCLUDES = -I ../../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/networkmod.o: ./src/networkmod.cpp
	g++ ${INCLUDES} ./src/networkmod.cpp -o ./build/networkmod.o ${OBJECT_FILE_FLAGS}
./build/socketobject.o: ./src/socketobject.cpp
	g++ ${INCLUDES} ./src/socketobject.cpp -o ./build/socketobject.o ${OBJECT_FILE_FLAGS}

./build/udpsocketobject.o: ./src/udpsocketobject.cpp
	g++ ${INCLUDES} ./src/udpsocketobject.cpp -o ./build/udpsocketobject.o ${OBJECT_FILE_FLAGS}
./build/tcpsocketserverobject.o: ./src/tcpsocketserverobject.cpp
	g++ ${INCLUDES} ./src/tcpsocketserverobject.cpp -o ./build/tcpsocketserverobject.o ${OBJECT_FILE_FLAGS}
./build/tcpsocketobject.o: ./src/tcpsocketobject.cpp
	g++ ${INCLUDES} ./src/tcpsocketobject.cpp -o ./build/tcpsocketobject.o ${OBJECT_FILE_FLAGS}
./build/socketaddressobject.o: ./src/socketaddressobject.cpp
	g++ ${INCLUDES} ./src/socketaddressobject.cpp -o ./build/socketaddressobject.o ${OBJECT_FILE_FLAGS}

./build/internetprotocolobject.o: ./src/internetprotocolobject.cpp
	g++ ${INCLUDES} ./src/internetprotocolobject.cpp -o ./build/internetprotocolobject.o ${OBJECT_FILE_FLAGS}
./build/ipv4addressobject.o: ./src/ipv4addressobject.cpp
	g++ ${INCLUDES} ./src/ipv4addressobject.cpp -o ./build/ipv4addressobject.o ${OBJECT_FILE_FLAGS}
./build/curlobject.o: ./src/curlobject.cpp
	g++ ${INCLUDES} ./src/curlobject.cpp -o ./build/curlobject.o ${OBJECT_FILE_FLAGS}
./build/networkpermission.o: ./src/networkpermission.cpp
	g++ ${INCLUDES} ./src/networkpermission.cpp -o ./build/networkpermission.o ${OBJECT_FILE_FLAGS}
./build/mailobject.o: ./src/mail/mailobject.cpp
	g++ ${INCLUDES} ./src/mail/mailobject.cpp -o ./build/mailobject.o ${OBJECT_FILE_FLAGS}
./build/smtpmailobject.o: ./src/mail/smtpmailobject.cpp
	g++ ${INCLUDES} ./src/mail/smtpmailobject.cpp -o ./build/smtpmailobject.o ${OBJECT_FILE_FLAGS}
./build/sendmailobject.o: ./src/mail/sendmailobject.cpp
	g++ ${INCLUDES} ./src/mail/sendmailobject.cpp -o ./build/sendmailobject.o ${OBJECT_FILE_FLAGS}
standalone: module
	g++ -fPIC -shared -std=c++14  -unresolved-symbols=ignore-in-shared-libs ${INCLUDES} ${SYSTEM_OBJECT_FILES} -lcurl -o ../bin/marble_networkmod.so
clean:
	rm -f ${SYSTEM_OBJECT_FILES}
