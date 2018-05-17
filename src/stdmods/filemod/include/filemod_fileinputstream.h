#ifndef FILEMOD_FILEINPUTSTREAM_H
#define FILEMOD_FILEINPUTSTREAM_H
#include "module.h"
#include "../../../commonmod/include/commonmod.h"
#include <memory>

class FileModule_File;
class FileModule_FileInputStream : public CommonModule_InputStream
{
public:
    FileModule_FileInputStream(Class* c);
    virtual ~FileModule_FileInputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);
    std::shared_ptr<FileModule_File> file;

    // Native FileOutputStream methods

    static void FileInputStream_Fill(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object);
};

#endif