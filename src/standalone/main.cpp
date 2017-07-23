#include <stdio.h>
#include "token.h"

int main(int argc, char** argv) {
    Token token(500);
	token.ivalue = 90;
    printf("%s, %i", "hello world", token.type);
}
