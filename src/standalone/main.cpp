#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "token.h"
#include "splitter.h"

int main(int argc, char** argv) {

   const char* data = "<html><title>Dans amazing web page</title><marble>Date date = new Date();print TOdays data is + date;</marble>We hope you enjoy our website";
   Splitter splitter;
   splitter.setData(data, strlen(data));
   
   split split;
   while(splitter.split(&split))
   {
	char* code_data = new char[split.code.size+1];
	memcpy(code_data, split.code.data, split.code.size);
	code_data[split.code.size] = 0;
	printf("%s", code_data);
   }

}
