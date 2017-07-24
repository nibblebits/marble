#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "token.h"
#include "splitter.h"

int main(int argc, char** argv) {

   const char* data = "<html><title>Dans amazing web page</title><marble>Date date = new Date();print TOdays data is + date;</marble>We hope you enjoy our website<marble>test</marble>";
   Splitter splitter;
   splitter.setData(data, strlen(data));
   
   int last_data_pos = 0;
   marble_code marble_code;
   while(splitter.split(&marble_code))
   {
	int size = marble_code.start - last_data_pos;
	char* code_data = new char[size+1];
	memcpy(code_data, data+last_data_pos, size);
	code_data[size] = 0;
	printf("%s", code_data);
	last_data_pos = marble_code.end;
   }

}
