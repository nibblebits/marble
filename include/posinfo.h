#ifndef POSINFO_H
#define POSINFO_H
#include <string>
class PosInfo
{
public:
    PosInfo();
    PosInfo(int line, int col, std::string filename);
    virtual ~PosInfo();
    int line;
    int col;
    std::string filename;
};
#endif
