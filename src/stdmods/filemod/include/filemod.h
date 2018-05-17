#ifndef FILEMODULE_H
#define FILEMODULE_H
#include "module.h"
#include "../../../commonmod/include/commonmod.h"
#include "filemod_file.h"
#include "filemod_fileoutputstream.h"
#include "filemod_fileinputstream.h"
#include "object.h"
#include <stdio.h>
#include <string>
#include <memory>



class FileModule : public Module
{
public:
    FileModule();
    virtual ~FileModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:

};
#endif