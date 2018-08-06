#include "posinfo.h"
PosInfo::PosInfo() : PosInfo(-1, -1, "")
{
   
}
PosInfo::PosInfo(int line, int col, std::string filename)
{
    this->line = line;
    this->col = col;
    this->filename = filename;
}

PosInfo::~PosInfo()
{

}
