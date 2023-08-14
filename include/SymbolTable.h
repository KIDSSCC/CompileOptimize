#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <string>
#include <map>

class Type;
class Operand;

class SymbolEntry
{
private:
    int kind;
    SymbolEntry* next;  //函数重载，同名不同参返的才ok，通过next指针串联起来
protected:
    enum {CONSTANT, VARIABLE, TEMPORARY};
    Type *type;

public:
    SymbolEntry(Type *type, int kind)
    {
        this->type = type;
        this->kind = kind;
    }
    virtual ~SymbolEntry() {};
    bool isConstant() const {return kind == CONSTANT;};
    bool isTemporary() const {return kind == TEMPORARY;};
    bool isVariable() const {return kind == VARIABLE;};

    Type* getType() {return type;};
    void setType(Type *type) {this->type = type;};
    virtual std::string toStr() = 0;
    bool setNext(SymbolEntry* se);
    SymbolEntry* getNext() const { return next; };
};


/*  
    Symbol entry for literal constant. Example:

    int a = 1;

    Compiler should create constant symbol entry for literal constant '1'.
*/
class ConstantSymbolEntry : public SymbolEntry
{
private:
    double value;
public:
    ConstantSymbolEntry(Type *type, double value);
    ConstantSymbolEntry(Type* type): SymbolEntry(type, SymbolEntry::CONSTANT){};
    virtual ~ConstantSymbolEntry() {};
   double getValue(){return value;};
   void setValue(double value){this->value=value;};
    std::string toStr();
    
};


/* 
    Symbol entry for identifier. Example:

    int a;
    int b;
    void f(int c)
    {
        int d;
        {
            int e;
        }
    }

    Compiler should create identifier symbol entries for variables a, b, c, d and e:

    | variable | scope    |
    | a        | GLOBAL   |
    | b        | GLOBAL   |
    | c        | PARAM    |
    | d        | LOCAL    |
    | e        | LOCAL +1 |
*/
class IdentifierSymbolEntry : public SymbolEntry
{
private:
    enum {GLOBAL, PARAM, LOCAL};
    std::string name;
    //作用域，对应的是上面的global，param，还是local
    int scope;
    int label;  
    Operand *addr;  
    double value;
    
    int* intarrayvalue;
    float* floatarrayvalue;
    //数组初始化时是否为全0
    bool quan0;
    //作为函数参数传入时是第几个参数
    int AsParamNum;


public:
    IdentifierSymbolEntry(Type *type, std::string name, int scope,int AsParamNum=-1);
    virtual ~IdentifierSymbolEntry() {};
    std::string toStr();

    //关于scope
    bool isGlobal() const {return scope == GLOBAL;};
    bool isParam() const {return scope == PARAM;};
    bool isLocal() const {return scope >= LOCAL;};
    int getScope() const {return scope;};

    void setAddr(Operand *addr) {this->addr = addr;};
    Operand* getAddr() {return addr;};
    void setValue(double value){this->value=value;};
    double getValue(){return value;};

    void setLabel(int label){this->label=label;}
    int getLabel(){return label;}
    std::string getName(){return name;};

    void setIntArrayValue(int* intarrayvalue) {this->intarrayvalue = intarrayvalue;};    
    int* getIntArrayValue() const { return intarrayvalue; };
    void setFloatArrayValue(float* floatarrayvalue){this->floatarrayvalue = floatarrayvalue;};
    float* getFloatArrayValue() const { return floatarrayvalue; };
    void setquan0() { quan0 = true; };
    bool ifquan0() const { return quan0; };
    int getAsParamNum(){return AsParamNum;}
};


/* 
    Symbol entry for temporary variable created by compiler. Example:

    int a;
    a = 1 + 2 + 3;

    The compiler would generate intermediate code like:

    t1 = 1 + 2
    t2 = t1 + 3
    a = t2

    So compiler should create temporary symbol entries for t1 and t2:

    | temporary variable | label |
    | t1                 | 1     |
    | t2                 | 2     |
*/
class TemporarySymbolEntry : public SymbolEntry
{
private:
    int stack_offset;   
    int label;
public:
    TemporarySymbolEntry(Type *type, int label): SymbolEntry(type, SymbolEntry::TEMPORARY),label(label){};
    virtual ~TemporarySymbolEntry() {};
    std::string toStr();
    int getLabel() const {return label;};
    void setOffset(int offset) { this->stack_offset = offset; };
    int getOffset() { return this->stack_offset; };
};

// symbol table managing identifier symbol entries
class SymbolTable
{
private:
    std::map<std::string, SymbolEntry*> symbolTable;
    SymbolTable *prev;
    int level;
    static int counter;
public:
    SymbolTable();
    SymbolTable(SymbolTable *prev);
    void install(std::string name, SymbolEntry* entry);
    bool install1(std::string name, SymbolEntry* entry);//变量名重定义
    bool install2(std::string name, SymbolEntry* entry);//函数重载
    SymbolEntry* lookup(std::string name);
    SymbolTable* getPrev() {return prev;};
    int getLevel() {return level;};
    static int getLabel() {return counter++;};  //哪里会getLabel？创建临时变量时;bb的no设置时
    std::map<std::string, SymbolEntry*> getsymbolTable(){return symbolTable;}
};

extern SymbolTable *identifiers;
extern SymbolTable *globals;

#endif
