#ifndef POSINFO_H
#define POSINFO_H
class PosInfo
{
public:
    PosInfo();
    PosInfo(int line, int col, const char* filename);
    virtual ~PosInfo();
    int line;
    int col;
    const char* filename;
};
#endif
