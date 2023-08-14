%option noyywrap
%option yylineno
%{
    #define YY_NO_UNPUT
    #define YY_NO_INPUT

    #include "parser.h"
    #include <ostream>
    #include <fstream>
    #include<iostream>
    using namespace std;

    extern FILE *yyin; 
    extern FILE *yyout;
    extern bool dump_tokens;

    void DEBUG_FOR_LAB4(string s)
    {
        string DEBUG_INFO = "[DEBUG LAB4]: \t" + s + "\n";
        fputs(DEBUG_INFO.c_str(), yyout);
    }
%}

DECIMIAL ([1-9][0-9]*|0)
OCTAL (0[0-7]+)
HEXADECIMAL (0[xX][0-9A-Fa-f]+) 
DECIMIALFLOAT  ((([0-9]*[.][0-9]*([eE][+-]?[0-9]+)?)|([0-9]+[eE][+-]?[0-9]+))[fLlL]?)
OCTALFLOAT     (0[0-7]+)(\.[0-7]+)?([Pp][+-]?[0-7]+)?
HEXADECIMALFLOAT  (0[xX](([0-9A-Fa-f]*[.][0-9A-Fa-f]*([pP][+-]?[0-9]+)?)|([0-9A-Fa-f]+[pP][+-]?[0-9]+))[fLlL]?)


ID [[:alpha:]_][[:alpha:][:digit:]_]*
EOL (\r\n|\n|\r)
WHITE [\t ]

COMMENTBEGIN   \/\*                        
COMMENTELEMENT .|\n                         
COMMENTEND \*\/
SINGLECOMMENT \/\/[^\n]* 

%x BLOCKCOMMENT



%%

"int" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("INT\tint");
    return INT;
}
"float" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("FLOAT\tfloat");
    return FLOAT;
}
"void" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("VOID\tvoid");
    return VOID;
}
"const" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("CONST\tconst");
    return CONST;
}
"if" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("IF\tif");
    return IF;
};
"else" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("ELSE\telse");
    return ELSE;
};
"return" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("RETURN\treturn");
    return RETURN;
}
"while" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("WHILE\twhile");
    return WHILE;
}
"break" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("BREAK\tbreak");
    return BREAK;
}
"continue" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("CONTINUE\tcontinue");
    return CONTINUE;
}
"&&" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("AND\t=");
    return AND;
}
"||" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("OR\t||");
    return OR;
}
"!" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("NOT\t!");
    return NOT;
}
"=" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("ASSIGN\t=");
    return ASSIGN;
}
"<" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("LESS\t<");
    return LESS;
}
">" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("GREAT\t>");
    return GREAT;
}
"<=" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("LE\t<=");
    return LE;
}
">=" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("GE\t>=");
    return GE;
}
"==" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("EQ\t==");
    return EQ;
}
"!=" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("NE\t!=");
    return NE;
}
"+" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("ADD\t+");
    return ADD;
}
"-" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("SUB\t-");
    return SUB;
}
"*" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("MUL\t*");
    return MUL;
}
"/" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("DIV\t/");
    return DIV;
}
"%" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("MOD\t%");
    return MOD;
}
";" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("SEMICOLON\t;");
    return SEMICOLON;
}
"," {
    if(dump_tokens)
        DEBUG_FOR_LAB4("COMM\t,");
    return COMM;
}
"(" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("LPAREN\t(");
    return LPAREN;
}
")" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("RPAREN\t)");
    return RPAREN;
}
"[" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("LBRACK\t[");
    return LBRACK;
}
"]" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("RBRACK\t]");
    return RBRACK;
}
"{" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("LBRACE\t{");
    return LBRACE;
}
"}" {
    if(dump_tokens)
        DEBUG_FOR_LAB4("RBRACE\t}");
    return RBRACE;
}

{DECIMIAL} {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    //sscanf函数作用为将指定格式的字符串转为数值
    int value;
    sscanf(yytext, "%d", &value);
    yylval.itype = value;
    return INTEGER;
}
{OCTAL} {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    int value;
    sscanf(yytext, "%o", &value);
    yylval.itype = value;
    return INTEGER;
}
{HEXADECIMAL} {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    int value;
    sscanf(yytext, "%x", &value);
    yylval.itype = value;
    return INTEGER;
}

{DECIMIALFLOAT} {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    float value;
    sscanf(yytext, "%f", &value);
    yylval.ftype = value;
    return FLOATNUMBER;
}
{OCTALFLOAT} {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    float value;
    sscanf(yytext, "%f", &value);
    yylval.ftype = value;
    return FLOATNUMBER;
}
{HEXADECIMALFLOAT} {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    float value;
    sscanf(yytext, "%f", &value);
    yylval.ftype = value;
    return FLOATNUMBER;
}
"starttime" {
    string func_name="_sysy_starttime";
    string message=func_name+std::to_string(yylineno);
    char *lexeme= new char[strlen(func_name.c_str()) + 1];
    char *message_lexeme=new char[strlen(message.c_str())+1];
    strcpy(lexeme, func_name.c_str());
    strcpy(message_lexeme,message.c_str());
    
    yylval.strtype = message_lexeme;
    SymbolEntry *se= identifiers->lookup(lexeme);
    if(se==nullptr)
    {
        vector<Type*> vec;
        vec.push_back(TypeSystem::intType);
        Type* funcType = new FunctionType(TypeSystem::voidType, vec);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, func_name, st->getLevel());//不输就是默认-1，也可显式写出
        st->install(func_name, se);
    }
    return ID;
}
"stoptime" {
    string func_name="_sysy_stoptime";
    string message=func_name+std::to_string(yylineno);
    char *lexeme= new char[strlen(func_name.c_str()) + 1];
    char *message_lexeme=new char[strlen(message.c_str())+1];
    strcpy(lexeme, func_name.c_str());
    strcpy(message_lexeme,message.c_str());
    
    yylval.strtype = message_lexeme;
    SymbolEntry *se= identifiers->lookup(lexeme);
    if(se==nullptr)
    {
        vector<Type*> vec;
        vec.push_back(TypeSystem::intType);
        Type* funcType = new FunctionType(TypeSystem::voidType, vec);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, func_name, st->getLevel());//不输就是默认-1，也可显式写出
        st->install(func_name, se);
    }
    return ID;
}

"getint" {
    //一定要在当前作用域查一下有没有这个getint名字，没有则置入
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry *se= identifiers->lookup(lexeme);       
    if(se==nullptr)
    {
        vector<Type*> vec;
        Type* funcType = new FunctionType(TypeSystem::intType, vec);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());//不输就是默认-1，也可显式写出
        st->install(yytext, se);
    }
    return ID;
}
"putint" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry *se = identifiers->lookup(lexeme);
    if(se==nullptr)
    {
        vector<Type*> vec;
        vec.push_back(TypeSystem::intType);
        Type* funcType = new FunctionType(TypeSystem::voidType, vec);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}

"getfloat" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry *se= identifiers->lookup(lexeme);       //一定要在当前作用域查一下有没有这个getfloat名字，没有则置入！！
    if(se==nullptr)
    {
        vector<Type*> vec;
        Type* funcType = new FunctionType(TypeSystem::floatType, vec);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}
"putfloat" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry *se= identifiers->lookup(lexeme);
    if(se==nullptr)
    {
        vector<Type*> vec;
        vec.push_back(TypeSystem::floatType);
        Type* funcType = new FunctionType(TypeSystem::voidType, vec);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}
"getch" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry* se=identifiers->lookup(lexeme);       
    if(se==nullptr)
    {
        vector<Type*> vec;
        Type* funcType = new FunctionType(TypeSystem::intType, vec);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}
"putch" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry *se= identifiers->lookup(lexeme);
    if(se==nullptr)
    {
        vector<Type*> vec;
        vec.push_back(TypeSystem::intType);
        Type* funcType = new FunctionType(TypeSystem::voidType, vec);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}
"putarray" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry* se=identifiers->lookup(lexeme);       
    if(se==nullptr)
    {
        //在sylib.c,函数putarray有两个参数，一个n和一个数组，打印数组中的n个元素
        vector<Type*> vec;
        vector<SymbolEntry*> vec1;
        ArrayType* arr = new ArrayType(TypeSystem::intType, 0);
        vec.push_back(TypeSystem::intType);
        vec.push_back(arr);
        Type* funcType = new FunctionType(TypeSystem::voidType, vec,vec1);

        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}

"getarray" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry* se=identifiers->lookup(lexeme);       
    if(se==nullptr)
    {
        vector<Type*> vec;
        vector<SymbolEntry*> vec1;
        ArrayType* arr = new ArrayType(TypeSystem::intType, 0);
        vec.push_back(arr);
        Type* funcType = new FunctionType(TypeSystem::intType, vec,vec1);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}
"putfarray" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry* se=identifiers->lookup(lexeme);       
    if(se==nullptr)
    {
        vector<Type*> vec;
        vector<SymbolEntry*> vec1;
        vec.push_back(TypeSystem::intType); 
        ArrayType* arr = new ArrayType(TypeSystem::floatType, 0);
        vec.push_back(arr);
        Type* funcType = new FunctionType(TypeSystem::voidType, vec, vec1);
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}

"getfarray" {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    SymbolEntry* se=identifiers->lookup(lexeme);       //一定要在当前作用域查一下有没有这个getfloat名字，没有则置入！！
    if(se==nullptr)
    {
        vector<Type*> vec;
        vector<SymbolEntry*> vec1;
        ArrayType* arr = new ArrayType(TypeSystem::floatType, 0);
        vec.push_back(arr);
        Type* funcType = new FunctionType(TypeSystem::intType, vec, vec1); 
        SymbolTable* st = identifiers;
        while(st->getPrev())
            st = st->getPrev();
        SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, st->getLevel());
        st->install(yytext, se);
    }
    return ID;
}

{ID} {
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme= new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    return ID;
}
{EOL}
{WHITE}
{SINGLECOMMENT}   {}
{COMMENTBEGIN} {BEGIN BLOCKCOMMENT;}
<BLOCKCOMMENT>{COMMENTELEMENT} {}
<BLOCKCOMMENT>{COMMENTEND} {BEGIN INITIAL;}
%%
