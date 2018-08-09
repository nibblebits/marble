SYSTEM_OBJECT_FILES = ./build/main.o ./build/commonmod.o ./build/commonmod_system.o ./build/commonmod_outputstream.o ./build/commonmod_inputstream.o ./build/commonmod_stringutils.o ./build/commonmod_sqldriver.o ./build/commonmod_sqlstatement.o ./build/commonmod_preparedstatement.o ./build/commonmod_sqlconnection.o ./build/commonmod_sqlresult.o ./build/commonmod_sqlrecord.o ./build/commonmod_list.o ./build/commonmod_value.o
INCLUDES = -I ../../include -I ./include
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14
SYSTEM_LIB_LOCAL_FILENAME = libmarble.so
SYSTEM_LIB_FILE_LOCATION = ../../../bin/${SYSTEM_LIB_LOCAL_FILENAME}

module: ${SYSTEM_OBJECT_FILES}
./build/main.o: ./src/main.cpp
	g++ ${INCLUDES} ./src/main.cpp -o ./build/main.o ${OBJECT_FILE_FLAGS}
./build/commonmod.o: ./src/commonmod.cpp
	g++ ${INCLUDES} ./src/commonmod.cpp -o ./build/commonmod.o ${OBJECT_FILE_FLAGS}
./build/commonmod_outputstream.o: ./src/commonmod_outputstream.cpp
	g++ ${INCLUDES} ./src/commonmod_outputstream.cpp -o ./build/commonmod_outputstream.o ${OBJECT_FILE_FLAGS}
./build/commonmod_stringutils.o: ./src/commonmod_stringutils.cpp
	g++ ${INCLUDES} ./src/commonmod_stringutils.cpp -o ./build/commonmod_stringutils.o ${OBJECT_FILE_FLAGS}
./build/commonmod_inputstream.o: ./src/commonmod_inputstream.cpp
	g++ ${INCLUDES} ./src/commonmod_inputstream.cpp -o ./build/commonmod_inputstream.o ${OBJECT_FILE_FLAGS}
./build/commonmod_sqldriver.o: ./src/sql/commonmod_sqldriver.cpp
	g++ ${INCLUDES} ./src/sql/commonmod_sqldriver.cpp -o ./build/commonmod_sqldriver.o ${OBJECT_FILE_FLAGS}
./build/commonmod_sqlstatement.o: ./src/sql/commonmod_sqlstatement.cpp
	g++ ${INCLUDES} ./src/sql/commonmod_sqlstatement.cpp -o ./build/commonmod_sqlstatement.o ${OBJECT_FILE_FLAGS}
./build/commonmod_preparedstatement.o: ./src/sql/commonmod_preparedstatement.cpp
	g++ ${INCLUDES} ./src/sql/commonmod_preparedstatement.cpp -o ./build/commonmod_preparedstatement.o ${OBJECT_FILE_FLAGS}
./build/commonmod_sqlconnection.o: ./src/sql/commonmod_sqlconnection.cpp
	g++ ${INCLUDES} ./src/sql/commonmod_sqlconnection.cpp -o ./build/commonmod_sqlconnection.o ${OBJECT_FILE_FLAGS}
./build/commonmod_sqlresult.o: ./src/sql/commonmod_sqlresult.cpp
	g++ ${INCLUDES} ./src/sql/commonmod_sqlresult.cpp -o ./build/commonmod_sqlresult.o ${OBJECT_FILE_FLAGS}
./build/commonmod_sqlrecord.o: ./src/sql/commonmod_sqlrecord.cpp
	g++ ${INCLUDES} ./src/sql/commonmod_sqlrecord.cpp -o ./build/commonmod_sqlrecord.o ${OBJECT_FILE_FLAGS}
./build/commonmod_system.o: ./src/commonmod_system.cpp
	g++ ${INCLUDES} ./src/commonmod_system.cpp -o ./build/commonmod_system.o ${OBJECT_FILE_FLAGS}
./build/commonmod_list.o: ./src/storage/commonmod_list.cpp
	g++ ${INCLUDES} ./src/storage/commonmod_list.cpp -o ./build/commonmod_list.o ${OBJECT_FILE_FLAGS}
./build/commonmod_value.o: ./src/storage/commonmod_value.cpp
	g++ ${INCLUDES} ./src/storage/commonmod_value.cpp -o ./build/commonmod_value.o ${OBJECT_FILE_FLAGS}
standalone: module
	ld -r ${SYSTEM_OBJECT_FILES} -o ./bin/marble_commonmod.o
clean:
	rm ${SYSTEM_OBJECT_FILES}
