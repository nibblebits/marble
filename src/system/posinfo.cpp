#include "posinfo.h"
PosInfo::PosInfo() : PosInfo(-1, -1, "")
{
   
}
PosInfo::PosInfo(int line, int col, const char* filename)
{
    this->line = line;
    this->col = col;
    this->filename = filename;
}

PosInfo::~PosInfo()
{

}
