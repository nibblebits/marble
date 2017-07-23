#ifndef TOKEN_H
#define TOKEN_H
#include <string>
class Token
{
    public:
        Token(int type);
        virtual ~Token();
        int type;
		
		union
		{
			int ivalue;
			double dvalue;
			const char* svalue;
		};
};
#endif
