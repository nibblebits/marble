class Lexer
{
    public:
        void setInput(const char* buf, int size);
        void lex();
    private:
        char* buf;
}
