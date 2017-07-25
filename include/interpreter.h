#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter
{
	public:
		Interpreter();
		virtual ~Interpreter();
		void output(const char* data);
		void run(const char* code);
		void runScript(const char* filename);

};

#endif
