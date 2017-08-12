#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "scope.h"

class VarNode;
class Interpreter
{
	public:
		Interpreter();
		virtual ~Interpreter();
		void output(const char* data);
		void run(const char* code);
		void runScript(const char* filename);
	private:
		void interpret_variable_node(VarNode* var_node);

		Scope root_scope;

};

#endif
