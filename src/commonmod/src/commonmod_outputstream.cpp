#include "commonmod_outputstream.h"
CommonModule_OutputStream::CommonModule_OutputStream(Class* c) : Object(c)
{
  
}

CommonModule_OutputStream::~CommonModule_OutputStream()
{

}

std::shared_ptr<Object> CommonModule_OutputStream::newInstance(Class* c)
{
    return std::make_shared<CommonModule_OutputStream>(c);
}
