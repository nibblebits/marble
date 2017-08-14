#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "scope.h"
class Node;
class VarNode;
class ExpNode;
class Interpreter
{
	public:
		Interpreter();
		virtual ~Interpreter();
		void output(const char* data);
		void run(const char* code);
		void runScript(const char* filename);
	private:
		void interpret_assignment_expression(ExpNode* exp_node);
		void interpret_expression(ExpNode* exp_node);
		void interpret_variable_node_for_primitive(VarNode* var_node);
		void interpret_variable_node(VarNode* var_node);
		void fail();
		double evaluate_expression_get_number(Node* node);
		double op_on_values(double value1, double value2, std::string op);
		Scope root_scope;

};

#endif
