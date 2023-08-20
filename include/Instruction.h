#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "Operand.h"
#include "AsmBuilder.h" //lab7新加！！
#include <vector>
#include <map>
#include <sstream>      //lab7新加！！

union FloattoUint32
{
    /* data */
    float floatnumber;
    unsigned long IEEEnumber;
};

class BasicBlock;

//指令基类
class Instruction
{
public:
//构造和析构，指令的类型，和一个基本块？
    Instruction(unsigned instType, BasicBlock *insert_bb = nullptr);
    virtual ~Instruction();

    //每一条指令应该都属于一个基本块
    BasicBlock *getParent();

    //指令类型判断，是不是条件分支指令或者无条件跳转
    bool isUncond() const {return instType == UNCOND;};
    bool isCond() const {return instType == COND;};
    bool isAlloc() const {return instType == ALLOCA;};
    bool isRet() const {return instType==RET;};
    bool isLoad() const {return instType==LOAD;};
    bool isStore() const {return instType==STORE;};
    bool isFuncCall() const {return instType==FUNCCALL;};
    bool isGep() const {return instType==GEP;};

    //设置所属的基本块
    void setParent(BasicBlock *);

    //上一条指令和后一条指令
    void setNext(Instruction *);
    void setPrev(Instruction *);
    Instruction *getNext();
    Instruction *getPrev();
    //打印
    virtual void output() const = 0;
    /*
    新增关于机器指令的输出
    genMachineOperand：获取中间代码操作数对应的机器指令操作数
    genMachineReg：物理寄存器，根据传入的整数返回一个寄存器类型的物理指令操作数
    genMachineVReg：虚拟寄存器，根据当前的临时标签返回一个虚拟寄存器类型的物理指令操作数
    genMachineImm：立即数，根据传入的值返回一个立即数类型的物理指令操作数
    genMachineLabel：地址标签，根据传入的块号返回一个地址标签类型的物理指令操作数
    genMachineCode：提供给其他指令实现的机器指令生成
    */
    MachineOperand* genMachineOperand(Operand*);//lab7以下新加
    MachineOperand* genMachineFloatOperand(Operand*);
    MachineOperand* genMachineReg(int reg,bool is_float);
    MachineOperand* genMachineVReg(bool is_float=false);
    MachineOperand* genMachineImm(int val);
    MachineOperand* genMachineLabel(int block_no);
    virtual void genMachineCode(AsmBuilder*) = 0;

    //用于全局变量局部化
    virtual std::vector<Operand*> getUse() { return std::vector<Operand*>(); }
    virtual Operand* getDef() { return nullptr; }
    virtual void replaceUse(Operand* old_operand, Operand* new_operand) {}
    virtual void replaceDef(Operand* new_operand) {}
protected:
    //指令类型
    unsigned instType;
    //操作码
    unsigned opcode;
    Instruction *prev;
    Instruction *next;
    BasicBlock *parent;
    std::vector<Operand*> operands;
    //指令的类型：位运算，条件，非条件，return，load，store，cmp。分配一个空间。
    enum {BINARY, COND, UNCOND, RET, LOAD, STORE, CMP, ALLOCA,XOR,FUNCCALL,ZEXT,GEP,I2F,F2I};
};

// meaningless instruction, used as the head node of the instruction list.
//假指令？无用指令。
class DummyInstruction : public Instruction
{
public:
    DummyInstruction() : Instruction(-1, nullptr) {};
    void output() const {};
    void genMachineCode(AsmBuilder*) {};    //lab7新加！！
};

class AllocaInstruction : public Instruction
{
public:
    //内存分配指令？有一个自己的符号表项，一个操作数
    AllocaInstruction(Operand *dst, SymbolEntry *se, BasicBlock *insert_bb = nullptr);
    ~AllocaInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！
private:
    SymbolEntry *se;
};

class LoadInstruction : public Instruction
{
public:
    //加载指令
    LoadInstruction(Operand *dst, Operand *src_addr, BasicBlock *insert_bb = nullptr);
    ~LoadInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    //全局变量局部化
    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[1]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

class StoreInstruction : public Instruction
{
public:
    StoreInstruction(Operand *dst_addr, Operand *src, BasicBlock *insert_bb = nullptr);
    ~StoreInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    //全局变量局部化
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[0], operands[1]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
};

class BinaryInstruction : public Instruction
{
public:
    enum {SUB,ADD,MUL,DIV,MOD,AND, OR};
    BinaryInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb = nullptr);
    ~BinaryInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    
    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[1], operands[2]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

class CmpInstruction : public Instruction
{
public:
    enum {E, NE, L, GE, G, LE};
    CmpInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb = nullptr);
    ~CmpInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[1], operands[2]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

// unconditional branch
class UncondBrInstruction : public Instruction
{
public:
    UncondBrInstruction(BasicBlock*, BasicBlock *insert_bb = nullptr);
    void output() const;
    void setBranch(BasicBlock *);
    BasicBlock *getBranch();
    void genMachineCode(AsmBuilder*);    //lab7新加！！
protected:
    //无条件跳转指令，branch是他要跳转的分支，是个基本块
    BasicBlock *branch;
};

// conditional branch
class CondBrInstruction : public Instruction
{
public:
    CondBrInstruction(BasicBlock*, BasicBlock*, Operand *, BasicBlock *insert_bb = nullptr);
    ~CondBrInstruction();
    void output() const;
    void setTrueBranch(BasicBlock*);
    BasicBlock* getTrueBranch();
    void setFalseBranch(BasicBlock*);
    BasicBlock* getFalseBranch();
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[0]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
protected:
    BasicBlock* true_branch;
    BasicBlock* false_branch;
};

class RetInstruction : public Instruction
{
public:
    RetInstruction(Operand *src, BasicBlock *insert_bb = nullptr);
    ~RetInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    std::vector<Operand*> getUse() 
    {
        if (operands.size()!=0)
            return std::vector<Operand*>({operands[0]});
        else
            return std::vector<Operand*>();
    }
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

class XorInstruction:public Instruction
{
public:
    XorInstruction(Operand* ,Operand*,BasicBlock *insert_bb = nullptr );
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[1]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

class FuncCallInstruction:public Instruction
{
private:
    SymbolEntry* func;
    Operand* dst;//自己加
public:
    FuncCallInstruction(Operand* ,SymbolEntry* ,std::vector<Operand*> ,BasicBlock* );
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！
    SymbolEntry* getFuncEntry(){return func;}

    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() 
    {
        std::vector<Operand*> vec;
        for (auto it = operands.begin() + 1; it != operands.end(); it++)
            vec.push_back(*it);
        return vec;
    }
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

class ZextInstruction:public Instruction
{
public:
    ZextInstruction(Operand*,Operand*,BasicBlock*);
    void output()const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[1]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

class GepInstruction : public Instruction {
   private:
    bool paramFirst;
    bool first;
    bool last;
    Operand* init;

   public:
    GepInstruction(Operand* dst,
                   Operand* arr,
                   Operand* idx,
                   BasicBlock* insert_bb = nullptr,
                   bool paramFirst = false);
    ~GepInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);
    void setFirst() { first = true; };
    void setLast() { last = true; };
    Operand* getInit() const { return init; };
    void setInit(Operand* init) { this->init = init; };

    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[1], operands[2]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

//类型转化指令
class I2FInstruction:public Instruction
{
private:
    Operand* dst;
    Operand* src;
public:
    I2FInstruction(Operand* dst,Operand* src,BasicBlock *insert_bb = nullptr );
    ~I2FInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[1]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};
class F2IInstruction:public Instruction
{
private:
    Operand* dst;
    Operand* src;
public:
    F2IInstruction(Operand* dst ,Operand* src,BasicBlock *insert_bb = nullptr );
    ~F2IInstruction();
    void output() const;
    void genMachineCode(AsmBuilder*);    //lab7新加！！

    Operand* getDef() { return operands[0]; }
    std::vector<Operand*> getUse() {return std::vector<Operand*>({operands[1]});}
    void replaceUse(Operand* old_operand, Operand* new_operand);
    void replaceDef(Operand* new_operand);
};

#endif