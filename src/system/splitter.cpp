#include "splitter.h"
#include "statics.h"
Splitter::Splitter()
{
	this->data = 0;
	this->previous = 0;
}
virtual Splitter::~Splitter();


void Splitter::setData(char* data, int length)
{
	this->data = data;
	this->length = length;
}

int Splitter::getPositionOfNextMarbleTag(int current_pos)
{
	for (int i = current_pos; i < this->length; i++)
	{
		if(strcmp((const char*) this->data, MARBLE_OPEN_TAG) == 0)
		{
			return i;
		}
	}
}

struct marble_code Splitter::split()
{
	int start = 0;
	if (this->previous != 0)
	{
		start = this->previous.end;
	}
	
	int current_pos = getPositionOfNextMarbleTag(start);
	
	struct marble_code marble_code;
	marble_code.start = current_pos;
	return marble_code;
}