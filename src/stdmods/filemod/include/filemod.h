#ifndef FILEMODULE_H
#define FILEMODULE_H
#include "module.h"
#include "../../../commonmod/include/commonmod.h"
#include "filemod_file.h"
#include "filemod_fileoutputstream.h"
#include "filemod_fileinputstream.h"
#include "filemod_directory.h"
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

    /**
    * Returns if the given mode is a writeable mode
    */
    static bool isWriteMode(std::string mode);
    /**
     * Returns if the given mode is a readable mode
     */
    static bool isReadMode(std::string mode);
    

private:

};
#endif