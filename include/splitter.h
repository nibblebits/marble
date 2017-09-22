#ifndef SPLITTER_H
#define SPLITTER_H

struct split;
struct data_descriptor
{
    int start;
    int end;
    int size;
    const char* data;
    struct split* split;
    
    data_descriptor();
    virtual void update();
};

struct output_data : public data_descriptor
{
    void update();
};

struct marble_code : public data_descriptor
{
    void update();
};

struct split
{
    split();
    struct output_data output;
    struct marble_code code;
    bool has_code;
    bool has_data;
    bool is_last;
};

class Splitter
{
public:
    Splitter();
    virtual ~Splitter();
    void setData(const char* data, int length);
    bool split(struct split* marble_code);
private:
    bool isSafeRange(int position);
    int findPositionOfMemoryInData(int position, const char* memory, int memory_size);
    int getPositionOfNextMarbleTag(int position);
    int getPositionOfNextMarbleClosingTag(int current_pos);
    const char* data;
    int length;
    struct marble_code* previous;
};
#endif
