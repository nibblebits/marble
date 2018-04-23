#include <memory.h>
#include <stdexcept>

#include "splitter.h"
#include "statics.h"
#include "logger.h"
data_descriptor::data_descriptor()
{
    this->start = 0;
    this->end = 0;
    this->data = 0;
    this->size = 0;
}
void data_descriptor::update()
{
    this->size = this->end - this->start;
}

split::split()
{
    this->output.split = this;
    this->code.split = this;
    this->has_data = false;
    this->has_code = false;
    this->is_last = false;
}

void output_data::update()
{
    data_descriptor::update();
    this->split->has_data = this->size != 0;
}

void marble_code::update()
{
    data_descriptor::update();
    this->split->has_code = this->size != 0;
}

Splitter::Splitter()
{

}

Splitter::Splitter(Logger* logger, const char* filename)
{
    this->logger = logger;
    this->data = 0;
    this->previous = 0;
    this->filename = filename;
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
    // Nothing to do here if this split is already the last split.
    if (split->is_last)
    {
        return false;
    }

    struct marble_code* marble_code = &split->code;
    struct output_data* output_data = &split->output;

    split->has_data = false;
    split->has_code = false;
    int start = 0;
    if (this->previous != 0)
    {
        // At the end of the marble code will be a marble closing tag.
        start = this->previous->end + strlen(MARBLE_CLOSING_TAG);
    }

    int pos_to_next_marble_tag = getPositionOfNextMarbleTag(start);
    if (pos_to_next_marble_tag == -1)
    {
        // There is no more marble tags
        output_data->start = start;
        output_data->end = this->length;
        output_data->data = this->data+start;
        output_data->update();
        split->is_last = true;
        return true;
    }

    // We have a marble tag lets deal with it
    int pos_to_marble_code = pos_to_next_marble_tag + strlen(MARBLE_OPEN_TAG);
    int pos_to_marble_closing_tag = getPositionOfNextMarbleClosingTag(pos_to_marble_code);
    if (pos_to_marble_closing_tag == -1)
    {
        PosInfo posInfo;
        posInfo.filename = this->filename;
        logger->error("A Marble tag was opened but never closed", posInfo);
    }
    marble_code->start = pos_to_marble_code;
    marble_code->end = pos_to_marble_closing_tag;
    marble_code->data = this->data+marble_code->start;
    marble_code->update();

    if (pos_to_next_marble_tag != start)
    {
        // We have standard output data lets deal with it
        int pos_of_data = start;
        output_data->start = pos_of_data;
        output_data->end = pos_to_next_marble_tag;
        output_data->data = this->data+output_data->start;
        output_data->update();
    }
    this->previous = marble_code;
    return true;
}

void Splitter::free()
{
    if (this->data != NULL)
        delete this->data;
}