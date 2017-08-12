#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
class Variable
{
public:
	Variable();
	virtual ~Variable();
	std::string name;
	int type;
	union
	{
		double dvalue;
    	void* ovalue;
		void* svalue;
	};
};
#endif
