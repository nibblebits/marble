#include <memory.h>

#include "splitter.h"
#include "statics.h"

Splitter::Splitter()
{
	this->data = 0;
	this->previous = 0;
}

Splitter::~Splitter()
{

}


void Splitter::setData(const char* data, int length)
{
	this->data = data;
	this->length = length;
}

bool Splitter::isSafeRange(int position)
{
	return position >= 0 && position < this->length;
}

int Splitter::findPositionOfMemoryInData(int position, const char* memory, int memory_size)
{
	for (int i = position; i < this->length; i++)
	{
		if (!isSafeRange(i))
		{
			break;
		}

		if(memcmp((const char*) (this->data+i), memory, memory_size) == 0)
		{
			return i;
		}
	}

	return -1;
}

int Splitter::getPositionOfNextMarbleTag(int current_pos)
{
	return findPositionOfMemoryInData(current_pos, MARBLE_OPEN_TAG, strlen(MARBLE_OPEN_TAG));
}

int Splitter::getPositionOfNextMarbleClosingTag(int current_pos)
{
	return findPositionOfMemoryInData(current_pos, MARBLE_CLOSING_TAG, strlen(MARBLE_CLOSING_TAG));
}

bool Splitter::split(struct split* split)
{
	struct marble_code* marble_code = &split->code;
	int start = 0;
	if (this->previous != 0)
	{
		start = this->previous->end;
	}
	
	// Some defaults. 
	split->has_code = true;
	split->is_last = false;

	// Lets check if we have marble code
	int next_marble_code_start_tag_pos = getPositionOfNextMarbleTag(start);
	int next_marble_code_pos = next_marble_code_start_tag_pos + strlen(MARBLE_OPEN_TAG);
	if (next_marble_code_start_tag_pos == -1)
	{
		// No marble tag was found.
		split->is_last = true;
		split->has_code = false;
		return false;
	}

    int next_marble_code_closing_tag_pos = getPositionOfNextMarbleClosingTag(next_marble_code_pos);
	int next_marble_code_pos_end = next_marble_code_closing_tag_pos;

	marble_code->start = next_marble_code_pos;

	// If we do not have a closing tag or the end of the marble code is the end of the document then we are done
	if (next_marble_code_closing_tag_pos == -1 || next_marble_code_pos_end == this->length)
	{
    	marble_code->end = this->length;
	}
	else
	{
		marble_code->end = next_marble_code_pos_end;

		// Is this the last marble tag?
		if(getPositionOfNextMarbleTag(next_marble_code_pos_end) == -1)
		{
			// OK this is the last marble tag
			split->is_last = true;
		}

	}

	marble_code->data = this->data+marble_code->start;
	marble_code->size = marble_code->end-marble_code->start;

	this->previous = marble_code;
	return true;
}
