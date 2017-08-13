#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "scope.h"
class Node;
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
		double evaluate_expression_get_number(Node* node);
		double op_on_values(double value1, double value2, std::string op);
		Scope root_scope;

};

#endif
