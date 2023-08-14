#ifndef __OPERAND_H__
#define __OPERAND_H__

#include "SymbolTable.h"
#include <vector>
#include<iostream>

class Instruction;
class Function;

// class Operand - The operand of an instruction.
class Operand
{
    //一个指令的动态数组迭代器，用于迭代use列表
typedef std::vector<Instruction *>::iterator use_iterator;

private:
    //作为一个操作数来说，他是在哪里被定义的
    Instruction *def;                
    //哪些指令用到了这个操作数
    std::vector<Instruction *> uses; 
    
    //这个操作数的符号表项
    SymbolEntry *se;                 
public:
    Operand(SymbolEntry*se) :se(se){def = nullptr;};

    void setDef(Instruction *inst) {def = inst;};
    void addUse(Instruction *inst) {uses.push_back(inst);};
    void removeUse(Instruction *inst);
    int usersNum() const {return uses.size();};

    use_iterator use_begin() {return uses.begin();};
    use_iterator use_end() {return uses.end();};
    Type* getType() {return se->getType();};
    Instruction* getDef() { return def; };
    SymbolEntry * getEntry() { return se; };
    
    //operand的toStr函数就是打印symbolentry
    std::string toStr() const;
};

#endif